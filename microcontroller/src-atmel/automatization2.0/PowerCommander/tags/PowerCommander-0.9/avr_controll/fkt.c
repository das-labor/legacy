/* -*- Mode: C; tab-width: 2 -*- */
/*
	set-ter und get-ter funktionen fuer den i2cslave
*/
#include "PowerCommander.h"
#include "PowerCommander_matrix.h"
#include "fkt.h"

static uint8_t power_stat;
static uint8_t virt_vortrag_stat;

void switch_fkt(struct t_i2cproto* i2cproto)
{
	switch (i2cproto->fkt)
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
										sw_matrix[i2cproto->object].pin, &(i2cproto->out_data));
			i2cproto->has_out_data = HASDATA;
		}
		case F_SW_TOGGLE:
		{
			uint8_t stat;
			switch_status(sw_matrix[i2cproto->object].port, 
										sw_matrix[i2cproto->object].pin, &stat);
			if (stat == 1)
				switch_off(sw_matrix[i2cproto->object].port, 
				           sw_matrix[i2cproto->object].pin);
			else
				switch_on(sw_matrix[i2cproto->object].port, 
				          sw_matrix[i2cproto->object].pin);
		}
		
		break;
		default:
		break;
	}
}

void pwm_fkt(struct t_i2cproto* i2cproto)
{
	switch (i2cproto->fkt)
	{
		case F_PWM_SET:
		{
			pwm_set(pwm_matrix[i2cproto->object].port, i2cproto->in_data);
		}
		break;
		case F_PWM_GET:
		{
			pwm_get(pwm_matrix[i2cproto->object].port, &(i2cproto->out_data));
			i2cproto->has_out_data = HASDATA;
		}
		break;
		default:
		break;
	}
}

void virt_fkt(struct t_i2cproto* i2cproto)
{
	switch (i2cproto->object)
	{
		case VIRT_POWER:
			virt_power(i2cproto);
		break;
		case VIRT_VORTRAG:
			virt_vortrag(i2cproto);
		break;
		case VIRT_VORTRAG_PWM:
			virt_vortrag_pwm_set(i2cproto);
		break;
		default:
		break;
	}
}

void virt_power(struct t_i2cproto* i2cproto)
{
	switch (i2cproto->fkt)
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
		case F_SW_STATUS:
		{
			virt_power_status(i2cproto);
		}
		break;
		default:
		break;
	}
}

void virt_vortrag(struct t_i2cproto* i2cproto)
{
	switch (i2cproto->fkt)
	{
		case F_SW_ON:
			virt_vortrag_on();
		break;
		case F_SW_OFF:
			virt_vortrag_off();
		break;
		case F_SW_STATUS:
			virt_vortrag_status(i2cproto);
		break;
		case F_SW_TOGGLE:
			virt_vortrag_toggle();
		break;
		default:
		break;
	}
}

void virt_vortrag_pwm_set(struct t_i2cproto* i2cproto)
{
	if (i2cproto->fkt == F_PWM_SET)
	{
		pwm_set(pwm_matrix[PWM_TAFEL].port, i2cproto->in_data);
		pwm_set(pwm_matrix[PWM_BEAMER].port, i2cproto->in_data);
		pwm_set(pwm_matrix[PWM_SCHRANK].port, i2cproto->in_data);
		pwm_set(pwm_matrix[PWM_FLIPPER].port, i2cproto->in_data);
	}
}

void virt_power_on()
{
	switch_on(sw_matrix[SWA_KLO].port, sw_matrix[SWA_KLO].pin);
	switch_on(sw_matrix[SWA_HS].port, sw_matrix[SWA_HS].pin);
	switch_on(sw_matrix[SWA_STECKDOSEN].port, sw_matrix[SWA_STECKDOSEN].pin);
	power_stat = 1;
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
	power_stat = 0;
}

void virt_power_status(struct t_i2cproto* i2cproto)
{
	i2cproto->out_data = power_stat;
	i2cproto->has_out_data = HASDATA;
}

void virt_vortrag_on()
{
	switch_on(sw_matrix[SWL_TAFEL].port, sw_matrix[SWL_TAFEL].pin);
	switch_on(sw_matrix[SWL_BEAMER].port, sw_matrix[SWL_BEAMER].pin);
	switch_on(sw_matrix[SWL_SCHRANK].port, sw_matrix[SWL_SCHRANK].pin);
	switch_on(sw_matrix[SWL_FLIPPER].port, sw_matrix[SWL_FLIPPER].pin);
	switch_on(sw_matrix[SWL_VORTRAG].port, sw_matrix[SWL_VORTRAG].pin);
	virt_vortrag_stat = 1;
}

void virt_vortrag_off()
{
	switch_off(sw_matrix[SWL_TAFEL].port, sw_matrix[SWL_TAFEL].pin);
	switch_off(sw_matrix[SWL_BEAMER].port, sw_matrix[SWL_BEAMER].pin);
	switch_off(sw_matrix[SWL_SCHRANK].port, sw_matrix[SWL_SCHRANK].pin);
	switch_off(sw_matrix[SWL_FLIPPER].port, sw_matrix[SWL_FLIPPER].pin);
	switch_off(sw_matrix[SWL_VORTRAG].port, sw_matrix[SWL_VORTRAG].pin);
	virt_vortrag_stat = 0;
}

void virt_vortrag_status(struct t_i2cproto* i2cproto)
{
	i2cproto->out_data = virt_vortrag_stat;
	i2cproto->has_out_data = HASDATA;
}


void virt_vortrag_toggle()
{
	if (virt_vortrag_stat)
		virt_vortrag_off();
	else
		virt_vortrag_on();
}

void switch_on(volatile uint8_t *port, uint8_t pin)
{
	(*port) |= _BV(pin);
}

void switch_off(volatile uint8_t *port, uint8_t pin)
{
	(*port) &= ~_BV(pin);
}

void switch_status(volatile uint8_t *port, uint8_t pin, uint8_t *result)
{
	(*result) = ((*port) >> pin) & 1;
}

void pwm_set(volatile uint8_t *port, uint8_t value)
{
	(*port) = value;
}

void pwm_get(volatile uint8_t *port, uint8_t *result)
{
	(*result) = (*port);
}
