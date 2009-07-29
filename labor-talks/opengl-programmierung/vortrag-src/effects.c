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

#include "effects.h"
#include "textures.h"

static uint8_t demostate = 0;

void effect_handler (uint8_t in_cmd)
{
	static uint8_t efnum = 0;

	printf("efhandler %u\n", in_cmd);

	switch (in_cmd)
	{
		case EF_NEXT:
			if (efnum < 0xff) efnum++;
			demostate = 0;
		break;

		case EF_LAST:
			if (efnum > 0) efnum--;
			demostate = 0;
		break;

		case EF_DISABLE:
			demostate = 0;
			return;

		case EF_SWITCH:
			demostate++;
		break;

		default:
		break;
		
	}
	
	switch (efnum)
	{
		case 0:
			show_coordsys();
		break;
		case 1:
			show_triangle();
		break;
		case 2:
			show_repeatexample();
		break;
		case 10:
			draw_landscape(1);
		break;
	}
}

void drawfog()
{
	GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	
	GLuint fogfilter= 2;
	GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f};		

	
	glClearColor(0.5f,0.5f,0.5f,1.0f);			

	glFogi(GL_FOG_MODE, fogMode[fogfilter]);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.05f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, -10.0f);
	glFogf(GL_FOG_END, 15.0f);
	glEnable(GL_FOG);
}


void effect_intro ()
{
	static float current_rot = 0.0f;

	if (current_rot < 90.0f)
	{
		current_rot += 5.0f;
	}
	glRotatef (current_rot, 1.0f, 1.0f, 0.0f);
}

void show_triangle ()
{
	glDisable (GL_TEXTURE_2D);
	glBegin (GL_TRIANGLES);
		glColor4f (1.0f, 0.0f,0.0f, 1.0f);
		glVertex3f (0.0f, 1.0f, 1.0f);
		glColor4f (0.0f, 1.0f,0.0f, 1.0f);
		glVertex3f (-1.0f, 0.0f, 1.0f);
		glColor4f (0.0f,0.0f,1.0f, 1.0f);
		glVertex3f (1.0f, 0.0f, 1.0f);
	glEnd();
	glEnable (GL_TEXTURE_2D);
}

void show_coordsys ()
{
	glEnable (GL_BLEND); 
	glDisable (GL_TEXTURE_2D);
	glBegin (GL_LINES);
		glColor4f (1.0f,0.0f,0.0f, 1.0f);
		glVertex3f (-1.0f, 0.0f, 0.0f);
		glVertex3f (1.0f,  0.0f, 0.0f);

		glColor4f (0.0f,  1.0f,0.0f, 1.0f);
		glVertex3f (0.0f, -1.0f, 0.0f);
		glVertex3f (0.0f,  1.0f, 0.0f);

		glColor4f (1.0f,  1.0f,  0.0f, 1.0f);
		glVertex3f (0.0f,  0.0f, -1.0f);
		glVertex3f (0.0f,  0.0f, 1.0f);
	glEnd();

	/* arrows */
	glBegin (GL_TRIANGLES);
		glColor4f (1.0f,0.0f,0.0f, 1.0f);
		glVertex3f (1.0f, 0.0f, 0.0f);
		glColor4f (1.0f,0.0f,0.0f, 0.0f);
		glVertex3f (0.90f, 0.03f, 0.0f);
		glColor4f (1.0f,0.0f,0.0f, 0.0f);
		glVertex3f (0.90f, -0.03f, 0.0f);

		glColor4f (0.0f,1.0f,0.0f, 1.0f);
		glVertex3f (0.0f, 1.0f, 0.0f);
		glColor4f (0.0f,1.0f,0.0f, 0.0f);
		glVertex3f (0.03f, 0.90f, 0.0f);
		glColor4f (0.0f,1.0f,0.0f, 0.0f);
		glVertex3f (-0.03f, 0.90f, 0.0f);

		glColor4f  (1.0f,1.0f,0.0f, 1.0f);
		glVertex3f (0.0f, 0.0f, 1.0f);
		glColor4f  (1.0f,1.0f,0.0f, 0.0f);
		glVertex3f (0.03f, 0.03f, 0.90f);
		glColor4f  (1.0f,1.0f,0.0f, 0.0f);
		glVertex3f (-0.03f, -0.03f, 0.90f);
	glEnd ();

	glEnable (GL_TEXTURE_2D);
	glDisable (GL_BLEND); 
}

