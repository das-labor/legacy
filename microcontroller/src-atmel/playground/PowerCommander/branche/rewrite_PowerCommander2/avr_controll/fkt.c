/* -*- Mode: C; tab-width: 2 -*- */
/*
	set-ter und get-ter funktionen fuer den i2cslave
*/
#include "PowerCommander.h"
#include "PowerCommander_matrix.h"
#include "fkt.h"

void switch_fkt(struct t_i2cproto* i2cproto)
{
	switch(i2cproto->fkt)
	{
	case F_SW_OFF:
		{
			switch_off(sw_matrix[i2cproto->object].port, 
								 sw_matrix[i2cproto->object].pin);
		}
		break;
	case F_SW_ON:
		{
			switch_on(sw_matrix[i2cproto->object].port, 
								sw_matrix[i2cproto->object].pin);
		}
		break;
	case F_SW_STATUS:
		{
			switch_status(sw_matrix[i2cproto->object].port, 
										sw_matrix[i2cproto->object].pin, 
										&(i2cproto->out_data));
			i2cproto->has_out_data = HASDATA;
		}
		break;
	default:
		break;
	}
}

void pwm_fkt(struct t_i2cproto* i2cproto)
{
	switch(i2cproto->fkt)
	{
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

void virt_fkt(struct t_i2cproto* i2cproto)
{
	switch(i2cproto->object)
	{
	case VIRT_POWER:
		virt_power(i2cproto);
		break;
	case VIRT_VORTRAG:
		break;
	default:
		break;
	}
}

void virt_power(struct t_i2cproto* i2cproto)
{
	switch(i2cproto->fkt)
	{
	case F_SW_ON:
		{
			virt_power_on();
		}
		break;
	case F_SW_OFF:
		{
			virt_power_off();
		}
		break;
	default:
		break;
	}
}

void virt_vortrag(struct t_i2cproto* i2cproto)
{
	switch(i2cproto->fkt)
	{
	case F_SW_ON:
		virt_vortrag_on();
		break;
	case F_SW_OFF:
		virt_vortrag_off();
		break;
	default:
		break;
	}
	
}

void virt_power_on()
{
	switch_on(sw_matrix[SWA_KLO].port, sw_matrix[SWA_KLO].pin);
	switch_on(sw_matrix[SWA_HS].port, sw_matrix[SWA_HS].pin);
	switch_on(sw_matrix[SWA_STECKDOSEN].port, sw_matrix[SWA_STECKDOSEN].pin);
}

void virt_power_off()
{
	switch_off(sw_matrix[SWL_TAFEL].port, sw_matrix[SWL_TAFEL].pin);
	switch_off(sw_matrix[SWL_BEAMER].port, sw_matrix[SWL_BEAMER].pin);
	switch_off(sw_matrix[SWL_SCHRANK].port, sw_matrix[SWL_SCHRANK].pin);
	switch_off(sw_matrix[SWL_FLIPPER].port, sw_matrix[SWL_FLIPPER].pin);
	switch_off(sw_matrix[SWL_VORTRAG].port, sw_matrix[SWL_VORTRAG].pin);
	switch_off(sw_matrix[SWL_LOUNGE].port, sw_matrix[SWL_LOUNGE].pin);
	switch_off(sw_matrix[SWA_KLO].port, sw_matrix[SWA_KLO].pin);
	switch_off(sw_matrix[SWL_KUECHE].port, sw_matrix[SWL_KUECHE].pin);
	switch_off(sw_matrix[SWA_HS].port, sw_matrix[SWA_HS].pin);
	switch_off(sw_matrix[SWA_BEAMER].port, sw_matrix[SWA_BEAMER].pin);
	switch_off(sw_matrix[SWA_STECKDOSEN].port, sw_matrix[SWA_STECKDOSEN].pin);
}

void virt_vortrag_on()
{
	switch_on(sw_matrix[SWL_TAFEL].port, sw_matrix[SWL_TAFEL].pin);
	switch_on(sw_matrix[SWL_BEAMER].port, sw_matrix[SWL_BEAMER].pin);
	switch_on(sw_matrix[SWL_SCHRANK].port, sw_matrix[SWL_SCHRANK].pin);
	switch_on(sw_matrix[SWL_FLIPPER].port, sw_matrix[SWL_FLIPPER].pin);
	switch_on(sw_matrix[SWL_VORTRAG].port, sw_matrix[SWL_VORTRAG].pin);
}

void virt_vortrag_off()
{
	switch_off(sw_matrix[SWL_TAFEL].port, sw_matrix[SWL_TAFEL].pin);
	switch_off(sw_matrix[SWL_BEAMER].port, sw_matrix[SWL_BEAMER].pin);
	switch_off(sw_matrix[SWL_SCHRANK].port, sw_matrix[SWL_SCHRANK].pin);
	switch_off(sw_matrix[SWL_FLIPPER].port, sw_matrix[SWL_FLIPPER].pin);
	switch_off(sw_matrix[SWL_VORTRAG].port, sw_matrix[SWL_VORTRAG].pin);
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
	(*result) = ((*port) >> _BV(pin)) & 1;
}

void pwm_set(volatile uint8_t *port,uint8_t value)
{
	(*port) = value;
}

void pwm_get(volatile uint8_t *port,uint8_t *result)
{
	(*result) = (*port);
}
