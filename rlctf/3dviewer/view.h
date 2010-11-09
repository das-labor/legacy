#include <stdint.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#pragma once
#define NUM_VIEWPORTS 2

typedef struct vp_s
{
	float pos[3];
	float rot[4];
} vp_t;

void view_init (vp_t* in_vp);
void view_set (vp_t* in_vp);
void view_move (vp_t* in_vp, float in_x, float in_y, float in_z);
