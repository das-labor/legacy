#ifndef POWERCOMMANDER_LED_H
#define POWERCOMMANDER_LED_H


typedef struct {
	uint8_t r:1;
	uint8_t g:1;
	uint8_t b:1;
	uint8_t fade:1;
	uint8_t blink:1;
	uint8_t z:1;
	uint8_t v:1;
} rgb;

void set_led(rgb color);
void rgb_led_animation(void);

#endif
