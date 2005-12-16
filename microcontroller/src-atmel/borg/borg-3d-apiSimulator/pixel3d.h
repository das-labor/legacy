#include <stdio.h>
#include "util.h"
#include "config.h"

extern unsigned char pixmap[NUM_LEVELS][NUM_PLANES][PLANEBYTES];

typedef struct {
	signed char x;
	signed char y;
	signed char z;
} pixel3d;

typedef enum {right, left, forward, back, up, down} direction;

void clear_screen(unsigned char value);
void setpixel3d(pixel3d p, unsigned char value);
#define clearpixel3d(p) setpixel3d(p, 0);
unsigned char get_pixel3d(pixel3d p);
unsigned char get_next_pixel3d(pixel3d p, direction dir);
pixel3d next_pixel3d(pixel3d pix, direction dir);
direction direction_r(direction dir);
void shift3d(direction dir);
void set_plane(direction dir, unsigned char num, unsigned char color);

#define Cos(a) Sin((a)+16)
char Sin(unsigned char a);
void rotate(unsigned char a, unsigned char b, unsigned char c, pixel3d* points, 
			pixel3d* resPoints, int numPoint, pixel3d rotP);
