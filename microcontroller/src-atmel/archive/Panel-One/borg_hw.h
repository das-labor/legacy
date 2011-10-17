#ifndef BORG_HW_H
#define BORG_HW_H

#include "config.h"
#include <stdint.h>

//Linebytes gibt die Zahl der Bytes pro Zeile in der
//Pixmap an, also Spaltenzahl/8 aufgerundet
#define LINEBYTES (((NUM_COLS-1)/8)+1)


extern unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

extern volatile uint8_t keys[8];

void watchdog_enable();
void borg_hw_init();
void timer0_off();

#endif
