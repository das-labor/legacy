#ifndef SANTASLITTLEHELPER_H
#define SANTASLITTLEHELPER_H

#include "config.h"
#include "util.h"
#include "api.h"

#define FADE_BY 10
#define FADE_MIN 32
#define RAND_COL_STEPS 3

typedef struct {
	voxel pp;
	color col;
} pixel3d;

// prototypes:
unsigned char sameColor(color param1, color param2);
unsigned char sameCoord(voxel param1, voxel param2);
unsigned char isVoxelValid(voxel pos);
color fadeBGR(color c);
color fadePixel(color c);
color randomColor();
voxel randomPos();
direction randomDirection();

#endif

