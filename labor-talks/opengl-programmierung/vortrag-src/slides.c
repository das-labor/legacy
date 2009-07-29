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
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define TEX_CURRENT 0x01
#define TEX_LAST    0x02
#define TEX_SWAP    0x0f

static uint8_t en_slides = 1;
static GLuint *mytextures;

void slidestate(uint8_t in_state)
{
	if (in_state == 0xff)
	{
		en_slides ^= 0x01;
		return;
	}
	
	en_slides = in_state;
}

void slides_init(GLuint *in_texs)
{
	mytextures = in_texs;
}

int loadslide (uint8_t in_target, uint32_t in_slide)
{
	char slidename[512];
	int t;
	SDL_Surface *texsurf[1];

	snprintf (slidename, 512, "../slides/slide-%02u.bmp", in_slide);
	t = SDL_LoadBMP (slidename);

	if (t < 0)
		return t;
	
	texsurf[0] = abs(t);

	glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
	glBindTexture (GL_TEXTURE_2D, mytextures[in_target]);

	glTexImage2D (GL_TEXTURE_2D, 0, 3, texsurf[0]->w,
		texsurf[0]->h, 0, GL_BGR,
		GL_UNSIGNED_BYTE, texsurf[0]->pixels);
	
	gluBuild2DMipmaps (GL_TEXTURE_2D, 3, texsurf[0]->w,
		texsurf[0]->h, GL_BGR,
		GL_UNSIGNED_BYTE, texsurf[0]->pixels);
	
	return 0;
}

void showslide (uint32_t in_slide)
{
	static uint32_t current_slide = 0xffff;
	static float current_alpha = 0.0f;

	glEnable (GL_BLEND); 
	if (current_slide != in_slide)
	{
		mytextures[TEX_LAST] = mytextures[TEX_CURRENT];
		loadslide (TEX_CURRENT, in_slide);

		current_slide = in_slide;
		current_alpha = 0.0f;
	}

	if (!en_slides)
	{
		glTranslatef (0.0f, -4.0f, 0.0f);
		glRotatef (90.0f, 1.0f, 0.0f, 0.0f);
	}

	if (current_alpha < 1.0f)
	{
		current_alpha += 0.02f;

		glColor4f (1.0f,1.0f,1.0f, 1.0f - current_alpha);
		glRotatef (current_alpha * 90.0f, -0.8f, 0.3f, 1.0f);
		glBindTexture (GL_TEXTURE_2D, mytextures[TEX_LAST]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f,  1.5f,  -9.0f * current_alpha);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 2.0f,  1.5f,  -9.0f * current_alpha);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 2.0f, -1.5f,  -9.0f * current_alpha);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.0f, -1.5f,  -9.0f * current_alpha);
		glEnd();
		glRotatef (current_alpha * 90.0f, 0.8f, -0.3f, -1.0f);
	}

		glBindTexture (GL_TEXTURE_2D, mytextures[TEX_CURRENT]);
	glColor4f (1.0f,1.0f,1.0f, current_alpha);
	glBegin(GL_QUADS);
		glBindTexture (GL_TEXTURE_2D, mytextures[TEX_CURRENT]);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f,  1.5f,  0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 2.0f,  1.5f,  0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 2.0f, -1.5f,  0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.0f, -1.5f,  0.0f);
	glEnd();
	
	if (!en_slides)
	{
		glRotatef (-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef (0.0f, 4.0f, 0.0f);
	}
	glDisable (GL_BLEND); 
}


