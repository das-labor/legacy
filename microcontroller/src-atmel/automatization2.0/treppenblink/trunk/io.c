#include <avr/io.h>

#include "config.h"
#include "io.h"
#include "netvar/netvar.h"
#include "netvar/netvar_io.h"
#include "animationen.h"
#include "can_handler.h"

static struct t_switch_parameter {
	volatile uint8_t *port;
	uint8_t pin;
} out_matrix[] = {
	{ O_OUT00_PORT , O_OUT00_BIT},
	{ O_OUT01_PORT , O_OUT01_BIT},
	{ O_OUT02_PORT , O_OUT02_BIT}
};


// the inverted parameter is used to normalize all inputs
// before they are stored in the state parameter and passed to the changed event.
// The normalized values are:
// Taster Licht Vortragsraum 1 = pressed, 0 = released
// Taster Treppenblink       1 = on, 0 = off
// Taster 3                  1 = on, 0 = off
// Taster 4                  1 = on, 0 = off

static struct t_pin_parameter {
	uint8_t state;
	uint8_t inverted;
	volatile uint8_t *pin;
	uint8_t bit;
} pin_matrix[] = {
	{ 0, 1, (&(I_PIN_0)), I_BV_0},
	{ 0, 1, (&(I_PIN_1)), I_BV_1}
};

#ifdef NO_NETVAR
#ifndef NEW_PROTO
	static void keypress(void);
#endif
#endif

void io_init(void)
{
	// Ausgänge für LED Stripe
	RGB_STRING_DDR |= RGB_STRING_DATA | RGB_STRING_CLK;
	RGB_STRING_PORT |= RGB_STRING_DATA | RGB_STRING_CLK;

	// Ausgänge für RGB LED
	RGB_LED_DDR |= RGBLED_R | RGBLED_G | RGBLED_B;

	// Eingänge für Taster
	DDRB &= ~(I_BV_0 | I_BV_1);

	// pullups aktivieren
	PORTB |= (I_BV_0 | I_BV_1);
}

static void output_set(uint8_t output, uint8_t value)
{
	if (value)
		(*out_matrix[output].port) |= out_matrix[output].pin;
	else
		(*out_matrix[output].port) &= ~(out_matrix[output].pin);
}

void set_leds(uint8_t val)
{
	for (uint8_t i = 0; i < 6; i++)
		output_set(i, (val >> i) & 0x01);
}

#ifndef NO_NETVAR
#define F_LED 0

static void lamp_out(void *num, uint8_t val) {
	uint8_t i = (uint8_t) (uint16_t) num;
	switch (i) {
		case F_LED:
			if (val) {
				set_leds(1);
			} else
				set_leds(2);
			break;
	}
}

static netvar_desc *out_netvars[NUM_INPUTS];

void switch_netvars_init(void) {
	out_netvars[0] = netvar_register(0x0100, 0x2f, 1); // Taster Vortragsraum Licht
//	out_netvars[1] = netvar_register(0x000A, 0x00, 1); // Taster Treppenblink
}

#define NV_IDX_LAMP_CONTROLLER_VORTRAG 0x0100

void lamp_out_init(void) {
	new_netvar_output_1(NV_IDX_LAMP_CONTROLLER_VORTRAG, 0x3f, lamp_out, (void *) F_LED);
}
#endif
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
		can_send_ani_status();
	}
#ifdef NEW_PROTO
	can_send_input_status(num, val);
#endif
#endif
}

void switch_handler(void) {
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
#ifndef NEW_PROTO
	keypress();
#endif
#endif
}


#ifdef NO_NETVAR

#ifndef NEW_PROTO

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

void keypress(void) {
	static uint8_t counter;
	static uint8_t clicked = 0;
	static uint8_t held    = 0;
	static uint8_t last_held;

	clicked = 0;
	held = 0;
	if (pin_matrix[0].state)
	{
		counter++;
		if (counter > HOLD_THRESHOLD)
		{
			held = 1;
			counter = HOLD_THRESHOLD;
		}
	}
	else
	{
		if (counter > CLICK_THRESHOLD)
		{
			if (counter < HOLD_THRESHOLD)
			{
				clicked = 1;
			}
		}
		counter = 0;
	}
	if (clicked == 1)
	{
		can_message *msg = can_buffer_get();
		msg->data[0] = C_VIRT;
		msg->data[1] = VIRT_VORTRAG;
		msg->data[2] = F_SW_TOGGLE;
		msg->addr_src = myaddr;
		msg->addr_dst = 0x02;
		msg->port_dst = 1;
		msg->port_src = LIGHTCANPORT;
		msg->dlc = 4;
		can_transmit(msg);	/* send packet */
	}
	if (held)
	{
		can_message *msg = can_buffer_get();
		msg->data[0] = C_VIRT;
		msg->data[1] = VIRT_VORTRAG_PWM;
		msg->data[2] = F_PWM_MOD;
		msg->addr_src = myaddr;
		msg->addr_dst = 0x02;
		msg->port_dst = 1;
		msg->port_src = LIGHTCANPORT;
		msg->dlc = 4;
		can_transmit(msg);	/* send packet */
	}
	else if (last_held)
	{
		can_message *msg = can_buffer_get();
		msg->data[0] = C_VIRT;
		msg->data[1] = VIRT_VORTRAG_PWM;
		msg->data[2] = F_PWM_DIR;
		msg->addr_src = myaddr;
		msg->addr_dst = 0x02;
		msg->port_dst = 1;
		msg->port_src = LIGHTCANPORT;
		msg->dlc = 4;
		can_transmit(msg);	/* send packet */
	}
	last_held = held;
}
#endif
#endif

