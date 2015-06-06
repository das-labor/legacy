#pragma once

enum {
	I0,
	I1,
	I2,
	G0,
	G1,
	Q0,
	Q1,
} TEA6420_CONFIG;

enum {
	OUTPUT_1,
	OUTPUT_2,
	OUTPUT_3,
	OUTPUT_4,
} TEA6420_OUTPUT;

enum {
	INPUT_1,
	INPUT_2,
	INPUT_3,
	INPUT_4,
	INPUT_5,
	MUTE,
} TEA6420_INPUT;

enum {
	GAIN_6DB,
	GAIN_4DB,
	GAIN_2DB,
	GAIN_0DB,
} TEA6420_GAIN;

extern uint8_t tea6420_get_state(uint8_t output);
extern void tea6420_connect_i2o(uint8_t input, uint8_t gain, uint8_t output);

