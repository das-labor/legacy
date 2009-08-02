#ifndef API_H
#define API_H

#include "spike_hw.h"
#include <stdio.h>

typedef   signed short  int16_t;
typedef unsigned short uint16_t;

#define R 0
#define G 1
#define B 2

typedef struct {
	uint8_t x, y, z;
} voxel; 

typedef struct {
	uint32_t r, g, b;
} color; 

typedef enum {
	noDirection = 0,
	up, down, 
	right, left,
	forward, back
} direction;

extern color black, white, red, green, blue;

extern uint32_t imag[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];

void clearScreen(color c);
void clearImage(color c);
void normalize();

void setVoxel(voxel pos, color c);

void setVoxelH_f(int x, int y, int z, float h);

unsigned isqrt(unsigned long val) ;

void setSymetricVoxel(voxel pos, color c);

void setVoxelH(int x, int y, int z, int h);

unsigned char isVoxelSet(voxel pos);
voxel getNextVoxel(voxel pos, direction d);

direction direction_r(direction dir);

color getColor(voxel pos);

void shift(direction dir);

void fade(unsigned int msProStep, unsigned int steps);
void swapAndWait(unsigned int ms);

unsigned char easyRandom();

color HtoRGB(int h31bit);

void drawLine3D(char px1, char py1, char pz1, 
 			    char px2, char py2, char pz2, color value);

// Sinustabelle 
// für plasma
int32_t Sine(int32_t phase);
int32_t Cosi(int32_t phase);
// für 3D
char Sin(unsigned char a);
#define Cos(a) Sin((a)+16)

void rotate(char a, char b, char c, voxel* points, 
			voxel* resPoints, int numPoint, voxel rotP);
void scale(char sx, char sy, char sz, voxel* points, 
			voxel* resPoints, int numPoint, voxel scaleP);

void blur();

#endif // API_H
