#include <avr/io.h>
#include "statusled.h"
#include "config.h"

static uint8_t rgb_led_counter;
static rgb rgb_led_color;

void set_led(rgb color) {
	uint8_t cnt;
	rgb_led_color = color;
        rgb_led_counter = 0;

	cnt = 0;
	if(color.r)
		cnt++;
	if(color.g)
		cnt++;
	if(color.b)
		cnt++;

	if(cnt < 2)
	{
		rgb_led_color.fade = 0;
	}

	if(rgb_led_color.blink)
	{
		rgb_led_color.fade = 0;
	}

	PORTA &= ~LED_ROT;
	PORTA &= ~LED_GRUEN;
	PORTA &= ~LED_BLAU;

	if(rgb_led_color.fade)
	{
		if(color.r){
			PORTA |= LED_ROT;
			return;
		}

		if(color.g){
			PORTA |= LED_GRUEN;
			return;
		}

		if(color.b){
			PORTA |= LED_BLAU;
		}

	}
	else
	{
		if(color.r)
			PORTA |= LED_ROT;

		if(color.g)
			PORTA |= LED_GRUEN;

		if(color.b)
			PORTA |= LED_BLAU;
	}
}

void rgb_led_animation( void ) {

	/* fade between colors */
	if(rgb_led_color.fade)
	{
		rgb_led_counter++;
		/* update at 3 Hz */
		if(rgb_led_counter > 15){
			rgb_led_counter = 0;
			if( rgb_led_color.r && (PORTA & LED_ROT) ){
				PORTA &= ~LED_ROT;
				if( rgb_led_color.g){
					PORTA |= LED_GRUEN;
					return;
				}
				if( rgb_led_color.b){
					PORTA |= LED_BLAU;
					return;
				}
			}
			if( rgb_led_color.g && (PORTA & LED_GRUEN) ){
				PORTA &= ~LED_GRUEN;
				if( rgb_led_color.b){
					PORTA |= LED_BLAU;
					return;
				}
				if( rgb_led_color.r){
					PORTA |= LED_ROT;
					return;
				}
			}
			if( rgb_led_color.b && (PORTA & LED_BLAU) ){
				PORTA &= ~LED_BLAU;
				if( rgb_led_color.r){
					PORTA |= LED_ROT;
					return;
				}
				if( rgb_led_color.g){
					PORTA |= LED_GRUEN;
					return;
				}
			}
		}
	}

	/* flash the led */
	if(rgb_led_color.blink)
	{
		rgb_led_counter++;

		/* update at 3 Hz */
		if(rgb_led_counter == 15){
			PORTA &= ~LED_ROT;
			PORTA &= ~LED_GRUEN;
			PORTA &= ~LED_BLAU;
		}
		else if(rgb_led_counter > 30){
			rgb_led_counter = 0;
			if(rgb_led_color.r)
				PORTA |= LED_ROT;
			if(rgb_led_color.g)
				PORTA |= LED_GRUEN;
			if(rgb_led_color.b)
				PORTA |= LED_BLAU;
		}
	}

}
