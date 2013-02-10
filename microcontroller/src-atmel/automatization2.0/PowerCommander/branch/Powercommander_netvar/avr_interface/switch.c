#include <avr/io.h>

#include "config.h"
#include "hauptschalter.h"
#include "netvar/netvar.h"

uint8_t status_input;

static struct t_pin_parameter {
	volatile uint8_t *pin;
	uint8_t bit;
} pin_matrix[] = {
	{ (&(PINA)), HAUPTSCHALTER},
	{ (&(PINB)), TASTER_LOUNGE},
	{ (&(PIND)), TASTER_VORTRAG},
	{ (&(PINC)), POWER_OK},
	{ (&(PIND)), RCD_SERVER},
	{ (&(PIND)), RCD_POWER},
	{ (&(PINA)), RCD_LICHT}
};


static netvar_desc *out_netvars[NUM_INPUTS];

/*
*  check for changes on monitored inputs
*  on change: call send_stat() and call exec()
*/

static void get_inputs() {
	uint8_t i;
	for (i = 0; i < NUM_INPUTS; i++) {
		if (((*pin_matrix[i].pin) & pin_matrix[i].bit) && (((status_input >> i) & 1) == 0)) {
			status_input |= (1 << i);
			uint8_t tmp = 0xff;
			netvar_write(out_netvars[i], &tmp);
			if (i == 0) {
				hauptschalter_update(1);
			}
		}
		if (!((*pin_matrix[i].pin) & pin_matrix[i].bit) && ((status_input >> i) & 1)) {
			status_input &= ~(1 << i);
			uint8_t tmp = 0x00;
			netvar_write(out_netvars[i], &tmp);
			if (i == 0) {
				hauptschalter_update(0);
			}
		}
	}
}

void switch_netvars_init() {
	out_netvars[0] = netvar_register(0x0000, 0x00, 1);//hauptschalter
	out_netvars[1] = netvar_register(0x0101, 0x2f, 1);//taster lounge
	out_netvars[2] = netvar_register(0x0100, 0x2f, 1);//taster vortrag
	out_netvars[3] = netvar_register(0x0001, 0x00, 1);//power 24V ok
	out_netvars[4] = netvar_register(0x0002, 0x00, 1);//rcd server
	out_netvars[5] = netvar_register(0x0003, 0x00, 1);//rcd power main
	out_netvars[6] = netvar_register(0x0004, 0x00, 1);//rcd licht
}

