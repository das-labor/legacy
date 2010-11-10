#include "map_parser.h"


#ifdef XML_LARGE_SIZE
#if defined(XML_USE_MSC_EXTENSIONS) && _MSC_VER < 1400
#define XML_FMT_INT_MOD "I64"
#else
#define XML_FMT_INT_MOD "ll"
#endif
#else
#define XML_FMT_INT_MOD "l"
#endif

#define BUFFSIZE        8000000

/* base size of point database (will be realloc()'ed */
#define PDB_BASESIZE 8192
#define PDB_ALLOCSTEPSIZE 1024

char Buff[BUFFSIZE];
static uint_fast32_t depth = 0;

double x, y, max[2], min[2], bbox[2][2], proj[2];
double map_xpos = 0.0f, map_ypos = 0.0f, map_zpos = -20.0f;


/* <bounds minlat="51.47509" minlon="7.19276" maxlat="51.4855" maxlon="7.21057"/> */

typedef struct mappoint_s
{
	double lon, lat, ele;
	uint64_t id;
} mappoint_t;

typedef struct 
{
	char name[16];
	uint8_t colorcode;
} waytype_t;

const waytype_t waytypes[] =
{
	{ "motorway", 0 },
	{ "motorway_link", 0 },

	{ "trunk", 0 },
	{ "trunk_link", 0 },

	{ "primary", 1 },
	{ "primary_link", 1},

	{ "secondary", 2},
	{ "secondary_link", 2},

	{ "tertiary", 3},
	{ "unclassified", 5}, /* 10 */

	{ "road", 4},
	{ "residential", 4},

	{ "living_street", 6},
	{ "service", 0},

	{ "track", 7},
	{ "pedestrian", 7},

	{ "raceway", 8},
	{ "services", 0},

	{ "bus_guideway", 8},
	{ "path", 7},         /* 20 */

	{ "cycleway", 7},
	{ "footway", 7},

	{ "bridleway", 7},
	{ "byway", 7},

	{ "steps", 9},
	{ "building", 10 },

	{ "undefined", 11 } /* last must always be undef */
};

typedef struct
{
	uint8_t type;
	uint_fast32_t numpoints;
	uint64_t id;
	mappoint_t **points;
} mapway_t;

typedef struct
{
	vec3_t coords[4];
	uint8_t ccode;
} maptile_t;
static maptile_t *tiles;
static uint_fast32_t num_tiles = 0;

static uint_fast32_t num_ways = 0;
static mapway_t **waydb = NULL;
void waypoint_add (mapway_t *in_way, mappoint_t *in_point);

static mappoint_t **pointdb;
static size_t pointdb_buflen, pointdb_numpoints;
const double map_scalef = 1024.0f;

static float maprot[4]; /* not used yet */

typedef struct
{
	vec3_t pos;
	vec3_t rotv;
	double rotangle;
	double rotspeed;
	vec3_t color;
	uint_fast32_t statecounter;
} animated_entity_t;

static animated_entity_t ents[16];
static uint_fast32_t num_ents = 0;

static float ccodes[][4] =
{
	{0.2f, 0.2f, 1.0f, 1.0f},
	{0.2f, 0.2f, 0.2f, 1.0f},
	{0.4f, 0.4f, 0.4f, 1.0f},
	{0.6f, 0.6f, 0.6f, 1.0f},
	{0.8f, 0.8f, 0.8f, 1.0f},

	{0.2f, 0.2f, 0.2f, 1.0f},
	{0.2f, 0.2f, 0.2f, 1.0f},
	{0.8f, 1.0f, 0.8f, 1.0f},
	{0.2f, 0.2f, 0.2f, 1.0f},
	{1.0f, 0.0f, 0.0f, 1.0f},

	{1.0f, 0.7f, 0.0f, 1.0f},
	{0.1f, 0.1f, 0.1f, 1.0f}
};

uint8_t type_lookup (char* in_str)
{
	uint_fast8_t i;

	for (i=0;i<sizeof(waytypes) / sizeof(waytype_t);i++)
		if (!strncasecmp (in_str, waytypes[i].name, strlen(waytypes[i].name)))
			return i;
	
	return (sizeof(waytypes) / sizeof(waytype_t)) -1;
}

