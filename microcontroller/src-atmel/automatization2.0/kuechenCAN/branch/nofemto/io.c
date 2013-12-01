#include <avr/io.h>

#include "config.h"
#include "io.h"
#include "netvar/netvar.h"
#include "netvar/netvar_io.h"
#include "can_handler.h"


static struct t_switch_parameter {
	volatile uint8_t *port;
	uint8_t pin;
} out_matrix[] = {
	{ O_OUT00_PORT , O_OUT00_BIT},
	{ O_OUT01_PORT , O_OUT01_BIT},
	{ O_OUT02_PORT , O_OUT02_BIT},
	{ O_OUT03_PORT , O_OUT03_BIT},
	{ O_OUT04_PORT , O_OUT04_BIT},
	{ O_OUT05_PORT , O_OUT05_BIT}
};

// the inverted parameter is used to normalize all inputs
// before they are stored in the state parameter and passed to the changed event.
// The normalized values are:
// Taster            1 = pressed, 0 = released
// Türkontakt        1 = on, 0 = off

static void keypress(void);

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

void io_init()
{
	// ############ Küchenlicht ################
	// RGB LED im Taster
	RGB_LED_DDR |= RGBLED_R | RGBLED_G | RGBLED_B; // Ausgang
	// Taster
	DDRC &= ~_BV(PC0); // Eingang
	PORTC |= _BV(PC0); // pullup

	// ############ Alarm ################
	// 3 Taster LEDs
	DDR_LED |= B_LED1 | B_LED2 | B_LED3; // Ausgang
	// Taster
	DDRB &= ~_BV(PB1);      // Eingang
	PORTB |= _BV(PB1);      // pullup
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
				output_set(0, 1);
				output_set(1, 0);
			} else {
				output_set(0, 0);
				output_set(1, 1);
			}
			break;
	}
}


static netvar_desc *out_netvars[NUM_INPUTS];

void switch_netvars_init() {
	out_netvars[0] = netvar_register(0x0102, 0x2f, 1); // Taster Küche
	out_netvars[1] = netvar_register(0x0008, 0x00, 1); // Alarmbutton
}

#define NV_IDX_LAMP_CONTROLLER_KUECHE 0x0102

void lamp_out_init() {
	new_netvar_output_8(NV_IDX_LAMP_CONTROLLER_KUECHE, 0x3f, lamp_out, (void *) F_LED);
}
#endif
static void input_changed_event(uint8_t num, uint8_t val) {
	if (num == 1) {
		if (val)
			PORTD |= _BV(PD5) | _BV(PD6) | _BV(PD7);
		else
			PORTD &= ~(_BV(PD5) | _BV(PD6) | _BV(PD7));
	}
#ifndef NO_NETVAR
	netvar_write(out_netvars[num], &val);
#endif
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
#ifdef NO_NETVAR
	keypress();
#endif
}


#ifdef NO_NETVAR
#define HOLD_THRESHOLD 30
#define CLICK_THRESHOLD 0

#define C_SW  (0x00)
#define C_PWM (0x01)
#define O_SW00 (0x00)
#define O_PWM05 (0x05)
#define F_SW_TOGGLE   (0x03)
#define F_PWM_MOD     (0x02)
#define F_PWM_DIR     (0x03)
#define PWM_KUECHE   (O_PWM05)
#define SWL_KUECHE      (O_SW00)

#define LIGHTCANPORT 0x04

#include "can/can.h"

static void keypress(void) {
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
		can_message *msg = can_buffer_get();
		msg->data[0] = C_SW;
		msg->data[1] = SWL_KUECHE;
		msg->data[2] = F_SW_TOGGLE;
		msg->addr_src = myaddr;
		msg->addr_dst = 0x02;
		msg->port_dst = 1;
		msg->port_src = LIGHTCANPORT;
		msg->dlc = 4;
		can_transmit(msg);	/* send packet */
	}
	if (held_0)
	{
		can_message *msg = can_buffer_get();
		msg->data[0] = C_PWM;
		msg->data[1] = PWM_KUECHE;
		msg->data[2] = F_PWM_MOD;
		msg->addr_src = myaddr;
		msg->addr_dst = 0x02;
		msg->port_dst = 1;
		msg->port_src = LIGHTCANPORT;
		msg->dlc = 4;
		can_transmit(msg);	/* send packet */
	}
	else if (last_held_0)
	{
		can_message *msg = can_buffer_get();
		msg->data[0] = C_PWM;
		msg->data[1] = PWM_KUECHE;
		msg->data[2] = F_PWM_DIR;
		msg->addr_src = myaddr;
		msg->addr_dst = 0x02;
		msg->port_dst = 1;
		msg->port_src = LIGHTCANPORT;
		msg->dlc = 4;
		can_transmit(msg);	/* send packet */
	}
	last_held_0 = held_0;
}
#endif

