#include <stdio.h>
#include "util.h"
#include "config.h"

extern unsigned char pixmap[NUM_LEVELS][NUM_PLANES][PLANEBYTES];

typedef struct {
	char x;
	char y;
	char z;
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
