#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#include "lib/textures.h"
#include "lib/3ds_draw.h"

static GLfloat xrot = -60.5f, yrot = 0.0f, zrot = 0.4f;
static GLfloat xpos = -0.5f, ypos = 0.0f, zpos = -18.0f;
static float lz, lx, ly;

void set_yline (float in_x, float in_y, float in_z)
{
	lx = in_x;
	lz = in_z;
	ly = in_y;
}

void draw_yline (float in_x, float in_y, float in_z)
{
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
	glBegin (GL_LINES);
		glVertex3f (-in_z, -in_x, 10.0f);
		glVertex3f (-in_z, -in_x, -10.0f);
	glEnd();

	glColor4f (0.0f, 1.0f, 0.0f, 0.5f);
	glBegin (GL_QUADS);
		glVertex3f (-in_z + 0.1f, -in_x, in_y);
		glVertex3f (-in_z + 0.1f, -in_x + 0.1f, in_y);
		glVertex3f (-in_z - 0.1f, -in_x +0.1f, in_y);
		glVertex3f (-in_z - 0.1f, -in_x, in_y);
	glEnd();
}

void scan_ani ()
{
	static uint8_t dir = 0;
	static float x = 0.0f;
	float y = -3.0f;
	glTranslatef (0.0f, -0.6f, 0.0f);
	glBlendFunc (GL_ONE, GL_ZERO);
	glBegin (GL_LINES);
		for (y = -2.0f; y <= 2.0f; y += 0.2f)
		{
			glVertex3f (x, y, -1.0f);
			glVertex3f (x, y,  3.0f);
			
			glVertex3f (x, -2.0f, y+1.0f);
			glVertex3f (x, 2.0f,  y+1.0f);
		}
	glEnd();

	glBlendFunc (GL_ONE, GL_SRC_ALPHA);
	glBegin (GL_QUADS);
		glVertex3f (x, 2.0f, -1.0f);
		glVertex3f (x, 2.0f, 3.0f);
		glVertex3f (x, -2.0f, 3.0f);
		glVertex3f (x, -2.0f, -1.0f);
	glEnd();
	glTranslatef (0.0f, 0.6f, 0.0f);
	if (dir == 0)
	{
		x += 0.4f;
		if (x > 8.0f)
			dir = 1;
	} else
	{
		x -= 0.4f;
		if (x < -8.0f)
			dir = 0;
	}
}

void set_rot (float in_z)
{
	zrot = in_z;
}

void set_zoom_relative (float in_zval)
{
	zpos += in_zval;
}

void draw_lab ()
{
	float fcol[3] = {0.0f, 0.0f, 0.0f};

	glBlendFunc (GL_ONE, GL_ZERO);
	// glEnable (GL_CULL_FACE);
	glLoadIdentity();

	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf (GL_FOG_DENSITY, 0.75f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogfv (GL_FOG_COLOR, fcol);
	glFogf (GL_FOG_START, 5.0f);
	glFogf (GL_FOG_END, 20.0f);
//	glEnable (GL_FOG);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glTranslatef (xpos, ypos, zpos);
	glRotatef (xrot, 1.0f, 0.0f, 0.0f);
	glRotatef (yrot, 0.0f, 1.0f, 0.0f);
	glRotatef (zrot, 0.0f, 0.0f, 1.0f);
	
	glColor4f (1.0f, 1.0f, 1.0f, 0.8f);
	glBlendFunc (GL_ONE, GL_ZERO);


	glEnable (GL_TEXTURE_2D);
	enable_autotexturing();


	tds_drawmodel(0);


	glColor4f (1.0f, 0.0f, 0.0f, 1.0f);
	glDisable (GL_TEXTURE_2D);
	scan_ani();
	draw_yline (lx, ly, lz);

	glTranslatef (-xpos, -ypos, -zpos);
}
