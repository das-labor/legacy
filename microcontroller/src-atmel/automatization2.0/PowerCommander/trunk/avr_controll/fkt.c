/*
	set-ter funktionen fuer den i2cslave
*/
#include "fkt.h"
#include "PowerCommander_matrix.h"


void switch_set(uint8_t output, uint8_t value);
void pwm_set(uint8_t port, uint8_t value);

t_outputdata outputdata = {0, {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}};

void set_outputs() {
	for (uint8_t i = 0; i < 12; i++)
		switch_set(i, ((outputdata.ports) >> i) & 0x01);

	for (uint8_t i = 0; i < PWM_CHAN; i++)
		pwm_set(i, outputdata.pwmval[i]);
}

void switch_set(uint8_t output, uint8_t value)
{
	if (value == 1)
		(*sw_matrix[output].port) |= sw_matrix[output].pin;
	else
		(*sw_matrix[output].port) &= ~(sw_matrix[output].pin);
}

void pwm_set(uint8_t port, uint8_t value)
{
	(*pwm_matrix[port].port) = value;
}

