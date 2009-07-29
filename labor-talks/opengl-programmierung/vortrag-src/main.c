/*
 *  Copyright (C) 2009 by Soeren Heisrath <forename@surename dot org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to:
 *  Free Software Foundation
 *  51 Franklin Street, Fifth Floor
 *  Boston, MA  02111-1301  USA
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#include "slides.h"
#include "effects.h"
#include "textures.h"

SDL_Surface *mysurface;



void scene_init();
void dummyscene ();
void (*sceneptr)(void) = dummyscene;

float xpos = -0.7f, ypos = 0.0f, zpos = -3.53f;
static GLfloat xrot = -0.0f, yrot = 0.0f, zrot = 0.0f;
static uint32_t slidenum = 1;


static GLuint mytextures[8];



void cleanup (int in_retcode)
{
    SDL_Quit();
    exit(in_retcode);
}

void dummyscene ()
{
	static uint8_t init_done = 0;
	static GLuint dummy_texture[4];

	scene_init();
	glLoadIdentity ();
	
	glTranslatef (xpos, ypos, zpos);
	glRotatef (xrot, 1.0f, 0.0f, 0.0f);
	glRotatef (yrot, 0.0f, 1.0f, 0.0f);
	glRotatef (zrot, 0.0f, 0.0f, 1.0f);
	if (!init_done)
	{
		float maxant;

		glEnable (  GL_NORMALIZE );
		glEnable (  GL_AUTO_NORMAL );

		SDL_Surface *mytexture[1];
		mytexture[0] = SDL_LoadBMP ("../bitmaps/grass.bmp");
		glGenTextures (8, mytextures);

		glBindTexture (GL_TEXTURE_2D, mytextures[0]);

		glTexImage2D (GL_TEXTURE_2D, 0, 3, mytexture[0]->w,
			mytexture[0]->h, 0, GL_BGR,
			GL_UNSIGNED_BYTE, mytexture[0]->pixels);
		
		glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		gluBuild2DMipmaps (GL_TEXTURE_2D, 3, mytexture[0]->w,
			mytexture[0]->h, GL_BGR,
			GL_UNSIGNED_BYTE, mytexture[0]->pixels);

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glGetFloatv (GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxant);

		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxant);

		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenf(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

	

		SDL_FreeSurface (mytexture[0]);
		mytextures[3] = loadtexture ("../bitmaps/grass.bmp");

		init_done = 1;
	}

	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
	glColor4f (1.0f,1.0f,1.0f, 1.0f);

	showslide (slidenum);
	glTranslatef (xpos, ypos, zpos);
	effect_handler (EF_TICK);

	gluLookAt (xpos, ypos, zpos, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);


	SDL_GL_SwapBuffers ();
}

void keyhandler(SDL_keysym *in_key)
{
	static uint8_t grabstate = 0x00;

	switch (in_key->sym)
	{
		case SDLK_ESCAPE:
			cleanup (0);
		break;

		case SDLK_f:
			SDL_WM_ToggleFullScreen (mysurface);
		break;

		case SDLK_LEFT:
			xpos += 0.1f;
		break;

		case SDLK_RIGHT:
			xpos -= 0.1f;
		break;

		case SDLK_UP:
			zpos += 0.1f;
		break;

		case SDLK_DOWN:
			zpos -= 0.1f;
		break;
		
		case SDLK_PAGEUP:
			ypos += 0.1f;
		break;

		case SDLK_PAGEDOWN:
			ypos -= 0.1f;
		break;


		case SDLK_i:
			grabstate ^= 0x01;
			printf("input grabbing: %u\n", grabstate);

			if (grabstate)
			{
				SDL_ShowCursor(0);
				SDL_WM_GrabInput(SDL_GRAB_ON);
			}
			else
			{
				SDL_ShowCursor(1);
				SDL_WM_GrabInput(SDL_GRAB_OFF);
			}
		break;

		case SDLK_s:
			slidestate (0xff);
		break;

		case SDLK_n:
			slidenum++;
		break;

		case SDLK_r:
			glLoadIdentity();	
			xpos = -0.7f, ypos = 0.0f, zpos = -3.53f;
			xrot = -0.0f, yrot = 0.0f, zrot = 0.0f;
		break;

		case SDLK_p:
			slidenum--;
		break;

		case SDLK_F1:
			effect_handler (EF_DISABLE);
		break;

		case SDLK_F2:
			effect_handler (EF_ENABLE);
		break;
		
		case SDLK_F3:
			effect_handler (EF_LAST);
		break;
		
		case SDLK_F4:
			effect_handler (EF_NEXT);
		break;

		case SDLK_F5:
			effect_handler (EF_SWITCH);
		break;

		default:
		break;
	}
}

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

/* set basics for every scene */
void scene_init()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth( 10.0f );
}

