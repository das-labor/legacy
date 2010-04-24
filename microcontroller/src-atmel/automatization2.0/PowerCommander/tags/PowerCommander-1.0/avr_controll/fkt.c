/* -*- Mode: C; tab-width: 2 -*- */
/*
	set-ter und get-ter funktionen fuer den i2cslave
*/
#include "PowerCommander.h"
#include "PowerCommander_matrix.h"
#include "fkt.h"

static uint8_t power_stat;
static uint8_t virt_vortrag_stat;
static uint8_t virt_vortrag_pwm_dir;

static uint8_t virt_lounge_pwm_dir;
static uint8_t virt_kueche_pwm_dir;

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
		case F_PWM_MOD:{
			uint8_t port_power, port_pwm_value;
			if (i2cproto->object == PWM_LOUNGE) {
				pwm_get(pwm_matrix[i2cproto->object].port, &port_pwm_value);
				switch_status(sw_matrix[SWL_LOUNGE].port, sw_matrix[SWL_LOUNGE].pin, &port_power);
				if (!port_power) {
					switch_on(sw_matrix[SWL_LOUNGE].port, sw_matrix[SWL_LOUNGE].pin);
					port_pwm_value = 0;
				}
			
				if (port_pwm_value == 255)
					virt_lounge_pwm_dir = 0;
				if (port_pwm_value == 0)
					virt_lounge_pwm_dir = 1;
				if (virt_lounge_pwm_dir)
					port_pwm_value += 1;
				else
					port_pwm_value -= 1;
				pwm_set(pwm_matrix[i2cproto->object].port, port_pwm_value);
			}
			if (i2cproto->object == PWM_KUECHE) {
				pwm_get(pwm_matrix[i2cproto->object].port, &port_pwm_value);
				switch_status(sw_matrix[SWL_KUECHE].port, sw_matrix[SWL_KUECHE].pin, &port_power);
				if (!port_power) {
					switch_on(sw_matrix[SWL_KUECHE].port, sw_matrix[SWL_KUECHE].pin);
					port_pwm_value = 0;
				}
			
				if (port_pwm_value == 255)
					virt_kueche_pwm_dir = 0;
				if (port_pwm_value == 0)
					virt_kueche_pwm_dir = 1;
				if (virt_kueche_pwm_dir)
					port_pwm_value += 1;
				else
					port_pwm_value -= 1;
				pwm_set(pwm_matrix[i2cproto->object].port, port_pwm_value);
				}
			break;
		}
		case F_PWM_DIR:
			if (i2cproto->object == PWM_KUECHE) {
				if (virt_kueche_pwm_dir)
					virt_kueche_pwm_dir = 0;
				else
					virt_kueche_pwm_dir = 1;
			}
			if (i2cproto->object == PWM_LOUNGE) {
				if (virt_lounge_pwm_dir)
					virt_lounge_pwm_dir = 0;
				else
					virt_lounge_pwm_dir = 1;
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
			virt_power_on();
			break;
		
		case F_SW_OFF:
			virt_power_off();
			break;
			
		case F_SW_STATUS:
			virt_power_status(i2cproto);
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


//increase/decrease value if given min/max is larger/smaller than value before
void virt_vortrag_pwm_set_all(uint8_t min, uint8_t max)
{
	uint8_t objs[] = {PWM_TAFEL, PWM_BEAMER, PWM_SCHRANK, PWM_FLIPPER};
	uint8_t tmp;
	uint8_t x;

	for (x=0;x<sizeof(objs);x++)
	{
		pwm_get(pwm_matrix[objs[x]].port, &tmp);
		if (tmp < min) tmp = min;
		if (tmp > max) tmp = max;
		pwm_set(pwm_matrix[objs[x]].port, tmp);
	}
}

uint8_t pwm_vortrag_get_min()
{
	uint8_t objs[] = {PWM_TAFEL, PWM_BEAMER, PWM_SCHRANK, PWM_FLIPPER};
	uint8_t tmp;
	uint8_t x;
	uint8_t min = 255;

	for (x = 0; x < sizeof(objs); x++)
	{
		pwm_get(pwm_matrix[objs[x]].port, &tmp);
		if (tmp < min) min = tmp;
	}
	
	return min;
}

uint8_t pwm_vortrag_get_max()
{
	uint8_t objs[] = {PWM_TAFEL, PWM_BEAMER, PWM_SCHRANK, PWM_FLIPPER};
	uint8_t tmp;
	uint8_t x;
	uint8_t max = 0;

	for (x = 0; x < sizeof(objs); x++)
	{
		pwm_get(pwm_matrix[objs[x]].port, &tmp);
		if (tmp > max) max = tmp;
	}
	
	return max;
}


void virt_vortrag_pwm_set(struct t_i2cproto* i2cproto)
{
	switch (i2cproto->fkt)
	{
		case F_PWM_SET:
			virt_vortrag_pwm_set_all(i2cproto->in_data, i2cproto->in_data);
			break;
		case F_PWM_MOD: {
				uint8_t val;

				if (virt_vortrag_stat == 0)
				{
					virt_vortrag_on();
					virt_vortrag_pwm_set_all(0, 0);
				}

				if (virt_vortrag_pwm_dir == 1)
				{
					val = pwm_vortrag_get_min();
					if (val == 255)
					{
						virt_vortrag_pwm_dir = 0;
					} else
					{
						virt_vortrag_pwm_set_all(val+1, 255);
					}
				} else
				{
					val = pwm_vortrag_get_max();
					if (val == 0)
					{
						virt_vortrag_pwm_dir = 1;
					} else
					{
						virt_vortrag_pwm_set_all(0, val-1);
					}
				}
			}
			break;
		case F_PWM_DIR:
			if (virt_vortrag_pwm_dir)
				virt_vortrag_pwm_dir = 0;
			else
				virt_vortrag_pwm_dir = 1;
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
	virt_vortrag_pwm_set_all(0xff, 0xff);
	pwm_set(pwm_matrix[PWM_LOUNGE].port, 0xff);
	pwm_set(pwm_matrix[PWM_KUECHE].port, 0xff);
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
