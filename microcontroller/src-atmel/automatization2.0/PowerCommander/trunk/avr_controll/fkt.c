/*
	set-ter und get-ter funktionen fuer den i2cslave
*/
#include "PowerCommander.h"
#include "PowerCommander_matrix.h"
#include "fkt.h"

void switch_set(uint8_t output, uint8_t value);
void pwm_set(uint8_t port, uint8_t value);


void set_outputs() {
	for (uint8_t i = 0; i < 12; i++) {
		switch_set(i, ((outputdata.ports) >> i) & 0x01);
	}
	for (uint8_t i = 0; i < 6; i++)
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

