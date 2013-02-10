#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"

#include "twi_master/twi_master.h"
#include "can/can.h"
#include "can_handler.h"
#include "i2c_funktionen.h"
#include "../include/PowerCommander.h"
#include "statusled.h"
#include "virt_lamp.h"

union {
	struct {
		uint8_t hauptschalter:1;
		uint8_t taster_lounge:1;
		uint8_t taster_vortrag:1;
		uint8_t power_ok:1;
		uint8_t rcd_server:1;
		uint8_t rcd_power:1;
		uint8_t rcd_licht:1;
	} inputs;
	uint8_t status_input;
} stat_inputs;

#define HAUPTSCHALTER	(_BV(PA0))
#define POWER_OK		(_BV(PC2))
#define TASTER_LOUNGE	(_BV(PB2))
#define TASTER_VORTRAG	(_BV(PD3))
#define RCD_SERVER		(_BV(PD6))
#define RCD_POWER		(_BV(PD7))
#define RCD_LICHT		(_BV(PA1))

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
	can_message *msg = can_buffer_get();
	//msg = {0x03, 0x00, 0x01, 0x01, 2, {0}};
	msg->data[0] = stat_inputs.status_input;
	msg->data[1] = pos;
	msg->addr_src = myaddr;
	can_transmit(msg);
}


/*
static struct t_i2c_cmd_matrix {
	uint16_t ports;
	uint8_t tue_etwas;
} i2c_cmd_matrix[] = {{0, }, {0, }, {0, }, {0, }, {0, }, {0, }};*/

static uint8_t timeout_cnt;
void start_main_switch_timeout(void) {
	/* 5 seconds timeout */
	timeout_cnt = 250;
}

void handle_main_switch_timeout(void) {
	if (timeout_cnt > 0)
	{
		timeout_cnt--;
		if (!timeout_cnt)
		{
			/* turn of lights, preset default dim level */
			set_lamp_all(ROOM_VORTRAG, 0);
			set_lamp_all(ROOM_LOUNGE, 0);
			set_lamp_all(ROOM_KUECHE, 0);
			/* no need to handle other relays, they are controlled by set_lamp... */
			outputdata.ports &= ~((1<<SWA_HS) | (1<<SWA_KLO) | (1<<SWA_STECKDOSEN) | (1<<SWA_BEAMER));
			twi_send();
		}
	}
}

void exec(uint8_t index) {
	//if (i2c_cmd_matrix[index].tue_etwas) {
	//	uint8_t i;
	//	outputdata.ports = i2c_cmd_matrix[index].ports;
		/*for (i = 0; i <= (i2c_cmd_matrix[index]).i; i++) {
			outputdata.pwm = i2c_cmd_matrix[index]->objectlist[i];
		}*/
	//	twi_send();
	//}
	if (index == 0) {
		if (stat_inputs.inputs.hauptschalter == 1) {
			timeout_cnt = 0;
			outputdata.ports |= (1<<SWA_HS) | (1<<SWA_KLO) | (1<<SWA_STECKDOSEN);
			twi_send();
		}
		else
		{
			set_bright_all(ROOM_VORTRAG, 178);
			set_bright_all(ROOM_KUECHE, 178);
			set_bright_all(ROOM_LOUNGE, 178);
			/* start timeout, shutdown after 5 seconds */
			start_main_switch_timeout();
		}
	}
}


/*
* set the rgb led according to the current state
* Green         : No Error - Labor On
* Blue          : No Error - Labor Off
* White blinking: Error, 24V Power down
* Green blinking: Error, rcd server failed
* Red blinking  : Error, rcd main failed
* Blue blinking : Error, rcd licht failed
*/
void update_rgb_led() {
	if (!stat_inputs.inputs.power_ok) /* Error case */
	{
		set_led( (rgb){ .r = 1, .g = 1, .b = 1 , .fade=0 , .blink=1} );
		return;
	}
	if (stat_inputs.inputs.rcd_server) /* Error case */
	{
		set_led( (rgb){ .r = 0, .g = 1, .b = 0 , .fade=0 , .blink=1} );
		return;
	}
	if (stat_inputs.inputs.rcd_power) /* Error case */
	{
		set_led( (rgb){ .r = 1, .g = 0, .b = 0 , .fade=0 , .blink=1} );
		return;
	}
	if (stat_inputs.inputs.rcd_licht) /* Error case */
	{
		set_led( (rgb){ .r = 0, .g = 0, .b = 1 , .fade=0 , .blink=1} );
		return;
	}
	if (stat_inputs.inputs.hauptschalter) /* Switch on */
	{
		set_led( (rgb){ .r = 0, .g = 1, .b = 0 , .fade=0 , .blink=0} );
	}
	else
	{
		set_led( (rgb){ .r = 0, .g = 0, .b = 1 , .fade=0 , .blink=0} );
	}
}


/* 
*  check for changes on monitored inputs
*  on change: call send_stat() and call exec()
*/

