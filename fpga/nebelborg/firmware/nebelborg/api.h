#ifndef API_H
#define API_H

#include <stdio.h>
#include "config.h"

#include "spike_hw.h"

#define R 0
#define G 1
#define B 2


typedef struct {
	unsigned char x, y;
} pixel; 

typedef struct {
	uint8_t r, g, b;
} color;


extern color black, white, red, green, blue;

//Color order is GBR
extern uint8_t imag[MAX_Y][MAX_X][COLOR_BYTES];


void clearScreen(color c);
void clearImage(color c);

void setPixel(pixel pos, color c);
void initPwm();

void swap();

/*

unsigned char isVoxelSet(voxel pos);
voxel getNextVoxel(voxel pos, direction d);

direction direction_r(direction dir);

void shift(direction dir);

void fade(unsigned char msProStep, unsigned char steps);
void swapAndWait(unsigned char ms);

unsigned char easyRandom();

void drawLine3D(char px1, char py1, char pz1, 
 			    char px2, char py2, char pz2, color value);

*/


#endif // API_H
