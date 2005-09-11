#ifndef BORG_HW_H
#define BORG_HW_H

#include "config.h"

#define LINEBYTES (((NUM_COLS-1)/8)+1)


extern unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

void watchdog_enable();
void borg_hw_init();
void timer0_off();

#endif
