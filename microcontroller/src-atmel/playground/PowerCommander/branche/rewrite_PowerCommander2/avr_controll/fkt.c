/* -*- Mode: C; tab-width: 2 -*- */
/*
	set-ter und get-ter funktionen fuer den i2cslave
*/
#include "PowerCommander.h"
#include "PowerCommander_matrix.h"
#include "fkt.h"

void switch_fkt(struct t_i2cproto* i2cproto)
{
	switch(i2cproto->fkt){
	case F_SW_OFF:
		{
			switch_on(sw_matrix[i2cproto->object].port, 
								sw_matrix[i2cproto->object].pin);
		}
		break;
	case F_SW_ON:
		{
			switch_off(sw_matrix[i2cproto->object].port, 
								 sw_matrix[i2cproto->object].pin);
		}
		break;
	case F_SW_STATUS:
		{
			switch_status(sw_matrix[i2cproto->object].port, 
										sw_matrix[i2cproto->object].pin, 
										&(i2cproto->out_data) );
			i2cproto->has_out_data = HASDATA;
		}
		break;
	default:
		break;
	}
}

void pwm_fkt(struct t_i2cproto* i2cproto)
{
	switch(i2cproto->fkt){
	case F_PWM_SET:
		{
			pwm_set(pwm_matrix[i2cproto->object].port, i2cproto->in_data);
		}
		break;
	case F_PWM_GET:
		{
			pwm_get(pwm_matrix[i2cproto->object].port, 
							&(i2cproto->out_data));
			i2cproto->has_out_data = HASDATA;
		}
		break;
	default:
		break;
	}
}


void switch_on(volatile uint8_t *port,uint8_t pin)
{
	(*port) |= _BV(pin);
}

void switch_off(volatile uint8_t *port,uint8_t pin)
{
	(*port) &= ~_BV(pin);
}

void switch_status(volatile uint8_t *port,uint8_t pin, uint8_t *result)
{
	(*result) = (*port) >> _BV(pin) & 1;
}

void pwm_set(volatile uint8_t *port,uint8_t value)
{
	(*port) = value;
}

void pwm_get(volatile uint8_t *port,uint8_t *result)
{
	(*result) = (*port);
}
