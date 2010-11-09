#include "3ds_draw.h"


static long
fileio_seek_func(void *self, long offset, Lib3dsIoSeek origin) {
    FILE *f = (FILE*)self;
    int o;
    switch (origin) {
        case LIB3DS_SEEK_SET:
            o = SEEK_SET;
            break;

        case LIB3DS_SEEK_CUR:
            o = SEEK_CUR;
            break;

        case LIB3DS_SEEK_END:
            o = SEEK_END;
            break;
    }
    return (fseek(f, offset, o));
}

static long
fileio_tell_func(void *self) {
    FILE *f = (FILE*)self;
    return(ftell(f));
}


static size_t
fileio_read_func(void *self, void *buffer, size_t size) {
    FILE *f = (FILE*)self;
    return (fread(buffer, 1, size, f));
}


static size_t
fileio_write_func(void *self, const void *buffer, size_t size) {
    FILE *f = (FILE*)self;
    return (fwrite(buffer, 1, size, f));
}


static void 
fileio_log_func(void *self, Lib3dsLogLevel level, int indent, const char *msg)
{
	return;
}






int tds_loadmodel (char* in_filename)
{
	return 0;
}

void* tds_modeldb (uint8_t in_command, void *in_data)
{
	static Lib3dsFile **mydb;
	static uint_fast32_t mydbsz = 0;

	switch (in_command)
	{
		case MDB_GET:
			if ((uint_fast32_t) in_data < mydbsz)
				return (void *) mydb[(uint_fast32_t) in_data];
			return NULL;
		break;
		case MDB_ADD:
			mydbsz++;
			mydb = realloc (mydb, mydbsz * sizeof(Lib3dsFile*));
			mydb[mydbsz-1] = malloc(sizeof(Lib3dsFile));
			memcpy(mydb[mydbsz-1], in_data, sizeof(Lib3dsFile));
			return (void *)  (mydbsz - 1);
		break;
		
		default:
		break;
	}

	return NULL;
}

void* tds_evp_db (uint8_t in_command, void *in_data)
{
	static tds_evp_t **mydb;
	static uint_fast32_t mydbsz = 0;

	switch (in_command)
	{
		case EVP_GET:
			if ((uint_fast32_t) in_data < mydbsz)
				return (void *) mydb[(uint_fast32_t) in_data];
			return NULL;
		break;
		case EVP_ADD:
			mydbsz++;
			mydb = realloc (mydb, mydbsz * sizeof(tds_evp_t*));
			mydb[mydbsz-1] = malloc(sizeof(tds_evp_t));
			memcpy(mydb[mydbsz-1], in_data, sizeof(tds_evp_t));
			return (void *)  (mydbsz - 1);
		break;
		
		default:
		break;
	}

	return NULL;
}

tds_modelnum tds_load_from_file (char* in_filename)
{
	Lib3dsIo tds_io;
	Lib3dsFile *tds_f = 0;
	int i;
	FILE *myfile;


	tds_f = lib3ds_file_new();

	myfile = fopen(in_filename, "rb");
	assert (myfile);

	memset(&tds_io, 0, sizeof(tds_io));
	tds_io.self = myfile;
	tds_io.seek_func = fileio_seek_func;
	tds_io.tell_func = fileio_tell_func;
	tds_io.read_func = fileio_read_func;
	tds_io.write_func = fileio_write_func;
	tds_io.log_func = fileio_log_func;

	assert (lib3ds_file_read(tds_f, &tds_io));
	fclose (myfile);

	/* free event points from mesh so they won't be displayed */
	for (i=0;i<tds_f->nmeshes;i++)
	{
		if (!strncasecmp (tds_f->meshes[i]->name, "evp_", 4))
		{
			tds_evp_t tmp;
			/* copy the first vertice we can find */
			lib3ds_vector_copy (tmp.coords, tds_f->meshes[i]->vertices[tds_f->meshes[i]->faces[0].index[0]]);
			tds_evp_db (EVP_ADD, &tmp);
			lib3ds_file_remove_mesh(tds_f, i);
			i=-1; /* start over */
		}
	}

	return (tds_modelnum) (tds_modeldb (MDB_ADD, tds_f));
}

double calc_length (GLfloat in_v1[3], GLfloat in_v2[3])
{
	double out;
	double l[3];
	uint8_t i;

	for (i=0;i<3;i++)
		l[i] = ((double) in_v2[i]) - ((double) in_v1[i]);
	
	out = sqrt ((double) (l[0] * l[0]) + (double) (l[1] * l[1]) + (double) (l[2] * l[2]));

	if (out == 0.0f)
		out = 1.0f;
	
	return out;
}

int tds_drawmodel (tds_modelnum in_modelnum)
{
	uint_fast32_t h,i;
	float bbox[2][3];
	Lib3dsFile *tds_f = 0;
	GLfloat cp[3][3];
	tds_evp_t *evp;


	tds_f = tds_modeldb (MDB_GET, NULL);
	lib3ds_mesh_bounding_box(tds_f->meshes[0], bbox[0], bbox[1]);

	/* Print Faces */

	glBlendFunc (GL_ONE, GL_ZERO);
	glColor4f (1.0f, 1.0f, 1.0f, 0.5f);
	for (h=0;h<tds_f->nmeshes;h++)
	{
		for (i=0;i<(tds_f->meshes[h])->nfaces;i++)
		{
			lib3ds_vector_copy (cp[0], tds_f->meshes[h]->vertices[tds_f->meshes[h]->faces[i].index[0]]);
			lib3ds_vector_copy (cp[1], tds_f->meshes[h]->vertices[tds_f->meshes[h]->faces[i].index[1]]);
			lib3ds_vector_copy (cp[2], tds_f->meshes[h]->vertices[tds_f->meshes[h]->faces[i].index[2]]);


			glBegin(GL_TRIANGLES);
			glNormal3f (1.0f,1.0f,1.0f);
			if (tds_f->meshes[h]->texcos)
			{
				glTexCoord2fv (tds_f->meshes[h]->texcos[0]);
				glVertex3fv(cp[2]);
				glTexCoord2fv (tds_f->meshes[h]->texcos[1]);
				glVertex3fv(cp[0]);
				glTexCoord2fv (tds_f->meshes[h]->texcos[2]);
				glVertex3fv(cp[1]);
			} else
			{
				glVertex3fv(cp[2]);
				glVertex3fv(cp[0]);
				glVertex3fv(cp[1]);
			}
			glEnd();
		}
	}
	
	i=0;
	evp = (tds_evp_t*) tds_evp_db(EVP_GET, i);

	while (evp != NULL)
	{
		glBegin (GL_LINES);
			glVertex3f (evp->coords[0], evp->coords[1], 10.0f);
			glVertex3f (evp->coords[0], evp->coords[1], -10.0f);
		glEnd();
		evp = (tds_evp_t*) tds_evp_db(EVP_GET, (void*) abs(i));
		i++;
	}
	return 1;
}
