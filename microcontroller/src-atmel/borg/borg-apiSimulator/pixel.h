#ifndef PIXEL_H_
#define PIXEL_H_

#include "util.h"
#include "config.h"

extern unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

#ifndef PIXEL_C
extern unsigned char shl_table[];
#endif

typedef struct{
	unsigned char x;
	unsigned char y;
} pixel;


typedef enum{right,left,up,down} direction;

typedef enum{clear=0, set=1} commands;

typedef struct{
	pixel pos;
	direction dir;
	commands mode;
} cursor;

void clear_screen(unsigned char value);

void setpixel(pixel p, unsigned char value);
#define clearpixel(p) setpixel(p, 0);

unsigned char get_pixel(pixel p);
unsigned char get_next_pixel(pixel p, direction dir);

pixel next_pixel(pixel pix, direction dir);

direction direction_r(direction dir);

void shift_pixmap_l();


void set_cursor(cursor* cur, pixel p);

//this is deprecated
void walk(cursor* cur, unsigned char steps, unsigned int delay);

#endif /* PIXEL_H */