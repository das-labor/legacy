#include <avr/io.h>

#include "io.h"
#include "uart_handler.h"
#include "config.h"


static struct t_switch_parameter
{
	volatile uint8_t *port;
	uint8_t pin;
} out_matrix[] =
{
	{ O_OUT00_PORT , O_OUT00_BIT},
	{ O_OUT01_PORT , O_OUT01_BIT},
	{ O_OUT02_PORT , O_OUT02_BIT},
	{ O_OUT03_PORT , O_OUT03_BIT},
	{ O_OUT04_PORT , O_OUT04_BIT},
	{ O_OUT05_PORT , O_OUT05_BIT}
};

union
{
	struct
	{
		uint8_t taster_licht:1;
		uint8_t taster_rot:1;
		uint8_t taster_gruen:1;
		uint8_t tuer_kontakt:1;
	} inputs;
	uint8_t status_input;
} stat_inputs;

static struct t_pin_parameter
{
	volatile uint8_t *pin;
	uint8_t bit;
} pin_matrix[] =
{
	{ (&(PIND)), TASTER_LICHT},
	{ (&(PIND)), TASTER_ROT},
	{ (&(PIND)), TASTER_GRUEN},
	{ (&(PINC)), TUER_KONTAKT}
};

void io_init()
{
	LED_DDR |= LED_R | LED_G | LED_B; // Pins LEDs als Ausgang
	RGB_LED_DDR |= LED_1 | LED_2 | LED_3; // Pins RGB LED als Ausgang
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
	for (uint8_t i = 0; i < NUM_OUTPUTS; i++)
		output_set(i, (val >> i) & 0x01);
}


/* 
*  check for changes on monitored inputs
*  on change: call send_status()
*/

static void get_inputs(void)
{
	uint8_t i, msk = 0x01;
	for (i = 0; i < NUM_INPUTS; i++)
	{
		if (((*pin_matrix[i].pin) & pin_matrix[i].bit) && ((stat_inputs.status_input & msk) == 0))
		{
			stat_inputs.status_input |= msk;
			uart_send_status(stat_inputs.status_input);
		}
		else if (!((*pin_matrix[i].pin) & pin_matrix[i].bit) && (stat_inputs.status_input & msk))
		{
			stat_inputs.status_input &= ~msk;
			uart_send_status(stat_inputs.status_input);
		}
		msk <<= 1;
	}
}

void input_handler()
{
	get_inputs();
}

