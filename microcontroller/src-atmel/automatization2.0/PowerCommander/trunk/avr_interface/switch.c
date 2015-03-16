#include <avr/io.h>

#include "config.h"
#include "switch.h"
#include "can_handler.h"
#include "../include/PowerCommander.h"
#include "i2c_funktionen.h"
#include "statusled.h"
#include "virt_lamp.h"


enum {
IN_HAUPTSCHALTER,
IN_TASTER_LOUNGE,
IN_TASTER_VORTRAG,
IN_POWER_OK,
IN_RCD_SERVER,
IN_RCD_POWER,
IN_RCD_LICHT,
} input_status_t;

static uint8_t status_input;


static struct t_pin_parameter {
	volatile uint8_t *pin;
	uint8_t bit;
} pin_matrix[] = {
	{ (&(PINA)), PIN_HAUPTSCHALTER},
	{ (&(PINB)), PIN_TASTER_LOUNGE},
	{ (&(PIND)), PIN_TASTER_VORTRAG},
	{ (&(PIND)), PIN_POWER_OK},
	{ (&(PINA)), PIN_RCD_SERVER},
	{ (&(PINC)), PIN_RCD_POWER},
	{ (&(PIND)), PIN_RCD_LICHT}
};


static uint8_t timeout_cnt;
static void start_main_switch_timeout(void) {
	/* 5 seconds timeout */
	timeout_cnt = 250;
}

