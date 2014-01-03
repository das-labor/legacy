/*
	set-ter funktionen fuer den i2cslave
*/
#include "fkt.h"
#include "PowerCommander_matrix.h"


static void output_set(uint8_t output, uint8_t value);
static void pwm_set(uint8_t port, uint8_t value);


t_outputdata outputdata = {0, {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}};

void set_outputs() {
	for (uint8_t i = 0; i < 12; i++)
		output_set(i, ((outputdata.ports) >> i) & 0x01);

	for (uint8_t i = 0; i < PWM_CHAN; i++)
		pwm_set(i, outputdata.pwmval[i]);
}

static void output_set(uint8_t output, uint8_t value)
{
	if (value)
		(*out_matrix[output].port) |= out_matrix[output].pin;
	else
		(*out_matrix[output].port) &= ~(out_matrix[output].pin);
}

static void pwm_set(uint8_t port, uint8_t value)
{
	(*pwm_matrix[port].port) = value;
}

