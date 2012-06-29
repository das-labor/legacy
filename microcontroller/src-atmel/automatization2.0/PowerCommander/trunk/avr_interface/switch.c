#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"

#include "twi_master/twi_master.h"
#include "can/can.h"
#include "can_handler.h"
#include "i2c_funktionen.h"
#include "../include/PowerCommander.h"

union {
	struct {
		uint8_t hauptschalter:1;
		uint8_t taster_lounge:1;
		uint8_t taster_vortrag:1;
		uint8_t power_ok:1;
		uint8_t rcd_server:1;
		uint8_t rcd_power:1;
		uint8_t rcd_licht:1;
	};
	uint8_t stat_sw;
} stat_switches;

#define HAUPTSCHALTER	(_BV(PA0))
#define POWER_OK	(_BV(PC2))
#define TASTER_LOUNGE	(_BV(PB2))
#define TASTER_VORTRAG	(_BV(PD3))
#define RCD_SERVER	(_BV(PD6))
#define RCD_POWER	(_BV(PD7))
#define RCD_LICHT	(_BV(PA1))

#define NUM_INPUTS 7

static struct t_pin_parameter {
	volatile uint8_t *pin;
	uint8_t bit;
} pin_matrix[] = {
	{ (&(PINA)) , HAUPTSCHALTER},
	{ (&(PINB)) , TASTER_LOUNGE},
	{ (&(PIND)) , TASTER_VORTRAG},
	{ (&(PINC)) , POWER_OK},
	{ (&(PIND)) , RCD_SERVER},
	{ (&(PIND)) , RCD_POWER},
	{ (&(PINA)) , RCD_LICHT}
};


void send_stat(uint8_t pos) {
	static can_message msg = {0x03, 0x00, 0x01, 0x01, 2, {0}};
	msg.data[0] = stat_switches.stat_sw;
	msg.data[1] = pos;
	msg.addr_src = myaddr;
	can_transmit(&msg);
}


/*
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
}*/

static struct t_i2c_cmd_matrix {
	uint16_t ports;
	uint8_t tue_etwas;
} i2c_cmd_matrix[] = {{0, }};

void exec(uint8_t index) {
	if (i2c_cmd_matrix[index].tue_etwas) {
		uint8_t i;
		outputdata.ports = i2c_cmd_matrix[index].ports;
		/*for (i = 0; i <= (i2c_cmd_matrix[index]).i; i++) {
			outdata.object = i2c_cmd_matrix[index]->objectlist[i];
		}*/
		twi_send();
	}
}

void get_inputs() {
	uint8_t i;
	for (i = 0; i < NUM_INPUTS; i++) {
		if (((*pin_matrix[i].pin) & pin_matrix[i].bit) && (((stat_switches.stat_sw >> i) & 1) == 0)) {
			stat_switches.stat_sw |= (1 << i);
			send_stat(i);
			exec(i);
		}
		if (!((*pin_matrix[i].pin) & pin_matrix[i].bit) && ((stat_switches.stat_sw >> i) & 1)) {
			stat_switches.stat_sw &= ~(1 << i);
			send_stat(i);
			exec(i);
		}
	}
}

void set_led() {
	if (stat_switches.stat_sw & 1) {
		PORTA |= LED_GRUEN;
		PORTA &= ~LED_BLAU;
	}
	else {
		PORTA &= ~LED_GRUEN;
		PORTA |= LED_BLAU;
	}

	if (stat_switches.stat_sw > 1)
		PORTA |= LED_ROT;
	else
		PORTA &= ~LED_ROT;
}


#define HOLD_THRESHOLD 18
#define CLICK_THRESHOLD 0
#define NUM_TASTER 2

typedef struct {
	uint8_t counter;
	uint8_t last_held;
	uint8_t dim_dir;
	uint8_t bright;
} taster_status;
/*
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
*/
void lamp_dim() {
/*
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
			virt_vortrag_pwm_set_all(val + 1, 255);
		}
	} else
	{
		val = pwm_vortrag_get_max();
		if (val == 0)
		{
			virt_vortrag_pwm_dir = 1;
		} else
		{
			virt_vortrag_pwm_set_all(0, val - 1);
		}
	}*/
}

void taster() {
	uint8_t i;
	static taster_status status[NUM_TASTER];
	for (i = 0; i < NUM_TASTER; i++)
	{
		uint8_t held = 0;

		if ((stat_switches.stat_sw >> (i + 1)) & 0x01) //XXX
		{
			status[i].counter ++;
			if (status[i].counter > HOLD_THRESHOLD)
			{
				held = 1;
				status[i].counter = HOLD_THRESHOLD;
			}
		} else
		{
			if (status[i].counter > CLICK_THRESHOLD)
			{
				if (status[i].counter < HOLD_THRESHOLD)
				{
					exec(7); // alle an / aus;
				}
			}
			status[i].counter = 0;
		}
		if (held)
		{
			lamp_dim(status[i].bright); // dim
		}
		else if (status[i].last_held)
		{
			status[i].dim_dir ^= 1; // toggle // XXX invert?
		}
		status[i].last_held = held;
	}
}


void switch_handler()
{
	get_inputs();
	set_led();
	taster();
}
