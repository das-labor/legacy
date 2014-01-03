#include <avr/io.h>

#include "switch_handler.h"
#include "can_handler.h"
#include "config.h"

/*
union {
	struct {
		uint8_t klingel:1;
		uint8_t standby:1;
		uint8_t tuerkontakt:1;
		uint8_t schloss:1;
	} switches;
} stat_switches;

*/

static uint8_t stat_sw;

static struct t_pin_parameter {
	volatile uint8_t *pin;
	uint8_t bit;
} pin_matrix[] = {
	{SIGNAL_PIN, KLINGEL},
	{SIGNAL_PIN, STANDBY},
	{SIGNAL_PIN, TUER_KONTAKT}
};

static void get_switches(void) {
	uint8_t i, msk = 0x01;

	for (i = 0; i < NUM_INPUTS; i++) {
		if (((*pin_matrix[i].pin) & pin_matrix[i].bit) && (stat_sw & msk) == 0) {
			stat_sw |= msk;
			can_send_status(stat_sw, i);
		}
		else if ( !((*pin_matrix[i].pin) & pin_matrix[i].bit) && (stat_sw & msk)) {
			stat_sw &= ~msk;
			can_send_status(stat_sw, i);
		}
		msk <<= 1;
	}
}

static uint8_t led_status;
void led_update(void) {
	if (led_status == 0)
		PORTB &= ~_BV(PB0);
	if (led_status == 1)
		PORTB ^= _BV(PB0);
	if (led_status == 2)
		PORTB |= _BV(PB0);
}

void switch_handler()
{
	get_switches();
	led_update();
}

/* hs | schützout
   0    0 led an
   0    1 blink
   1    0 
   1    1 led aus
*/
void set_led(uint8_t status) {
	if (status & 1 && status & 2)
		led_status = 0;
	else if ((!(status & 1)) && status & 2)
		led_status = 1;
	else if (status & 1 && (!(status & 2)))
		led_status = 2;
	else if ((!(status & 1)) && !(status & 2))
		led_status = 2;
	led_update();
}

