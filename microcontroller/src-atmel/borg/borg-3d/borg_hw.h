#ifndef BORG_HW_H
#define BORG_HW_H

#include "config.h"

//#define LINEBYTES (((NUM_COLS-1)/8)+1)

extern unsigned char pixmap[NUM_LEVELS][NUM_PLANES][PLANEBYTES];

void timer0_off();

void borg_hw_init();

#endif
