#include <avr/io.h>

#include "statusled.h"
#include "config.h"

static uint8_t rgb_led_counter;
static uint8_t rgb_led_color;

#ifdef TESTBOARD
	#define LED_ROT_AN()   PORT_LED |= LED_ROT
	#define LED_GRUEN_AN() PORT_LED |= LED_GRUEN
	#define LED_BLAU_AN()  PORT_LED |= LED_BLAU

	#define LED_ROT_AUS()   PORT_LED &= ~LED_ROT
	#define LED_GRUEN_AUS() PORT_LED &= ~LED_GRUEN
	#define LED_BLAU_AUS()  PORT_LED &= ~LED_BLAU
	
	#define LED_ROT_IST_AN()   (PORT_LED & LED_ROT)
	#define LED_GRUEN_IST_AN() (PORT_LED & LED_GRUEN)
	#define LED_BLAU_IST_AN()  (PORT_LED & LED_BLAU)
#else
	#define LED_ROT_AN()   PORT_LED &= ~LED_ROT
	#define LED_GRUEN_AN() PORT_LED &= ~LED_GRUEN
	#define LED_BLAU_AN()  PORT_LED &= ~LED_BLAU

	#define LED_ROT_AUS()   PORT_LED |= LED_ROT
	#define LED_GRUEN_AUS() PORT_LED |= LED_GRUEN
	#define LED_BLAU_AUS()  PORT_LED |= LED_BLAU
	
	#define LED_ROT_IST_AN()   (!(PORT_LED & LED_ROT))
	#define LED_GRUEN_IST_AN() (!(PORT_LED & LED_GRUEN))
	#define LED_BLAU_IST_AN()  (!(PORT_LED & LED_BLAU))
#endif



void set_led(uint8_t color) {
	uint8_t cnt;
	rgb_led_color = color;
	rgb_led_counter = 0;

	cnt = 0;
	if (color & _BV(RED))
		cnt++;
	if (color & _BV(GREEN))
		cnt++;
	if (color & _BV(BLUE))
		cnt++;

	if (cnt < 2) {
		rgb_led_color &= ~_BV(FADE);
	}

	if (rgb_led_color & _BV(BLINK))
		rgb_led_color &= ~_BV(FADE);

	LED_ROT_AUS();
	LED_GRUEN_AUS();
	LED_BLAU_AUS();

	if (rgb_led_color & _BV(FADE)) {
		if (color & _BV(RED)) {
			LED_ROT_AN();
			return;
		}

		if (color & _BV(GREEN)) {
			LED_GRUEN_AN();
			return;
		}

		if (color & _BV(BLUE)) {
			LED_BLAU_AN();
		}
	}
	else
	{
		if (color & _BV(RED))
			LED_ROT_AN();

		if (color & _BV(GREEN))
			LED_GRUEN_AN();

		if (color & _BV(BLUE))
			LED_BLAU_AN();
	}
}

void rgb_led_animation() {
	/* fade between colors */
	if (rgb_led_color & _BV(FADE)) {
		rgb_led_counter++;
		/* update at 3 Hz */
		if (rgb_led_counter > 15) {
			rgb_led_counter = 0;
			if (rgb_led_color & _BV(RED) && (LED_ROT_IST_AN())) {
				LED_ROT_AUS();
				if (rgb_led_color & _BV(GREEN)) {
					LED_GRUEN_AN();
					return;
				}
				if (rgb_led_color & _BV(BLUE)) {
					LED_BLAU_AN();
					return;
				}
			}
			if (rgb_led_color & _BV(GREEN) && (LED_GRUEN_IST_AN())) {
				LED_GRUEN_AUS();
				if (rgb_led_color & _BV(BLUE)) {
					LED_BLAU_AN();
					return;
				}
				if (rgb_led_color & _BV(RED)) {
					LED_ROT_AN();
					return;
				}
			}
			if (rgb_led_color & _BV(BLUE) && (LED_BLAU_IST_AN())) {
				LED_BLAU_AUS();
				if (rgb_led_color & _BV(RED)) {
					LED_ROT_AN();
					return;
				}
				if (rgb_led_color & _BV(GREEN)) {
					LED_GRUEN_AN();
					return;
				}
			}
		}
	}

	/* flash the led */
	if (rgb_led_color & _BV(BLINK)) {
		rgb_led_counter++;

		/* update at 3 Hz */
		if (rgb_led_counter == 15) {
			LED_ROT_AUS();
			LED_GRUEN_AUS();
			LED_BLAU_AUS();
		}
		else if (rgb_led_counter > 30) {
			rgb_led_counter = 0;
			if (rgb_led_color & _BV(RED))
				LED_ROT_AN();
			if (rgb_led_color & _BV(GREEN))
				LED_GRUEN_AN();
			if (rgb_led_color & _BV(BLUE))
				LED_BLAU_AN();
		}
	}
}

