#pragma once

void am_init(void);
void am_set_power_led(uint8_t state);
void am_tick(void);
void am_set_output(uint8_t input, uint8_t output);
void am_set_gain(uint8_t gain, uint8_t output);
void am_init_delay(void);
