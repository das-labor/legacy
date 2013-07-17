#include <avr/io.h>

#include "config.h"
#include "io.h"
#include "netvar/netvar.h"
#include "netvar/netvar_io.h"
#include "animationen.h"

// the inverted parameter is used to normalize all inputs
// before they are stored in the state parameter and passed to the changed event.
// The normalized values are:
// Taster            1 = pressed, 0 = released
// Türkontakt        1 = on, 0 = off

static struct t_pin_parameter {
	uint8_t state;
	uint8_t inverted;
	volatile uint8_t *pin;
	uint8_t bit;
} pin_matrix[] = {
	{ 0, 1, (&(I_PIN_0)), I_BV_0},
	{ 0, 1, (&(I_PIN_1)), I_BV_1}
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
//	out_netvars[1] = netvar_register(0x000A, 0x00, 1); // Taster Treppenblink
}

#define NV_IDX_LAMP_CONTROLLER_VORTRAG 0x0100

void lamp_out_init() {
	new_netvar_output_1(NV_IDX_LAMP_CONTROLLER_VORTRAG, 0x3f, lamp_out, (void *) F_LED);
}

static void input_changed_event(uint8_t num, uint8_t val) {

#ifndef NO_NETVAR
	netvar_write(out_netvars[num], &val);
#else
// Taster Treppenblink
	if (num && val) {
		if (animation < 3)
		{
			animation++;
		}
		else
		{
			animation = 0;
		}
	}
#endif
}

void switch_handler() {
	uint8_t i;
	for (i = 0; i < NUM_INPUTS; i++) {
		if ((*pin_matrix[i].pin) & pin_matrix[i].bit) {
			if (pin_matrix[i].state == pin_matrix[i].inverted) {
				pin_matrix[i].state = pin_matrix[i].inverted ^ 1;
				input_changed_event(i, pin_matrix[i].state);
			}
		} else {
			if (pin_matrix[i].state == (pin_matrix[i].inverted ^ 1)) {
				pin_matrix[i].state = pin_matrix[i].inverted;
				input_changed_event(i, pin_matrix[i].state);
			}
		}
	}
#ifdef NO_NETVAR
	keypress();
#endif
}


#ifdef NO_NETVAR
#define HOLD_THRESHOLD 18
#define CLICK_THRESHOLD 0

#define C_VIRT (0x02)
#define F_SW_TOGGLE   (0x03)
#define F_PWM_MOD     (0x02)
#define F_PWM_DIR     (0x03)
#define O_VIRT01 (0x01)
#define O_VIRT02 (0x02)
#define VIRT_VORTRAG (O_VIRT01)
#define VIRT_VORTRAG_PWM (O_VIRT02)

#define LIGHTCANPORT 0x04

#include "can/can.h"

void keypress() {
	static uint8_t counter_0;
	static uint8_t clicked_0 = 0;
	static uint8_t held_0    = 0;
	static uint8_t last_held_0;

	clicked_0 = 0;
	held_0 = 0;
	if (pin_matrix[0].state)
	{
		counter_0++;
		if (counter_0 > HOLD_THRESHOLD)
		{
			held_0 = 1;
			counter_0 = HOLD_THRESHOLD;
		}
	}
	else
	{
		if (counter_0 > CLICK_THRESHOLD)
		{
			if (counter_0 < HOLD_THRESHOLD)
			{
				clicked_0 = 1;
			}
		}
		counter_0 = 0;
	}
	if (clicked_0 == 1)
	{
		if (PORT_LED & R_LED)
		{
			lamp_out(0, 0);
		}
		else
		{
			lamp_out(0, 1);
		}
		can_message *msg = can_buffer_get();
		msg->data[0] = C_VIRT;
		msg->data[1] = VIRT_VORTRAG;
		msg->data[2] = F_SW_TOGGLE;
		msg->addr_src = 0x23;
		msg->addr_dst = 0x02;
		msg->port_dst = 1;
		msg->port_src = LIGHTCANPORT;
		msg->dlc = 4;
		can_transmit(msg);	/* send packet */
	}
	if (held_0)
	{
		can_message *msg = can_buffer_get();
		msg->data[0] = C_VIRT;
		msg->data[1] = VIRT_VORTRAG_PWM;
		msg->data[2] = F_PWM_MOD;
		msg->addr_src = 0x23;
		msg->addr_dst = 0x02;
		msg->port_dst = 1;
		msg->port_src = LIGHTCANPORT;
		msg->dlc = 4;
		can_transmit(msg);	/* send packet */
	}
	else if (last_held_0)
	{
		can_message *msg = can_buffer_get();
		msg->data[0] = C_VIRT;
		msg->data[1] = VIRT_VORTRAG_PWM;
		msg->data[2] = F_PWM_DIR;
		msg->addr_src = 0x23;
		msg->addr_dst = 0x02;
		msg->port_dst = 1;
		msg->port_src = LIGHTCANPORT;
		msg->dlc = 4;
		can_transmit(msg);	/* send packet */
	}
	last_held_0 = held_0;
}
#endif

