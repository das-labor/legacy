#include <avr/io.h>

#include "config.h"
#include "hauptschalter.h"
#include "netvar/netvar.h"


// the inverted parameter is used to normalize all inputs
// before they are stored in the state parameter and passed to the changed event.
// The normalized values are:
// Hauptschalter 1 = on, 0 = off
// Taster        1 = pressed, 0 = released
// Power ok      1 = ok
// RCDs          1 = RCD on, 0 = RCD tripped

static struct t_pin_parameter {
	uint8_t state;
	int8_t debounce_count;
	uint8_t inverted;
	volatile uint8_t *pin;
	uint8_t bit;
} pin_matrix[] = {
	{ 0, 0, 0, (&(PIN_HAUPTSCHALTER)),  BV_HAUPTSCHALTER},
	{ 0, 0, 1, (&(PIN_TASTER_LOUNGE)),  BV_TASTER_LOUNGE},
	{ 0, 0, 1, (&(PIN_TASTER_VORTRAG)), BV_TASTER_VORTRAG},
	{ 0, 0, 0, (&(PIN_POWER_OK)),       BV_POWER_OK},
	{ 0, 0, 1, (&(PIN_RCD_SERVER)),     BV_RCD_SERVER},
	{ 0, 0, 1, (&(PIN_RCD_POWER)),      BV_RCD_POWER},
	{ 0, 0, 1, (&(PIN_RCD_LICHT)),      BV_RCD_LICHT}
};

// pin matrix indexes
#define MX_HAUPTSCHALTER  0
#define MX_TASTER_LOUNGE  1
#define MX_TASTER_VORTRAG 2
#define MX_POWER_OK       3
#define MX_RCD_SERVER     4
#define MX_RCD_POWER      5
#define MX_RCD_LICHT      6



static netvar_desc *out_netvars[NUM_INPUTS];

void switch_netvars_init() {
	out_netvars[0] = netvar_register(0x0000, 0x00, 1);//hauptschalter
	out_netvars[1] = netvar_register(0x0101, 0x2f, 1);//taster lounge
	out_netvars[2] = netvar_register(0x0100, 0x2f, 1);//taster vortrag
	out_netvars[3] = netvar_register(0x0001, 0x00, 1);//power 24V ok
	out_netvars[4] = netvar_register(0x0002, 0x00, 1);//rcd server
	out_netvars[5] = netvar_register(0x0003, 0x00, 1);//rcd power main
	out_netvars[6] = netvar_register(0x0004, 0x00, 1);//rcd licht
}

static void input_changed_event (uint8_t num, uint8_t val) {
	if (num == MX_HAUPTSCHALTER) {
		hauptschalter_update(val);
	}
	netvar_write(out_netvars[num], &val);
}

#define DEBOUNCE_CYCLES 2

/*
*  debounce monitored inputs, invert them if wanted, and check for changes
*  on change: call input_changed_event
*/

void get_inputs() {
	uint8_t i;
	for (i = 0; i < NUM_INPUTS; i++) {
		if ((*pin_matrix[i].pin) & pin_matrix[i].bit) {
			pin_matrix[i].debounce_count++;
			if (pin_matrix[i].debounce_count > DEBOUNCE_CYCLES) {
				pin_matrix[i].debounce_count = DEBOUNCE_CYCLES;
				if (pin_matrix[i].state == pin_matrix[i].inverted) {
					pin_matrix[i].state = pin_matrix[i].inverted ^ 1;
					input_changed_event(i, pin_matrix[i].state);
				}
			}
		} else {
			pin_matrix[i].debounce_count--;
			if (pin_matrix[i].debounce_count < 0) {
				pin_matrix[i].debounce_count = 0;
				if (pin_matrix[i].state == (pin_matrix[i].inverted ^ 1)) {
					pin_matrix[i].state = pin_matrix[i].inverted;
					input_changed_event(i, pin_matrix[i].state);
				}
			}
		}
	}
}