void get_inputs() {
	uint8_t i;
	for (i = 0; i < NUM_INPUTS; i++) {
		if (((*pin_matrix[i].pin) & pin_matrix[i].bit) && (((stat_inputs.status_input >> i) & 1) == 0)) {
			stat_inputs.status_input |= (1 << i);
			//send_stat(i);
			update_rgb_led();
			exec(i);
		}
		if (!((*pin_matrix[i].pin) & pin_matrix[i].bit) && ((stat_inputs.status_input >> i) & 1)) {
			stat_inputs.status_input &= ~(1 << i);
			//send_stat(i);
			update_rgb_led();
			exec(i);
		}
	}
}

uint8_t lamploungepwm[8];

typedef struct {
	uint8_t counter;
	uint8_t last_held;
	uint8_t dim_dir;
	uint8_t bright;
	uint8_t room;
	void    (*sw_funct) ();
	void    (*dim_funct) ();
} taster_status;

void virt_pwm_set_all(taster_status *tst, uint8_t min, uint8_t max)
{
	uint8_t x;
	if (tst->room == 0)
	{
		for (x = 0; x < 4; x++)
		{
			if (outputdata.pwmval[x] < min)
				set_bright(ROOM_VORTRAG, x, min);
			if (outputdata.pwmval[x] > max)
				set_bright(ROOM_VORTRAG, x, max);
		}
	}
	else
	{
		for (x = 0; x < 8; x++)
		{
			if (lamploungepwm[x] < min)
				set_bright(ROOM_LOUNGE, x, min);
			if (lamploungepwm[x] > max)
				set_bright(ROOM_LOUNGE, x, max);
		}
	}
}

uint8_t pwm_get_min(taster_status *tst)
{
	uint8_t min = 255;
	if (tst->room == 0)
	{
		for (uint8_t x = 0; x < 4; x++)
		{
			if (outputdata.pwmval[x] < min)
				min = outputdata.pwmval[x];
		}
	}
	else
	{
		for (uint8_t x = 0; x < 4; x++)
			{
				if (lamploungepwm[x] < min)
					min = lamploungepwm[x];
			}
	}
	return min;
}

uint8_t pwm_get_max(taster_status *tst)
{
	uint8_t max = 0;
	if(tst->room == 0)
	{

		for (uint8_t x = 0; x < 4; x++)
		{
			if (outputdata.pwmval[x] > max)
				max = outputdata.pwmval[x];
		}
	}
	else
	{
		for (uint8_t x = 0; x < 4; x++)
		{
			if (lamploungepwm[x] > max)
				max = lamploungepwm[x];
		}
	}
	return max;
}

void lamp_dim(taster_status *tst) {
	uint8_t val;
	if (!tst->room) {
		if (!((outputdata.ports >> SWL_VORTRAG) & 0x01))
		{
			set_lamp_all(ROOM_VORTRAG, 1);
			virt_pwm_set_all(tst, 0, 0);
		}
	}
	else
	{
		if (!((outputdata.ports >> SWL_LOUNGE) & 0x01))
		{
			set_lamp_all(ROOM_LOUNGE, 1);
			virt_pwm_set_all(tst, 0, 0);
		}
	}

	if (tst->dim_dir)
	{
		val = pwm_get_min(tst);
		if (val == 255)
		{
			tst->dim_dir = 0;
		} else
		{
			virt_pwm_set_all(tst, val + 1, 255);
		}
	}
	else
	{
		val = pwm_get_max(tst);
		if (val == 0)
		{
			tst->dim_dir = 1;
		} else
		{
			virt_pwm_set_all(tst, 0, val - 1);
		}
	}
}



void toggle_vortrag() {
	set_lamp_all(ROOM_VORTRAG, (outputdata.ports >> SWL_VORTRAG) & 0x01 ? 0 : 1);
}
void toggle_lounge() {
	set_lamp_all(ROOM_LOUNGE, (outputdata.ports >> SWL_LOUNGE) & 0x01 ? 0 : 1);
}


#define HOLD_THRESHOLD 23
#define CLICK_THRESHOLD 0
#define NUM_TASTER 2

void dim_vortrag();
void dim_lounge();
static taster_status status[NUM_TASTER] = {{0, 0, 0, 0, 0, &toggle_vortrag, &dim_vortrag}, {0, 0, 0, 0, 1, &toggle_lounge, &dim_lounge}};

void dim_vortrag() {
	lamp_dim(&status[0]);
}
void dim_lounge() {
	lamp_dim(&status[1]);
}

void tog_dimdir_vortrag() {
	status[0].dim_dir ^= 1;
}

void taster() {
	uint8_t i;
	for (i = 0; i < NUM_TASTER; i++)
	{
		uint8_t held = 0;

		if (!(stat_inputs.status_input & _BV(i + 1)))
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
					(*status[i].sw_funct) (); // switch
				}
			}
			status[i].counter = 0;
		}
		if (held)
		{
			(*status[i].dim_funct)(); // dim
		}
		else if (status[i].last_held)
		{
			status[i].dim_dir ^= 1; // toggle dimdir
		}
		status[i].last_held = held;
	}
}


void switch_handler()
{
	get_inputs();
	taster();
	rgb_led_animation();
	handle_main_switch_timeout();
}
