#include <avr/io.h>

#include "config.h"
#include "io.h"
#include "netvar/netvar.h"
#include "netvar/netvar_io.h"

// the inverted parameter is used to normalize all inputs
// before they are stored in the state parameter and passed to the changed event.
// The normalized values are:
// Taster            1 = pressed, 0 = released
// Türkontakt        1 = on, 0 = off

static struct t_pin_parameter {
	uint8_t state;
	int8_t debounce_count;
	uint8_t inverted;
	volatile uint8_t *pin;
	uint8_t bit;
} pin_matrix[] = {
	{ 0, 0, 1, (&(I_PIN_0)), I_BV_0},
	{ 0, 0, 0, (&(I_PIN_1)), I_BV_1}
};

void init_io()
{
	// Ausgänge für LED Stripe
	RGB_DDR |= RGB_DATA | RGB_CLK;
	RGB_PORT |= RGB_DATA | RGB_CLK;

	// Ausgänge für RGB LED
	DDR_LED |= R_LED | G_LED | B_LED;

	// Eingänge für Taster
	DDRB &= ~(I_BV_0 | I_BV_1);

	// pullups aktivieren
	PORTB |= (I_BV_0 | I_BV_1);
}

#define F_LED 0

static void lamp_out(void *num, uint8_t val) {
	uint8_t i = (uint8_t) (uint16_t) num;
	switch (i) {
		case F_LED:
			if (val) {
				PORT_LED |= R_LED;
				PORT_LED &= ~(G_LED);
			} else {
				PORT_LED |= G_LED;
				PORT_LED &= ~(R_LED);
			}
			break;
	}
}

static netvar_desc *out_netvars[NUM_INPUTS];

void switch_netvars_init() {
	out_netvars[0] = netvar_register(0x0100, 0x2f, 1); // Taster Vortragsraum Licht
//	out_netvars[1] = netvar_register(0x000A, 0x00, 1); // Bewegung
}

#define NV_IDX_LAMP_CONTROLLER_VORTRAG 0x0100

void lamp_out_init() {
	new_netvar_output_1(NV_IDX_LAMP_CONTROLLER_VORTRAG, 0x3f, lamp_out, (void *) F_LED);
}

static void input_changed_event(uint8_t num, uint8_t val) {

	netvar_write(out_netvars[num], &val);
}

#define DEBOUNCE_CYCLES 2

/*
*  debounce monitored inputs, invert them if wanted, and check for changes
*  on change: call input_changed_event
*/

void switch_handler() {
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