void draw_entites (void)
{
	uint_fast32_t i;
	static double wobble = 0.0f;
	double wsin = sin(wobble);
	
	wobble += M_PI/64;
	if (wobble > M_PI)
		wobble = M_PI/360;

	glBlendFunc (GL_ONE, GL_ZERO);
	for (i=0;i<num_ents;i++)
	{
		glTranslated (ents[i].pos[0], ents[i].pos[1], ents[i].pos[2]);
		glRotated (ents[i].rotangle, ents[i].rotv[0], ents[i].rotv[1], ents[i].rotv[2]);
		glBegin (GL_TRIANGLES);
			glColor4f (1.0f, 0.6f, 0.0f, 1.0f);
			glVertex3f (-0.3f, 0.0f, 0.0f); /* bottom peak */
			glColor4f(0.0f,0.0f,0.0f,0.0f);
			glVertex3f (-.5f - (0.2f * wsin), -.5f - (0.2f * wsin), 0.0f);
			glVertex3f (-.5f - (0.2f * wsin), .5f  + (0.2f * wsin), 0.0f);
			
			glColor4f (1.0f, 0.6f, 0.0f, 1.0f);
			glVertex3f (0.3f, 0.0f, 0.0f);
			glColor4f(0.0f,0.0f,0.0f,0.0f);
			glVertex3f (.5f + (0.2f * wsin), -.5f - (0.2f * wsin), 0.0f);
			glVertex3f (.5f + (0.2f * wsin), .5f + (0.2f * wsin), 0.0f);
		glEnd();
		ents[i].rotangle += ents[i].rotspeed;
	}

}

/* TODO:
	- get height from xml tag (if avail.)
	- find a way to distinguish between inner and outer building (backface culling!)
*/
void add_building (uint_fast32_t w)
{
	uint_fast16_t num_rooftiles,p,np;
	uint8_t ccode;
	uint_fast32_t ti = num_tiles;
	vec3_t roof[4];

	num_rooftiles = waydb[w]->numpoints / 4;
	if (waydb[w]->numpoints % 4)
		num_rooftiles++;

	num_tiles += waydb[w]->numpoints + num_rooftiles;
	tiles = realloc(tiles, num_tiles * sizeof(maptile_t));

	ccode = waytypes[type_lookup ("building")].colorcode;

	for (p=0;p<waydb[w]->numpoints;p++)
	{
		np = (p+1<waydb[w]->numpoints) ? p+1 : 0;

		tiles[ti].coords[0][0] = waydb[w]->points[p]->lon * map_scalef;
		tiles[ti].coords[0][1] = waydb[w]->points[p]->lat * map_scalef;
		tiles[ti].coords[0][2] = waydb[w]->points[p]->ele;
		
		tiles[ti].coords[1][0] = waydb[w]->points[p]->lon * map_scalef;
		tiles[ti].coords[1][1] = waydb[w]->points[p]->lat * map_scalef;
		tiles[ti].coords[1][2] = (waydb[w]->points[p]->ele + 0.25f); /* FIXED HEIGHT */
		
		tiles[ti].coords[2][0] = waydb[w]->points[np]->lon * map_scalef;
		tiles[ti].coords[2][1] = waydb[w]->points[np]->lat * map_scalef;
		tiles[ti].coords[2][2] = (waydb[w]->points[np]->ele + 0.25f); /* FIXED HEIGHT */
		
		tiles[ti].coords[3][0] = waydb[w]->points[np]->lon * map_scalef;
		tiles[ti].coords[3][1] = waydb[w]->points[np]->lat * map_scalef;
		tiles[ti].coords[3][2] = waydb[w]->points[np]->ele;
		
		tiles[ti].ccode = ccode;
		ti++;
		
		roof[p%4][0] = waydb[w]->points[p]->lon * map_scalef;
		roof[p%4][1] = waydb[w]->points[p]->lat * map_scalef;
		roof[p%4][2] = (waydb[w]->points[p]->ele + 0.25f);
		
		if (p>0 && !(p%4))
		{
			memcpy (tiles[ti].coords, roof, sizeof(vec3_t)*4);
			tiles[ti].ccode = 0;
			ti++;
		}
	}
}


void mapway_parse_type (uint8_t *out_type, char* in_str)
{
	uint_fast8_t i;
	for (i=0;i<sizeof(waytypes) / sizeof(waytype_t);i++)
	{
		if (!strncasecmp(in_str, waytypes[i].name, strlen(waytypes[i].name)))
		{
			*out_type = waytypes[i].colorcode;
			return;
		}
	}
	/* set unknown to unclassified */
	*out_type = type_lookup ("undefined");
}

