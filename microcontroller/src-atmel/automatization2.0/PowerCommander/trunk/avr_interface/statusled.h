#ifndef POWERCOMMANDER_LED_H
#define POWERCOMMANDER_LED_H

enum {
	RED,
	GREEN,
	BLUE,
	FADE,
	BLINK
} rgb_status;


void set_led(uint8_t);
void rgb_led_animation(void);

#endif
