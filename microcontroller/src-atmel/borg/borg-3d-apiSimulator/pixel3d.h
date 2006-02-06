#include <stdio.h>
#include "util.h"
#include "config.h"

extern unsigned char pixmap[NUM_LEVELS][NUM_PLANES][PLANEBYTES];

#ifndef PIXEL_C
extern unsigned char shl_table[];
#endif

typedef struct {
	unsigned char x;
	unsigned char y;
	unsigned char z;
//	unsigned char dummy;
} pixel3d;

typedef enum {right, left, forward, back, up, down} direction;

void clear_screen(unsigned char value);
void setpixel3d(pixel3d p, unsigned char value);
 
#define clearpixel3d(p) setpixel3d(p, 0);

unsigned char get_pixel3d(pixel3d p);
unsigned char get_next_pixel3d(pixel3d p, direction dir);

pixel3d next_pixel3d(pixel3d pix, direction dir);

direction direction_r(direction dir);
direction turn_right(direction dir);
direction turn_left(direction dir);
direction turn_up(direction dir);
direction turn_down(direction dir);

unsigned char easyRandom();

void shift3d(direction dir);
void set_plane(direction dir, unsigned char num, unsigned char color);
pixel3d mulMatrixPoint(char *mat, pixel3d *p);

char Sin(unsigned char a);
#define Cos(a) Sin((a)+16)

void scale(char sx, char sy, char sz, pixel3d* points, 
		   pixel3d* resPoints, int numPoint, pixel3d scaleP);
void rotate(char a, char b, char c, pixel3d* points, 
			pixel3d* resPoints, int numPoint, pixel3d rotP);
void drawLine3D(char px1, char py1, char pz1, 
 			    char px2, char py2, char pz2, unsigned char value);