mapway_t* mapway_add (uint64_t in_id, char* in_description)
{
	waydb = realloc (waydb, sizeof(void*) * (num_ways+1));
	waydb[num_ways] = malloc(sizeof (mapway_t));
	waydb[num_ways]->type = 5; /* type is set later */
	waydb[num_ways]->numpoints = 0;
	waydb[num_ways]->id = in_id;
	waydb[num_ways]->points = NULL;
	num_ways++;
	
	return  (waydb[num_ways-1]);
}

void waypoint_add_by_ref (mapway_t *in_way, uint64_t in_id)
{
	uint_fast32_t i;
	
	

	for (i=0;i<pointdb_numpoints;i++)
	{
		if (!memcmp (&pointdb[i]->id, &in_id, sizeof(uint64_t)) || pointdb[i]->id == in_id)
		{
			break;
		}
	}

	
	/* not found */
	if (i == pointdb_numpoints)
	{
		
		return;
	}
	waypoint_add (in_way, (pointdb[i]));
	
}

void waypoint_add (mapway_t *in_way, mappoint_t *in_point)
{
	in_way->points = realloc (in_way->points, sizeof(void*) * (in_way->numpoints +1));
	
	in_way->points[in_way->numpoints] = in_point;
	
	in_way->numpoints++;
	
}

void pointdb_init()
{
	pointdb = malloc (PDB_BASESIZE * sizeof(void*));
	pointdb_numpoints = 0;
	pointdb_buflen = PDB_BASESIZE * sizeof(void *);
}

void pointdb_addpoint (double lon, double lat, uint64_t);
void pointdb_addpoint (double lon, double lat, uint64_t in_id)
{
	
	
	if (pointdb_buflen < (pointdb_numpoints+1) * sizeof(void*))
	{
		pointdb = realloc (pointdb, pointdb_buflen + (sizeof(void*) * PDB_ALLOCSTEPSIZE));
	}
	pointdb_buflen += (PDB_ALLOCSTEPSIZE * sizeof(void*));

	
	pointdb[pointdb_numpoints] = malloc (sizeof(mappoint_t));
	pointdb[pointdb_numpoints]->lon = lon;
	pointdb[pointdb_numpoints]->lat = lat;
	pointdb[pointdb_numpoints]->ele = 0.0f;
	pointdb[pointdb_numpoints]->id = in_id;
	
	pointdb_numpoints++;
}

void map_set_pos_delta (double x, double y, double z)
{
	map_xpos += x;
	map_zpos += z;
	map_ypos += y;
}

void pointdb_drawpoints ()
{
	uint_fast32_t i,k;
	vec3_t r0,r1,r2,r3;
	double rd_scale;
	
	r1[2] = 0.0f;
	r2[2] = 0.0f;
	r0[2] = 0.0f;
	r3[2] = 0.0f;

	rd_scale = map_scalef * 0.00005f;
	
	glLoadIdentity();
	glBlendFunc (GL_ONE, GL_ZERO);
//	glTranslatef (map_xpos, map_ypos, map_zpos);
	glRotatef (-40.0f, 1.0f, 0.0f, 0.0f);
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	glBegin (GL_LINES);
		for (i=0;i<pointdb_numpoints;i++)
		{
			glVertex3f (pointdb[i]->lon * map_scalef, pointdb[i]->lat * map_scalef, 0.5f);
			glVertex3f (pointdb[i]->lon * map_scalef, pointdb[i]->lat * map_scalef, 0.0f);
		}
	glEnd();
	glColor4f (1.0f, 0.7f, 0.0f, 1.0f);
		for (i=0;i<num_ways;i++)
		{
			if (!waydb[i]->numpoints) continue;
			
			for (k=0;k<waydb[i]->numpoints-1;k++)
			{
		#if 0
				glBegin (GL_QUADS);
				r1[0] = waydb[i]->points[k]->lon * map_scalef;
				r1[1] = waydb[i]->points[k]->lat * map_scalef;
				vec3xyortho (r2, r1);
				vec3scale (r2, rd_scale);
				vec3add (r0, r1, r2);
				memcpy (r3, r0, sizeof(vec3_t));
				vec3inv(r2);
				vec3add (r0, r1, r2);
				/* ddir <-- */
				if (waydb[i]->points[k]->lon > waydb[i]->points[k+1]->lon)
				{
					glVertex3dv (r0);
					glVertex3dv (r3);
				} else
				{
					glVertex3dv (r3);
					glVertex3dv (r0);
				}

				r1[0] = waydb[i]->points[k+1]->lon * map_scalef;
				r1[1] = waydb[i]->points[k+1]->lat * map_scalef;
				vec3xyortho (r2, r1);
				vec3scale (r2, rd_scale);
//				vec3inv (r2);
				vec3add (r0, r1, r2);
				memcpy (r3, r0, sizeof(vec3_t));
				vec3inv (r2);
				vec3add (r0, r1, r2);

				if (waydb[i]->points[k]->lon > waydb[i]->points[k+1]->lon)
				{
					glVertex3dv (r3);
					glVertex3dv (r0);
				} else
				{
					glVertex3dv (r0);
					glVertex3dv (r3);
				}
				if (r0[2] != 0.0f)
					printf("!0\n");
				glEnd();
				glBegin(GL_LINES);
				glVertex3f (waydb[i]->points[k]->lon * map_scalef, waydb[i]->points[k]->lat * map_scalef, 0.0f);
				glVertex3f (waydb[i]->points[k+1]->lon * map_scalef, waydb[i]->points[k+1]->lat * map_scalef, 0.0f);
				glEnd();
		#endif
			}

		}
}

