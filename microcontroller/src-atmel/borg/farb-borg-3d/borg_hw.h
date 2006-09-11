
#ifndef BORG_HW_H
#define BORG_HW_H

#include "config.h"

//#define LINEBYTES (((NUM_COLS-1)/8)+1)

#define CTRLPORT  PORTA
#define CTRLDDR   DDRA

#define DATAPORT  PORTC
#define DATADDR   DDRC

#define PIN_OE_DATA    	 PA0
#define PIN_SHIFT_DATA   PA1
#define PIN_CLEAR_DATA 	 PA2
#define PIN_CLK_DATA 	 PA3
#define PIN_SHIFT_PLANE  PA4
#define PIN_CLK_PLANE    PA5
#define PIN_CLEAR_PLANE  PA6

extern unsigned char pixmap[NUM_LEVELS][NUM_PLANES][PLANEBYTES];

void timer0_off();

void borg_hw_init();

#endif