/* called once when initializing */
void initgl()
{

	GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 0.5f };
	GLfloat LightDiffuse[]= { 0.1f, 0.1f, 0.1f, 0.1f };
	GLfloat LightPosition[]= { 100.0f, 10.0f, -20.0f, 1.0f };

	scene_init();

	glEnable (GL_TEXTURE_2D);
	glEnable (GL_TEXTURE_3D);
	glShadeModel(GL_SMOOTH);
	glClearDepth (100.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);	
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);
	slides_init(mytextures);
}

int main (int argc, char *argv[])
{
	int sdlvflags;
	SDL_Event sdlev;
	const SDL_VideoInfo *sdlvinfo;
	uint8_t dodraw = 1;
	uint8_t btnstate = 0;
	


	/* *sigh* all the funny stuff sdl needs... */
	if (SDL_Init (SDL_INIT_VIDEO) < 0)
	{
		printf("Gaaah, SDL error... sorry (%s)\n", SDL_GetError());
		exit (-1 * __LINE__);
	}

	sdlvinfo = SDL_GetVideoInfo ();

	if (!sdlvinfo)
	{
		printf("Gaaah, SDL error when trying to get video info... sorry (%s)\n", SDL_GetError());
		exit (-1 * __LINE__);
	}

	sdlvflags = (SDL_OPENGL |  SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE | SDL_RESIZABLE);

	if ( sdlvinfo->blit_hw )
		sdlvflags |= SDL_HWACCEL;

	if (sdlvinfo->hw_available)
		sdlvflags |= SDL_HWSURFACE;
	else
		sdlvflags |= SDL_SWSURFACE;

	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
	mysurface = SDL_SetVideoMode (1366, 768, 16, sdlvflags);

	if (!mysurface)
	{
		printf("Gaaah, SDL error when trying to create surface... sorry (%s)\n", SDL_GetError());
		exit (-1 * __LINE__);
	}

    	SDL_EnableKeyRepeat(25, SDL_DEFAULT_REPEAT_INTERVAL);


	/* ...finally... let's power up the gl stuff... */
	initgl();

	updatewindow (1366,768);

	/* wait for events */
	while (23)
	{
		while (SDL_PollEvent (&sdlev))
		{
			switch( sdlev.type )
			{
				case SDL_MOUSEBUTTONDOWN:
					switch (sdlev.button.button )
					{
						case SDL_BUTTON_LEFT:
							btnstate |= 0x04;
						break;
						case SDL_BUTTON_RIGHT:
							btnstate |= 0x01;
						break;
						case SDL_BUTTON_WHEELUP:
							zpos += 0.1f;
						break;
						case SDL_BUTTON_WHEELDOWN:
							zpos -= 0.1f;
						break;
					}
				break;
				case SDL_MOUSEBUTTONUP:
					switch (sdlev.button.button )
					{
						case SDL_BUTTON_LEFT:
							btnstate &= ~(0x04);
						break;
						case SDL_BUTTON_RIGHT:
							btnstate &= ~(0x01);
						break;
					}
				break;

				case SDL_MOUSEMOTION:
					if (btnstate & 0x01)
					{
						yrot += (((sdlev.motion.xrel)) * 0.1f);
						xrot += (((sdlev.motion.yrel)) * 0.1f);
					}
					if (btnstate & 0x04)
					{
						xpos -= (((sdlev.motion.xrel)) * 0.05f);
						ypos += (((sdlev.motion.yrel)) * 0.05f);
					}
					if (btnstate == 0x00 && 0)
					{
						yrot += (((sdlev.motion.xrel)) * 0.1f);
						xrot += (((sdlev.motion.yrel)) * 0.1f);
					}
				break;

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
					keyhandler (&sdlev.key.keysym);
				break;

				case SDL_QUIT:
					cleanup(0);
				break;

				default:
				break;
			}
		}

		if (dodraw)
		{
			sceneptr();
		}
	}
	cleanup (0);
}