static void XMLCALL
start(void *data, const char *el, const char **attr)
{
	int i;
	uint_fast32_t k;
	static uint_fast8_t waystate = 0;
	double tmp_max[2], tmp_min[2];
	uint64_t id;
	static mapway_t *current_way = NULL;
	static uint8_t way_ccode;

	depth++;

	if (strlen(el) == 6 && !strncasecmp(el, "bounds", 6))
	{
	
		for (i=0; attr[i]; i += 2)
		{
			if (!strncasecmp (attr[i], "minlon", 6))
			{
				sscanf (attr[i+1], "%lf", &min[0]);
			}
			
			if (!strncasecmp (attr[i], "maxlon", 6))
			{
				sscanf (attr[i+1], "%lf", &max[0]);
			}
			
			if (!strncasecmp (attr[i], "minlat", 6))
			{
				sscanf (attr[i+1], "%lf", &min[1]);
			}
			
			if (!strncasecmp (attr[i], "maxlat", 6))
			{
				sscanf (attr[i+1], "%lf", &max[1]);
			}
		}
//		bbox[0] = { 0.0f, 0.0f };
//		bbox[1][] = { fabs(min[0]-max[0]), fabs(min[1]-max[1]) };
		
		/* calc projection coordinates */
		for (i=0;i<2;i++)
			proj[i] = (min[i] > max[i]) ? max[i] : min[i];

		return;
	}
	
	/* way start */
	if (strlen(el) == 3 && !strncasecmp (el, "way", 3))
	{
		uint64_t wayid;
		uint_fast8_t visible = 0;
	
		for (i=0;attr[i];i+=2)
		{
			if (!strncasecmp(attr[i], "id", 2))
			{
				sscanf (attr[i+1], "%llu", &wayid);
			}

			if (!strncasecmp (attr[i], "visible", 7) && !strncasecmp (attr[i+1], "true", 4))
			{
				visible = 1;
			}
		}
	
		if (visible)
		{
			 current_way = mapway_add (wayid, NULL);
		}
		return;
	}

	if (strlen(el) == 2 && !strncasecmp (el, "nd", 2))
	{
	
		for (i=0;attr[i];i+=2)
		{
			if (!strncasecmp ("ref", attr[i], 3) && strlen(attr[i]) == 3)
			{
				uint64_t tmp_id;
	
				sscanf(attr[i+1], "%llu", &tmp_id);
				waypoint_add_by_ref (current_way, tmp_id);
			}


		}
	}

	if (!strncasecmp ("tag", el, 3) && strlen(el) == 3)
	{
		for (i=0;attr[i];i+=2)
		{
			if (!strncasecmp ("k", attr[i], 1) && strlen(attr[i]) == 1 &&
				!strncasecmp("highway", attr[i+1], 7) && strlen(attr[i+1]) == 7)
			{
				i += 2;
				for (;attr[i];i+=2)
				{
					if (!strncasecmp ("v", attr[i], 1) && strlen(attr[i]) == 1)
					{
						if (current_way != NULL)
						{
							mapway_parse_type (&(current_way->type), attr[i+1]);
						}
					}
				}
			} else if (!strncasecmp ("k", attr[i], 1) && strlen(attr[i]) == 1 && // 0 &&
				!strncasecmp("building", attr[i+1], strlen(attr[i+1])) && strlen(attr[i+1]) == 8)
			{
				for (;attr[i];i+=2)
				{
					if (!strncasecmp ("v", attr[i], 1) && strlen(attr[i]) == 1
						&& !strncasecmp ("yes", attr[i+1], 3) && strlen(attr[i]) == 1)
					{
						/* this indeed is a building... whao. */
						if (current_way != NULL)
							current_way->type = 25; /* building */
					}
				}
			}
		}
	}

	
	if (strncmp(el, "node", 4))
		return;

//	
	

	for (i = 0; attr[i]; i += 2)
	{
		if (!strncmp(attr[i], "lat", 3))
		{
			sscanf (attr[i+1], "%lf", &y);
			continue;
		}

		if (!strncmp(attr[i], "lon", 3))
		{
			sscanf (attr[i+1], "%lf", &x);
			continue;
		}
		
		if (strlen(attr[i]) == 2 && !strncmp(attr[i], "id", 2))
		{
			sscanf (attr[i+1], "%llu", &id);
		}
	}
	
	pointdb_addpoint (x - proj[0], y - proj[1], id);
	
}

