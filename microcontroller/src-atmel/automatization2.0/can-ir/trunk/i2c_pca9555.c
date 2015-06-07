#include <stdint.h>

#include "i2c_pca9555.h"
#include "twi_master/twi_master.h"
#include "config.h"


#define PCA9555_BASE_ADDR 0x40


static uint16_t pca9555_output_state;

uint16_t pca9555_get_output(void)
{
	return pca9555_output_state;
}

void pca9555_write_byte(uint8_t byte, uint8_t cmd)
{
	if (TWIM_Start(PCA9555_BASE_ADDR + PCA9555_ADDR + TW_WRITE)) {
		TWIM_Write(cmd);
		TWIM_Write(byte);
		//pca9555_state = byte;
	}
	TWIM_Stop();
}

void pca9555_write_word(uint16_t word, uint8_t cmd)
{
	if (TWIM_Start(PCA9555_BASE_ADDR + PCA9555_ADDR + TW_WRITE)) {
		TWIM_Write(cmd);
		TWIM_Write(word & 0xff);
		TWIM_Write((word >> 8) & 0xff);
		pca9555_output_state = word;
	}
	TWIM_Stop();
}

uint16_t pca9555_read_word(uint8_t cmd)
{
	uint16_t word = 0xffff;
	if (TWIM_Start(PCA9555_BASE_ADDR + PCA9555_ADDR + TW_WRITE)) {
		TWIM_Write(cmd);
		if (TWIM_Start(PCA9555_BASE_ADDR + PCA9555_ADDR + TW_READ)) {
			word = TWIM_ReadAck();
			word += (uint16_t) (TWIM_ReadAck() << 8);
		}
	}
	TWIM_Stop();
	return word;
}
