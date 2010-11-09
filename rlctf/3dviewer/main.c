#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#include "font.h"
#include "infowin.h"
#include "building.h"
#include "lib/3ds_draw.h"
#include "lib/textures.h"
#include "map_parser.h"
#include "view.h"
#include "flight.h"

int sdlvflags;
SDL_Surface *mysurface;
static vp_t views[2];
	const SDL_VideoInfo *sdlvinfo;


void updatewindow (uint16_t in_w, uint16_t in_h)
{
	GLfloat myratio;

	if (!in_w || !in_h) return;

	myratio = ((GLfloat) in_w / (GLfloat) in_h);
	glViewport (0, 0, (GLint) in_w, (GLint) in_h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (45.0f, myratio, 0.1f, 100.0f);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void initgl()
{
	GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightDiffuse[]= { 0.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition[]= { 0.0f, 3.0f, -50.0f, 1.0f };

	glShadeModel(GL_SMOOTH);
	glShadeModel(GL_LINE_SMOOTH);
	glClearDepth (10.0f);
//	glEnable(GL_BLEND);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glBlendFunc (GL_SRC_ALPHA, GL_SRC_ALPHA);
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

//	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);	
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);


	glMatrixMode(GL_MODELVIEW);
	loadtexture ("textures/logo.bmp");
	loadtexture ("textures/stone.bmp");
//	enable_autotexturing();
}


void sdlstuff_init ()
{
	/* *sigh* all the funny stuff sdl needs... */
	if (SDL_Init (SDL_INIT_VIDEO) < 0)
	{
		
		exit (-1 * __LINE__);
	}

	sdlvinfo = SDL_GetVideoInfo ();

	if (!sdlvinfo)
	{
		
		exit (-1 * __LINE__);
	}

	sdlvflags = (SDL_OPENGL |  SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE | SDL_RESIZABLE);

	if ( sdlvinfo->blit_hw )
		sdlvflags |= SDL_HWACCEL;

	if (sdlvinfo->hw_available)
	{
		sdlvflags |= SDL_HWSURFACE;
	} else
	{
		sdlvflags |= SDL_SWSURFACE;
	}

	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
	mysurface = SDL_SetVideoMode (sdlvinfo->current_w-20, sdlvinfo->current_h-200, 16, sdlvflags);

	if (!mysurface)
	{
		
		exit (-1 * __LINE__);
	}

	SDL_EnableKeyRepeat(25, SDL_DEFAULT_REPEAT_INTERVAL);


	/* ...finally... let's power up the gl stuff... */

}

void scene_init ()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth( 300.0f );
	glLoadIdentity();
}


int main (int argc, char* argv[])
{
	SDL_Event sdlev;
	uint8_t dodraw = 1, draw_minimap = 0, opt_editmode = 0;
	uint_fast8_t i;
	float lz = 0.0f, lx = 0.0f, ly = 10.0f;

	froute_t myroute[4];
	fpoint_t routepoints[4] =
	{
		{
			{20.0f,   5.0f,  3.0f},
			{26.8f,  11.8f,   0.0f},
			{0.0f,   0.0f,   1.0f}
		},
		{
			{20.0f,   15.0f,  10.0f},
			{24.8f,  11.8f,   0.0f},
			{0.0f,   0.0f,   1.0f}
		},
		{
			{30.0f,   15.0f,  10.0f},
			{20.8f,  11.8f,   0.0f},
			{0.0f,   0.0f,   1.0f}
		},
		{
			{30.0f,   5.0f,  1.0f},
			{26.8f,  11.8f,   0.0f},
			{0.0f,   0.0f,   1.0f}
		}
	};
	froute_t route_ref;

	for (i=0;i<4;i++)
	{
		myroute[i].progress = 0.0f;
		myroute[i].p = &routepoints[i];
		myroute[i].next = &myroute[(i+1)%4];
	}

	for (i=0;i<4;i++)
		flight_gen_transv (&myroute[i]);

	memcpy (&route_ref, &myroute[0], sizeof(froute_t));
	route_ref.p = malloc(sizeof(fpoint_t));
	memcpy (route_ref.p, myroute[0].p, sizeof(fpoint_t));
	
	sdlstuff_init();
	view_init(&views[0]);
	view_init(&views[1]);
	view_move (&views[0], -30.0f, -20.0f, -20.0f);
	view_move (&views[1], -30.0f, -20.0f, -5.0f);
	
	initgl();

	map_load ("map.osm");
	

	buildFont (120);
	select_font (120);

	updatewindow (1366, 768);
	while (42)
	{
		while (SDL_PollEvent (&sdlev))
		{
			switch( sdlev.type )
			{
				case SDL_ACTIVEEVENT:
					dodraw = 1;

					if (sdlev.active.gain == 0)
						dodraw = 0;
				break;
				case SDL_VIDEORESIZE:
					mysurface = SDL_SetVideoMode
					(
						sdlev.resize.w,
						sdlev.resize.h,
						16, sdlvflags
					);
					updatewindow (sdlev.resize.w, sdlev.resize.h);
				break;
				
				case SDL_KEYDOWN:
					switch (sdlev.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							return 0;
						break;

						case SDLK_DOWN:
							lz+=0.1f;
							view_move (&views[0], 0.0f, 0.4f, -0.076604444f);
							view_move (&views[1], 0.0f, 0.4f, 0.0f);
						break;

						case SDLK_UP:
							lz-= 0.1f;

							view_move (&views[0], 0.0f, -0.4f, 0.076604444f);
							view_move (&views[1], 0.0f, -0.4f, 0.0f);
						break;

						case SDLK_LEFT:
							lx-= 0.1f;
							view_move (&views[0], 0.4f, 0.0f, 0.0f);
							view_move (&views[1], 0.4f, 0.0f, 0.0f);
						break;
						
						case SDLK_RIGHT:
							lx+= 0.1f;
							view_move (&views[0], -0.4f, 0.0f, 0.0f);
							view_move (&views[1], -0.4f, 0.0f, 0.0f);
						break;

						case SDLK_PAGEUP:
							ly += 0.1f;
							view_move (&views[0], 0.0f, 0.0f, 0.5f);
						break;

						case SDLK_PAGEDOWN:
							ly -= 0.1f;
							view_move (&views[0], 0.0f, 0.0f, -0.5f);
						break;

						case SDLK_t:
							print_txt ("t!! gvggra");
						break;

						case SDLK_x:
							print_txt ("INCOMING CALL:\n+49133731337");
						break;

						case SDLK_e:
							opt_editmode ^= 0x01;
						break;


						default:
						break;
					}
					set_yline (lx, ly, lz);
				break;

				case SDL_MOUSEMOTION:
					set_rot (sdlev.button.x / 4);
					if (SDL_GetMouseState (NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
						map_set_pos_delta (sdlev.motion.xrel / 8, sdlev.motion.yrel / 8, 0.0f);
				break;

				case SDL_MOUSEBUTTONDOWN:
					switch (sdlev.button.button)
					{
						case SDL_BUTTON_WHEELUP:
							set_zoom_relative (0.5f);
							map_set_pos_delta (0.0f, -0.3213938f, 0.5f);
						break;
						case SDL_BUTTON_WHEELDOWN:
							set_zoom_relative (-0.5f);
							map_set_pos_delta (0.0f, 0.3213938f, -0.5f);
						break;
						default:
						break;
					}
				break;
				
				
				default:
				break;
			}
		}
		
		if (!dodraw)
			continue;
		

		glViewport (0, 0, sdlvinfo->current_w, sdlvinfo->current_h);
		scene_init();
		glLoadIdentity();
		if (opt_editmode)
		{
			view_set(&views[0]);
		} else
		{
			flight_iterate_percent (&route_ref, 0.002f);
			flight_set_view (&route_ref);
		}
		
		glColor3f (1.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
		for (i=0;i<4;i++)
		{
			glVertex3f (routepoints[i].eye[0], routepoints[i].eye[1], routepoints[i].eye[2]);
			glVertex3f (routepoints[(i+1)%4].eye[0],routepoints[(i+1)%4].eye[1], routepoints[(i+1)%4].eye[2]);
		}
		glEnd();
		
		map_draw();


		if (draw_minimap)
		{
			/* minimap */
			glViewport (1266, 668, 100, 100);
			view_set(&views[1]);
			glClearDepth(300.0f);
			glBegin(GL_QUADS);
				glColor4f (0.0f, 0.5f, 0.0f, 1.0f);
				glVertex3f (-10.0f, -5.0f,  -10.0f);
				glVertex3f (10.0f, -5.0f, -10.0f);
				glVertex3f (10.0f,  5.0f, -10.0f);
				glVertex3f (-10.0f,  5.0f,  -10.0f);
			glEnd();
			glColor3f (1.0f, 0.0f, 0.0f);
			glBegin(GL_LINES);
			for (i=0;i<4;i++)
			{
				glVertex3f (routepoints[i].eye[0], routepoints[i].eye[1], routepoints[i].eye[2]);
				glVertex3f (routepoints[(i+1)%4].eye[0],routepoints[(i+1)%4].eye[1], routepoints[(i+1)%4].eye[2]);
			}
			glEnd();
			map_draw();
			draw_infowin();
		}
		glFlush();

		SDL_GL_SwapBuffers ();
	}

	return 0;
}