static void XMLCALL
end(void *data, const char *el)
{
	depth--;
}

void map_draw ()
{
	uint_fast32_t i, k, o;
	float fcol[3] = {0.0f, 0.0f, 0.0f};
	
	glBlendFunc (GL_ONE, GL_ZERO);
	//glBlendFunc (GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	for (i=0;i<num_tiles;i++)
	{
		glColor4fv (ccodes[tiles[i].ccode]);
		for (k=0;k<4;k++)
		{
			glVertex3dv (tiles[i].coords[k]);
		}
	}
	glEnd();
	draw_entites();
}

void map_load (const char* in_mapname)
{
	uint_fast32_t ti,p,w,cp,i,gapidx,gapoffset = 0;
	XML_Parser xp = XML_ParserCreate(NULL);
	vec3_t vp,vo,vr,vpo,vtmp, gapv[8];

	vp[2] = 0.0f;
	vo[2] = 0.0f;
	vr[2] = 0.0f;
	
	pointdb_init();
	
	if (!xp)
	{
		fprintf(stderr, "Couldn't allocate memory for parser\n");
		exit(-1);
	}

	XML_SetElementHandler(xp, start, end);

	for (;;)
	{
		int done;
		int len;
		FILE *fp;
		
		fp = fopen (in_mapname, "r");
		len = (int)fread(Buff, 1, BUFFSIZE, fp);
		if (ferror(fp))
		{
			fprintf(stderr, "Read error\n");
			exit(-1);
		}
		done = feof(fp);

		if (XML_Parse(xp, Buff, len, done) == XML_STATUS_ERROR)
		{
			fprintf(stderr, "Parse error at line %" XML_FMT_INT_MOD "u:\n%s\n",
			XML_GetCurrentLineNumber(xp),
			XML_ErrorString(XML_GetErrorCode(xp)));
			exit(-1);
		}

		if (done)
			break;
	}
	XML_ParserFree(xp);

	printf("processed %lu points, %lu ways\n", pointdb_numpoints, num_ways);
	
	/* transform the ways into maptiles */
	for (w=0;w<num_ways;w++)
	{
		if (!waydb[w]->numpoints) continue;

		if (waydb[w]->type == 25) /* building */
		{
			add_building (w);
			continue;
		}

		gapoffset = 0;

		ti = num_tiles; /* tile index */
		num_tiles += (2*waydb[w]->numpoints)-3;

		tiles = realloc(tiles, num_tiles * sizeof(maptile_t));
//		memset (tiles + (ti*sizeof(maptile_t)), 0x00, ((2*waydb[w]->numpoints)-3) * sizeof(maptile_t));

		for (p=0;p<waydb[w]->numpoints-1;p++)
		{
			gapoffset = (p%2) * 4;
			/* pick the leftmost point first... */
			if (waydb[w]->points[p]->lon <= waydb[w]->points[p+1]->lon)
			{
				cp = p;
				/* fill the other-vector array */
				vpo[0] = waydb[w]->points[p+1]->lon * map_scalef;
				vpo[1] = waydb[w]->points[p+1]->lat * map_scalef;
				vpo[2] = waydb[w]->points[p+1]->ele;
				gapidx = 1;
			} else
			{
				cp = p+1;
				vpo[0] = waydb[w]->points[p]->lon * map_scalef;
				vpo[1] = waydb[w]->points[p]->lat * map_scalef;
				vpo[2] = waydb[w]->points[p]->ele;
				gapidx = 0;
			}

			vp[0] = waydb[w]->points[cp]->lon * map_scalef;
			vp[1] = waydb[w]->points[cp]->lat * map_scalef;
			vp[2] = waydb[w]->points[cp]->ele;

			for (i=0;i<2;i++)
			{
				vec3xyortho (vo, vp, vpo);
				vec3scale (vo, 0.01f);

				/* un-twist the horizontal case */
				if (vr[1] > vp[1] && i == 1)
				{
					vec3inv (vo);
				}
				vec3add (vr, vo, vp);
				
				/* if result_vector[y] > vector_point[y] draw it first */
				if (vr[0] >= vp[0])
				{
					memcpy (tiles[ti].coords[3-i], vr, sizeof(vec3_t));
					vec3inv (vo);
					vec3add (vr, vp, vo);
					memcpy (tiles[ti].coords[0+i], vr, sizeof(vec3_t));
				} else
				{
					memcpy (tiles[ti].coords[0+i], vr, sizeof(vec3_t));
					vec3inv (vo);
					vec3add (vr, vp, vo);
					memcpy (tiles[ti].coords[3-i], vr, sizeof(vec3_t));
				}
				
				/* next-to-coming point: fill the next section */
				if (gapidx == i)
				{
					memcpy (gapv[0+(gapoffset^0x4)], tiles[ti].coords[3-i],
						sizeof(vec3_t));
					memcpy (gapv[3+(gapoffset^0x4)], tiles[ti].coords[0+i],
						sizeof(vec3_t));
				} else /* end of current gap */
				{
					memcpy (gapv[1+(gapoffset)], tiles[ti].coords[0+i],
						sizeof(vec3_t));

					memcpy (gapv[2+(gapoffset)], tiles[ti].coords[3-i],
						sizeof(vec3_t));
				}
				/* swap vectors, draw the other end of the track */
				memcpy (vtmp, vpo, sizeof(vec3_t));
				memcpy (vpo, vp, sizeof(vec3_t));
				memcpy (vp, vtmp, sizeof(vec3_t));
			}

			tiles[ti].ccode = waydb[w]->type;

			ti++;
			/* fill gaps */
			if (p>0 && p < waydb[w]->numpoints)
			{
				uint_fast8_t gi;
#if 1
				/* sort vectors */
				vec3sort4clockwise2d (gapv+gapoffset, gapv+gapoffset);

				for (gi=gapoffset;gi<4+gapoffset;gi++)
				{
					gapv[gi][2] = waydb[w]->points[p]->ele;
					memcpy(tiles[ti].coords[gi%4], gapv[gi], sizeof(vec3_t));
				}
				tiles[ti].ccode = tiles[ti-1].ccode;
				ti++;
#endif
			}
		}
	}

	ents[0].rotv[0] = 0.0f;
	ents[0].rotv[1] = 0.0f;
	ents[0].rotv[2] = 1.0f;
	ents[0].rotangle = 0.0f;
	ents[0].rotspeed = 3.0f;
	ents[0].pos[0] = (7.2087541f - proj[0]) * map_scalef;
	ents[0].pos[1] = (51.4806517f - proj[1]) * map_scalef;
	ents[0].pos[2] = 0.1f;
	ents[1].statecounter = 0;
	/* lab@	51.4806517, 7.2087541 */
	num_ents = 1;
	printf("\n");
	printf("proj: %lf %lf\n", proj[0], proj[1]);
	printf("ent: %lf %lf\n", ents[0].pos[0], ents[0].pos[1]);
	printf("%lu tiles processed\n", num_tiles);
}
