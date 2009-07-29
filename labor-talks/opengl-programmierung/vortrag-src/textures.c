#include <stdint.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#include "textures.h"

GLuint loadtexture (char *in_name)
{
	char slidename[512];
	int t;
	GLuint retval;

	SDL_Surface *texsurf[1];

	t = SDL_LoadBMP (in_name);

	if (t < 0)
		return t;
	
	texsurf[0] = abs(t);

	glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
	glBindTexture (GL_TEXTURE_2D, &retval);

	glTexImage2D (GL_TEXTURE_2D, 0, 3, texsurf[0]->w,
		texsurf[0]->h, 0, GL_BGR,
		GL_UNSIGNED_BYTE, texsurf[0]->pixels);
	
	gluBuild2DMipmaps (GL_TEXTURE_2D, 3, texsurf[0]->w,
		texsurf[0]->h, GL_BGR,
		GL_UNSIGNED_BYTE, texsurf[0]->pixels);
	
	return retval;
}
