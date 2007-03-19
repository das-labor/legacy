#ifndef PIXEL_H
#define PIXEL_H

#include "config.h"
#include <stdint.h>

#define LINEBYTES (((NUM_COLS-1)/8)+1)

extern uint8_t shl_table[];

/****************************************************************************
 * Pixel routines
 */
void clear_screen(uint8_t value);
void setpixel(uint8_t x, uint8_t value);
#define clearpixel(p) setpixel(p, 0);

#endif // PIXEL_H
