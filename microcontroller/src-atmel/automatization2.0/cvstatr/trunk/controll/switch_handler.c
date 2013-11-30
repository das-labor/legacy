#include <avr/io.h>

#include "switch_handler.h"
#include "can/can.h"
#include "can/lap.h"
#include "can_handler.h"
#include "config.h"

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

static uint8_t stat_sw;

static struct t_pin_parameter {
	volatile uint8_t *pin;
	uint8_t bit;
} pin_matrix[] = {
	{SIGNAL_PIN, KLINGEL},
	{SIGNAL_PIN, STANDBY},
	{SIGNAL_PIN, TUER}
};

static void get_switches(void) {
	uint8_t i, msk = 0x01;

	for (i = 0; i < 3; i++) {
		if (((*pin_matrix[i].pin) & pin_matrix[i].bit) && (stat_sw & msk) == 0) {
			stat_sw |= msk;
			can_send_stat(stat_sw, i);
		}
		else if ( !((*pin_matrix[i].pin) & pin_matrix[i].bit) && (stat_sw & msk)) {
			stat_sw &= ~msk;
			can_send_stat(stat_sw, i);
		}
		msk <<= 1;
	}
}

void switch_handler()
{
	get_switches();
}

