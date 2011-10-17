#ifndef PIXEL_H
#define PIXEL_H

#include "config.h"
#define LINEBYTES (((NUM_COLS-1)/8)+1)

extern unsigned char shl_table[];
extern unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];


/****************************************************************************
 * Pixel routines
 */
void clear_screen(unsigned char value);
void setpixel(unsigned char x, unsigned char value);
#define clearpixel(p) setpixel(p, 0);

#endif // PIXEL_H
