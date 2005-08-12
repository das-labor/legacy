#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define BORGSIZE 8
#define BORGLEDS BORGSIZE*BORGSIZE*BORGSIZE

char data[BORGLEDS];    // 0 for off, 1 for dark, 2 for middle, 3 for bright

typedef struct {
	unsigned char x;
	unsigned char y;
	unsigned char z;
} pixel3d;

typedef enum {right, left, forward, back, up, down} direction;

void clear_screen(unsigned char value);
void setpixel3d(pixel3d p, unsigned char value);
void clearpixel3d(pixel3d p);
unsigned char get_pixel3d(pixel3d p);
unsigned char get_next_pixel3d(pixel3d p, direction dir);
pixel3d next_pixel3d(pixel3d pix, direction dir);
direction direction_r(direction dir);
void wait(unsigned int ms);
