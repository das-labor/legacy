#include <avr/io.h>

#include "config.h"

#include "i2c_com.h"
#include "../include/PowerCommander.h"


uint8_t status_input;

static struct t_pin_parameter {
	volatile uint8_t *pin;
	uint8_t bit;
} pin_matrix[] = {
	{ (&(PINA)) , HAUPTSCHALTER},
	{ (&(PINB)) , TASTER_LOUNGE},
	{ (&(PIND)) , TASTER_VORTRAG},
	{ (&(PINC)) , POWER_OK},
	{ (&(PIND)) , RCD_SERVER},
	{ (&(PIND)) , RCD_POWER},
	{ (&(PINA)) , RCD_LICHT}
};


/*
*  check for changes on monitored inputs
*  on change: call send_stat() and call exec()
*/

static void get_inputs() {
	uint8_t i;
	for (i = 0; i < NUM_INPUTS; i++) {
		if (((*pin_matrix[i].pin) & pin_matrix[i].bit) && (((status_input >> i) & 1) == 0)) {
			status_input |= (1 << i);
			
		}
		if (!((*pin_matrix[i].pin) & pin_matrix[i].bit) && ((status_input >> i) & 1)) {
			status_input &= ~(1 << i);
			
		}
	}
}

void switch_handler()
{
	get_inputs();
}
