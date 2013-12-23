#include <avr/io.h>
#include "statusled.h"
#include "config.h"

static uint8_t rgb_led_counter;
static rgb rgb_led_color;

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



void set_led(rgb color) {
	uint8_t cnt;
	rgb_led_color = color;
	rgb_led_counter = 0;

	cnt = 0;
	if (color.r)
		cnt++;
	if (color.g)
		cnt++;
	if (color.b)
		cnt++;

	if (cnt < 2) {
		rgb_led_color.fade = 0;
	}

	if (rgb_led_color.blink) {
		rgb_led_color.fade = 0;
	}

	LED_ROT_AUS();
	LED_GRUEN_AUS();
	LED_BLAU_AUS();

	if (rgb_led_color.fade) {
		if (color.r) {
			LED_ROT_AN();
			return;
		}

		if (color.g) {
			LED_GRUEN_AN();
			return;
		}

		if (color.b) {
			LED_BLAU_AN();
		}
	}
	else
	{
		if (color.r)
			LED_ROT_AN();

		if (color.g)
			LED_GRUEN_AN();

		if (color.b)
			LED_BLAU_AN();
	}
}

void rgb_led_animation() {
	/* fade between colors */
	if (rgb_led_color.fade) {
		rgb_led_counter++;
		/* update at 3 Hz */
		if (rgb_led_counter > 15) {
			rgb_led_counter = 0;
			if (rgb_led_color.r && (LED_ROT_IST_AN())) {
				LED_ROT_AUS();
				if (rgb_led_color.g) {
					LED_GRUEN_AN();
					return;
				}
				if (rgb_led_color.b) {
					LED_BLAU_AN();
					return;
				}
			}
			if (rgb_led_color.g && (LED_GRUEN_IST_AN())) {
				LED_GRUEN_AUS();
				if (rgb_led_color.b) {
					LED_BLAU_AN();
					return;
				}
				if (rgb_led_color.r) {
					LED_ROT_AN();
					return;
				}
			}
			if (rgb_led_color.b && (LED_BLAU_IST_AN())) {
				LED_BLAU_AUS();
				if (rgb_led_color.r) {
					LED_ROT_AN();
					return;
				}
				if (rgb_led_color.g) {
					LED_GRUEN_AN();
					return;
				}
			}
		}
	}

	/* flash the led */
	if (rgb_led_color.blink) {
		rgb_led_counter++;

		/* update at 3 Hz */
		if (rgb_led_counter == 15) {
			LED_ROT_AUS();
			LED_GRUEN_AUS();
			LED_BLAU_AUS();
		}
		else if (rgb_led_counter > 30) {
			rgb_led_counter = 0;
			if (rgb_led_color.r)
				LED_ROT_AN();
			if (rgb_led_color.g)
				LED_GRUEN_AN();
			if (rgb_led_color.b)
				LED_BLAU_AN();
		}
	}
}

