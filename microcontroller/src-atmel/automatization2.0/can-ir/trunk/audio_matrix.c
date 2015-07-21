#include <stdint.h>
#include <avr/io.h>

#include "i2c_pca9555.h"
#include "i2c_tea6420.h"

enum {
	LED_OUT1,
	LED_OUT2,
	LED_OUT3,
	LED_OUT4,
	LED_POWER,
	LED_IN5 = 8,
	LED_IN4,
	LED_IN3,
	LED_IN2,
	LED_IN1,
} AUDIO_MATRIX_LEDS;

#define ALL_LEDS _BV(LED_OUT1) | _BV(LED_OUT2) | _BV(LED_OUT3) | _BV(LED_OUT4) | _BV(LED_POWER) | _BV(LED_IN1) | _BV(LED_IN2) | _BV(LED_IN3) | _BV(LED_IN4) | _BV(LED_IN5)

static uint8_t led_out_state[4];

void set_input_led(void)
{
	uint8_t input[5];
	uint8_t led_set = pca9555_get_output();

	for (uint8_t i = 0; i < 4; i++)
		if ((tea6420_get_state(i) & (_BV(I2) | _BV(I1) | _BV(I0))) < MUTE) {
			input[tea6420_get_state(i) & (_BV(I2) | _BV(I1) | _BV(I0))]++;
			led_out_state[i] = (tea6420_get_state(i) & (_BV(I2) | _BV(I1) | _BV(I0))) + 1;
		}
		else
			led_out_state[i] = 0;

	for (uint8_t i = 0; i < 5; i++)
		if (input[i])
			led_set &= ~(_BV(8 + 5 - i)); // offset, leds are reversed
		else
			led_set |= (_BV(8 + 5 - i));

	pca9555_write_word(led_set, PCA9555_OUT0); // schreibe 16 bit in output 1 & 2
}

static void am_set_default(void)
{
	tea6420_connect_i2o(INPUT_1, GAIN_0DB, OUTPUT_1);
	tea6420_connect_i2o(INPUT_1, GAIN_0DB, OUTPUT_2);
	tea6420_connect_i2o(INPUT_1, GAIN_0DB, OUTPUT_3);
	tea6420_connect_i2o(INPUT_1, GAIN_0DB, OUTPUT_4);
	set_input_led();
}

void am_set_power_led(uint8_t state)
{
/*	uint8_t led_set;
	if (state)
		led_set = pca9555_get_output() & ~_BV(LED_POWER);
	else
		led_set = pca9555_get_output() | ALL_LEDS;
	
	pca9555_write_word(led_set, PCA9555_OUT0); // schreibe 16 bit in output 1 & 2*/
	if (state)
		am_set_default();
}

void am_init(void)
{
	pca9555_write_word(ALL_LEDS, PCA9555_CFG0); // set to output ports
	am_set_power_led(1);
}


void am_led_tick(void)
{
	static uint8_t counter;
	static uint8_t led_counter[4];
	uint8_t led_set = _BV(LED_OUT1) | _BV(LED_OUT2) | _BV(LED_OUT3) | _BV(LED_OUT4);

	if (counter < 30) {
		if (counter < 10)
			for (uint8_t i = 0; i < 4; i++) {
				if (!counter && led_out_state[i]) {
					led_counter[i] = led_out_state[i];
					led_set = pca9555_get_output() & ~_BV(i);
				}
				else if (counter % 2) {
					if (led_counter[i]) {
						led_counter[i]--;
						led_set = pca9555_get_output() | _BV(i);
					}
				}
				else if (led_counter[i])
					led_set = pca9555_get_output() & ~_BV(i);
					
				pca9555_write_word(led_set, PCA9555_OUT0); // schreibe 16 bit in output 1 & 2
			}
		counter++;
	}
	else
		counter = 0;
}