void show_lines ()
{
	static double linerot = 0.0f;
	
	effect_intro();

	linerot += 1.0f;
	glRotated (linerot, 0.0f, 1.0f, 0.5f);
	glColor4f (1.0f,0.8f,0.0f, 1.0f);

	glBegin (GL_LINES);
		glVertex3f (-1.0f, 1.0f, 0.0f);
		glVertex3f (1.0f, -1.0f, 0.0f);
	glEnd();

	if (linerot >= 360.0f)
		linerot = 0.0f;
}

void draw_landscape (GLuint in_texture)
{
	uint16_t i,k;
	static uint8_t emdone = 0, initdone = 0;
	static GLuint texnum;
	float em[1025][1025];
	float h;

	if (!initdone)
	{
		initdone = 1;
	}

	//glDisable (GL_TEXTURE_2D);
	glTranslatef (-12.6f, -4.0f, -12.6f);
	glBindTexture (GL_TEXTURE_2D, 1);
	glBegin (GL_QUADS);
	for (i=0;i<129;i++)
	{
		for (k=0;k<129;k++)
		{
			if (!emdone && (i == 0 || k == 0))
			{
				em[i][k] = (float) ((rand() % 4096) * 0.00003f);
			} else if (!emdone)
			{
				if (rand() % 2)
					em[i][k] = ((em[i-1][k] + em[i][k-1]) / 2) + (float) ((rand() % 4096) * 0.00003f);
				else
					em[i][k] = ((em[i-1][k] + em[i][k-1]) / 2) - (float) ((rand() % 4096) * 0.00003f);
			}
			glColor4f (0.0f, 0.0f, 0.8f + em[i][k], 1.0f);

			if (k == 0 || i == 0) continue;

			h = em[i-1][k-1];
			glTexCoord3f(0.0f, 0.0f, h);
			glVertex3f( 0.2f * i,       h, 0.2f * k);
			
			h = em[i][k-1];
			glTexCoord3f(1.0f, 0.0f, h);
			glVertex3f( 0.2f * (i + 1), h,   0.2f * k);
			
			h = em[i][k];
			glTexCoord3f(1.0f, 1.0f, h);
			glVertex3f( 0.2f * (i + 1), h,     0.2f * (k+1));

			h = em[i-1][k];
			glTexCoord3f(0.0f, 1.0f, h);
			glVertex3f( 0.2f * i,       em[i-1][k],   0.2f * (k+1));
		}
	}
	glEnd();
	//glEnable (GL_TEXTURE_2D);
	glTranslatef (12.6f, 4.0f, 12.6f);
	emdone = 1;
}


void show_repeatexample()
{
	
	glBindTexture (GL_TEXTURE_2D, 1);
	switch (demostate)
	{
		case 0:
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		break;

		case 1:
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		break;

		default:
			demostate = 0;
		break;
	}
	
	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f (-2.0f,  2.0f, 0.0f);

		glTexCoord2f(2.0f, 0.0f);
		glVertex3f ( 2.0f,  2.0f, 0.0f);
		
		glTexCoord2f(2.0f, 2.0f);
		glVertex3f ( 2.0f, -2.0f, 0.0f);
		
		glTexCoord2f(0.0f, 2.0f);
		glVertex3f (-2.0f, -2.0f, 0.0f);
	glEnd();

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
}
