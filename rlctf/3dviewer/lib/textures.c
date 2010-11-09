#include <stdint.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#include "textures.h"

static GLuint *texdb = NULL;
static uint32_t num_textures = 0;


GLuint loadtexture (char *in_name)
{
	int t;
	GLuint *new_name = malloc (sizeof(GLuint));

	SDL_Surface *texsurf;

	t = (int) SDL_LoadBMP (in_name);

	if (t < 0)
	{
		return t;
	}
	
	texsurf = (SDL_Surface *) t;

	glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
	glGenTextures (1, new_name);            /* request a new texture id */
	num_textures++;
	texdb = realloc (texdb, num_textures);  /* make room for another entry */
	texdb[num_textures-1] = *new_name;      /* copy the new id to our db */
	glBindTexture (GL_TEXTURE_2D, *new_name);

	glTexImage2D (GL_TEXTURE_2D, 0, 3, texsurf->w,
		texsurf->h, 0, GL_BGR,
		GL_UNSIGNED_BYTE, texsurf->pixels);
	
	gluBuild2DMipmaps (GL_TEXTURE_2D, 3, texsurf->w,
		texsurf->h, GL_BGR,
		GL_UNSIGNED_BYTE, texsurf->pixels);
	
	return *new_name;
}

void enable_autotexturing()
{
	const double len = 1.0f;
	const double v1[4] = {len, len, 0.0, 0.0};
	const double v2[4] = {0.0, len, len, 0.0};

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);

	glTexGendv(GL_S, GL_EYE_PLANE, v1);
	glTexGendv(GL_T, GL_EYE_PLANE, v2);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_PLANE);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_PLANE);
	
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
}

void disable_autotexturing()
{
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}
