#ifndef BORG_HW_H
#define BORG_HW_H

#include "config.h"
#include <stdint.h>

//Linebytes gibt die Zahl der Bytes pro Zeile in der
//Pixmap an, also Spaltenzahl/8 aufgerundet
#define LINEBYTES (((NUM_COLS-1)/8)+1)


extern unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

extern uint8_t display[NUM_DISPLAYS][NUM_ROWS];

extern volatile uint8_t keys[4];

void watchdog_enable();
void borg_hw_init();
void timer0_off();

void set_led(uint8_t num, uint8_t value);

#endif
