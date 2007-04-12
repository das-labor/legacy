#ifndef BORG_HW_H
#define BORG_HW_H

#include "config.h"
#include <stdint.h>
#include <avrx.h>

//Linebytes gibt die Zahl der Bytes pro Zeile in der
//Pixmap an, also Spaltenzahl/8 aufgerundet
#define LINEBYTES (((NUM_COLS-1)/8)+1)

extern uint8_t pixmap[NUM_ROWS][LINEBYTES];

extern volatile uint8_t keys[8];

void watchdog_enable();
void borg_hw_init();
void timer0_off();

typedef struct{
	MessageControlBlock mcb;
	uint8_t key;
}KeyboardMsg_t;

extern KeyboardMsg_t KeyboardMsg;

#endif
