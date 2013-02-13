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

#define LED_RED_BLUE  (rgb){1,0,1,1,0,0,0}
#define LED_RED_GREEN (rgb){1,1,0,1,0,0,0}

#define LED_RED   (rgb){1,0,0,0,0,0,0}
#define LED_GREEN (rgb){0,1,0,0,0,0,0}
#define LED_BLUE  (rgb){0,0,1,0,0,0,0}
#define LED_WHITE (rgb){1,1,1,0,0,0,0}

void set_led(rgb color);
void rgb_led_animation();

#endif
