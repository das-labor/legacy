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

uint8_t key_is_pressed(uint8_t key);

typedef struct{
	MessageControlBlock mcb;
	uint8_t key;
}KeyboardMsg_t;

extern KeyboardMsg_t KeyboardMsg;

#define KEY_STOP 0x04
#define KEY_RECORD 0x03
#define KEY_GREEN 0x02
#define KEY_TAPE2 0x01
#define KEY_TAPE 0x00
#define KEY_4TIMES 0x14
#define KEY_REPEAT 0x13
#define KEY_REWIND 0x12
#define KEY_FFWD 0x11
#define KEY_PLAY 0x10
#define KEY_CLOCK 0x24
#define KEY_1TON 0x23
#define KEY_WAVESWITCH 0x22
#define KEY_0TO12 0x20
#define KEY_WAVE 0x54
#define KEY_S 0x52
#define KEY_R 0x51
#define KEY_FLAG 0x62
#define KEY_TAPE 0x61
#define KEY_HORN 0x60


#endif
