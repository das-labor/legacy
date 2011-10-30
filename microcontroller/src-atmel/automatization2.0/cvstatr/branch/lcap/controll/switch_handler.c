/* -*- Mode: C; tab-width: 2 -*- */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"

#define KLINGEL	_BV(PD6)
#define STANDBY	_BV(PD5)
#define TUER	_BV(PD7)
#define SCHLOSS _BV(PD0) //XXX

#define SIGNAL_PIN	(&(PIND))

/*
union {

	struct {
		uint8_t klingel:1;
		uint8_t standby:1;
		uint8_t tuerkontakt:1;
		uint8_t schloss:1;
	} switches;
} stat_switches;

*/

	uint8_t stat_sw;

static struct t_pin_parameter {
	volatile uint8_t *pin;
	uint8_t bit;
} pin_matrix[] = {
	{ SIGNAL_PIN , KLINGEL},
	{ SIGNAL_PIN , STANDBY},
	{ SIGNAL_PIN , TUER}
};

void send_stat(uint8_t bla) {
	static can_message msg = {0x04, 0x00, 0x00, 0x01, 2, {0}};
	msg.data[0] = stat_sw;
	msg.data[1] = bla;
	
	// send changed bit oder beim ziel mit tab vergleichen
	//msg.addr_src = myaddr;
	can_transmit(&msg);
}

void get_switches() {
	uint8_t i, msk = 0x01;
	
	for (i = 0; i < 3; i++) {
		if (((*pin_matrix[i].pin) & pin_matrix[i].bit) && (stat_sw & msk) == 0) {
			stat_sw |= msk;
			send_stat(i);
		}
		if ( !((*pin_matrix[i].pin) & pin_matrix[i].bit) && (stat_sw & msk)) {
			stat_sw &= ~msk;
			send_stat(i);
		}
		msk <<= 1;
	}
}

void switch_handler()
{
	get_switches();
}

