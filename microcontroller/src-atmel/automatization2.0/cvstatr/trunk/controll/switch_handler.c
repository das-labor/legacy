/* -*- Mode: C; tab-width: 2 -*- */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "can/can.h"
#include "can_handler.h"
#include "can/lap.h"

#define KLINGEL	(PD6)
#define STANDBY	(PD5)
#define TUER	(PD7)
#define SCHLOSS (PD0) //XXX

#define SIGNAL_PIN	(&(PIND))


union {
	struct {
		uint8_t klingel:1;
		uint8_t standby:1;
		uint8_t tuerkontakt:1;
		uint8_t schloss:1;
	} switches;
	uint8_t stat_sw;
} stat_switches;

static struct t_pin_parameter {
	volatile uint8_t *pin;
	uint8_t bit;
} pin_matrix[] = {
	{ SIGNAL_PIN , KLINGEL},
	{ SIGNAL_PIN , STANDBY},
	{ SIGNAL_PIN , TUER},
	{ SIGNAL_PIN , SCHLOSS}
};

void send_stat() {
	static can_message msg = {0x04, 0x00, 0x00, 0x01, 1, {0}};
	msg.data[0] = stat_switches.stat_sw;
	// send changed bit oder beim ziel mit tab vergleichen
	//msg.addr_src = myaddr;
	can_transmit(&msg);
}

void get_switches() {
	uint8_t i;
	
	for (i = 0; i < 3; i++) {
		if (((*pin_matrix[i].pin) & _BV(pin_matrix[i].bit)) && (((stat_switches.stat_sw >> i) & 1) == 0)) {
			stat_switches.stat_sw |= (1 << i);
			send_stat();
		}
		
		if (!((*pin_matrix[i].pin) & _BV(pin_matrix[i].bit)) && ((stat_switches.stat_sw >> i) & 1)) {
			stat_switches.stat_sw &= ~(1 << i);
			send_stat();
		}
	}
}

void switch_handler()
{
	get_switches();
}