static void handle_main_switch_timeout(void) {
	if (timeout_cnt > 0) {
		timeout_cnt--;
		if (!timeout_cnt) {
			/* turn of lights, preset default dim level */
			set_lamp_all(ROOM_VORTRAG, 0);
			set_lamp_all(ROOM_LOUNGE, 0);
			set_lamp_all(ROOM_KUECHE, 0);
			/* no need to handle other relays, they are controlled by set_lamp... */
			outputdata.ports &= ~((1 << SWA_HS) | (1 << SWA_KLO) | (1 << SWA_STECKDOSEN));
			twi_send();
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
static void update_rgb_led(void) {
	if (!(status_input & _BV(IN_POWER_OK))) { /* Error case */
		set_led(_BV(RED) | _BV(GREEN) | _BV(BLUE) | _BV(BLINK));
		return;
	}
	if (status_input & _BV(IN_RCD_SERVER)) { /* Error case */
		set_led(_BV(GREEN) | _BV(BLINK));
		return;
	}
	if (status_input & _BV(IN_RCD_POWER)) { /* Error case */
		set_led(_BV(RED) | _BV(BLINK));
		return;
	}
	if (status_input & _BV(IN_RCD_LICHT)) { /* Error case */
		set_led(_BV(BLUE) | _BV(BLINK));
		return;
	}
	if (status_input & _BV(IN_HAUPTSCHALTER)) { /* Switch on */
		set_led(_BV(GREEN));
	}
	else {
		set_led(_BV(BLUE));
	}
}

static void exec(uint8_t index) {
	update_rgb_led();
	if (index == 0) {
		if ((status_input & _BV(IN_HAUPTSCHALTER)) == 1) {
			timeout_cnt = 0;
			outputdata.ports |= (1<<SWA_HS) | (1<<SWA_KLO) | (1<<SWA_STECKDOSEN);
			twi_send();
		}
		else {
			set_bright_all(ROOM_VORTRAG, 178);
			set_bright_all(ROOM_KUECHE, 178);
			set_bright_all(ROOM_LOUNGE, 178);
			/* start timeout, shutdown after 5 seconds */
			start_main_switch_timeout();
		}
	}
	if (index != IN_TASTER_VORTRAG && index != IN_TASTER_LOUNGE)
		can_send_input_stat(index, status_input);
}

/*
*  check for changes on monitored inputs
*  on change: call send_stat() and call exec()
*/

static void get_inputs(void) {
	uint8_t i, msk = 0x01;
	for (i = 0; i < NUM_INPUTS; i++) {
		if (((*pin_matrix[i].pin) & pin_matrix[i].bit) && ((status_input & msk) == 0)) {
			status_input |= msk;
			exec(i);
		}
		else if (!((*pin_matrix[i].pin) & pin_matrix[i].bit) && (status_input & msk)) {
			status_input &= ~msk;
			exec(i);
		}
		msk <<= 1;
	}
}

uint8_t lamploungepwm[8];

typedef struct {
	uint8_t raum;
	uint8_t anzahl_lampen;
	uint8_t sw_funktion;
	uint8_t *pwm_values;
} raum_strukt;

typedef struct {
	uint8_t counter;
	uint8_t last_held;
	uint8_t dim_dir;
	raum_strukt *s_raum;
} taster_strukt;


static void virt_pwm_set_all(taster_strukt *tst, uint8_t min, uint8_t max)
{
	for (uint8_t x = 0; x < (tst->s_raum)->anzahl_lampen; x++)
	{
		if ((tst->s_raum)->pwm_values[x] < min)
			set_bright((tst->s_raum)->raum, x, min);
		if ((tst->s_raum)->pwm_values[x] > max)
			set_bright((tst->s_raum)->raum, x, max);
	}
}

static uint8_t pwm_get_min(taster_strukt *tst)
{
	uint8_t min = 255;
	for (uint8_t x = 0; x < (tst->s_raum)->anzahl_lampen; x++)
		if ((tst->s_raum)->pwm_values[x] < min)
			min = (tst->s_raum)->pwm_values[x];
	return min;
}

static uint8_t pwm_get_max(taster_strukt *tst)
{
	uint8_t max = 0;
	for (uint8_t x = 0; x < (tst->s_raum)->anzahl_lampen; x++)
		if ((tst->s_raum)->pwm_values[x] > max)
			max = (tst->s_raum)->pwm_values[x];
	return max;
}

static void lamp_dim(taster_strukt *tst) {
	uint8_t val;

	if (!((outputdata.ports >> (tst->s_raum)->sw_funktion) & 0x01)) {
		set_lamp_all((tst->s_raum)->raum, 1);
		virt_pwm_set_all(tst, 0, 0);
	}

	if (tst->dim_dir)
	{
		val = pwm_get_min(tst);
		if (val == 255)
			tst->dim_dir = 0;
		else
			virt_pwm_set_all(tst, val + 1, 255);
	}
	else
	{
		val = pwm_get_max(tst);
		if (val == 0)
			tst->dim_dir = 1;
		else
			virt_pwm_set_all(tst, 0, val - 1);
	}
}

static void toggle_raum_licht(taster_strukt *tst) {
    /* toggle all lamps by current relais status */
	set_lamp_all((tst->s_raum)->raum, ((outputdata.ports >> (tst->s_raum)->sw_funktion) & 0x01) ^ 1);
}


#define HOLD_THRESHOLD 23
#define CLICK_THRESHOLD 0
#define NUM_TASTER 2

static raum_strukt r_vortrag = {ROOM_VORTRAG, 4, SWL_VORTRAG, outputdata.pwmval};
static raum_strukt r_lounge  = {ROOM_LOUNGE, 8, SWL_LOUNGE, lamploungepwm};
static raum_strukt r_kueche  = {ROOM_KUECHE, 1, SWL_KUECHE, &outputdata.pwmval[5]};

static taster_strukt taster[3] = {{0, 0, 0, &r_vortrag}, {0, 0, 0, &r_lounge}, {0, 0, 0, &r_kueche}};

void dim_vortrag(void) {
	lamp_dim(&taster[0]);
}

void dim_lounge(void) {
	lamp_dim(&taster[1]);
}

void dim_kueche(void) {
	lamp_dim(&taster[2]);
}

void tog_dimdir_vortrag(void) {
	taster[0].dim_dir ^= 1;
}

void tog_dimdir_kueche(void) {
	taster[2].dim_dir ^= 1;
}

void toggle_vortrag(void) {
	toggle_raum_licht(&taster[0]);
}

void toggle_lounge(void) {
	toggle_raum_licht(&taster[1]);
}

void toggle_kueche(void) {
	toggle_raum_licht(&taster[2]);
}


static void taster_auswertung(void) {
	uint8_t i;
	for (i = 0; i < NUM_TASTER; i++)
	{
		uint8_t held = 0;

		if (!(status_input & _BV(i + 1)))
		{
			taster[i].counter ++;
			if (taster[i].counter > HOLD_THRESHOLD)
			{
				held = 1;
				taster[i].counter = HOLD_THRESHOLD;
			}
		} else
		{
			if (taster[i].counter > CLICK_THRESHOLD)
				if (taster[i].counter < HOLD_THRESHOLD)
					toggle_raum_licht(&taster[i]);
			taster[i].counter = 0;
		}
		if (held)
			lamp_dim(&taster[i]);
		else if (taster[i].last_held)
			taster[i].dim_dir ^= 1; // toggle dimdir
		taster[i].last_held = held;
	}
}


void switch_handler()
{
	get_inputs();
	taster_auswertung();
	handle_main_switch_timeout();
}
