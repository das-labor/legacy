#include <avr/io.h>
#include <stdint.h>

#include "i2c_tea6420.h"
#include "twi_master/twi_master.h"
#include "config.h"
#include "can_handler.h"

#define TEA6420_BASE_ADDR 0x98

struct {
	uint8_t x:1;
	uint8_t output:2;
	uint8_t gain:2;
	uint8_t input:3;
} t_state; // XXX

static uint8_t tea6420_state[4];

uint8_t tea6420_get_state(uint8_t output)
{
	if (output < 5)
		return tea6420_state[output] & 0x1f;
	else
		return 0;
}

void tea6420_connect_i2o(uint8_t input, uint8_t gain, uint8_t output)
{
	if ((input < 10) && (gain < 4) && (output < 4)) {
		if (TWIM_Start(TEA6420_BASE_ADDR + TEA6420_ADDR + TW_WRITE)) {
			tea6420_state[output] = (output << Q0) + (gain << G0) + input;
			TWIM_Write(tea6420_state[output]);
		}
		TWIM_Stop();
	}
}
