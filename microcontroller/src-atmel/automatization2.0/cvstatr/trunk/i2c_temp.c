#include <avr/io.h>

#include "twi_master/twi_master.h"
#include "i2c_temp.h"

#define I2CADRESSE 0x9e

void init_sensor()
{
	if (!TWIM_Start(I2CADRESSE, TW_WRITE))
	{
		TWIM_Stop();
	}
	else
	{
		TWIM_Write(SOFTWARE_POR);
		TWIM_Stop();
	}
	if (!TWIM_Start(I2CADRESSE, TW_WRITE))
	{
		TWIM_Stop();
	}
	else
	{
		TWIM_Write(ACCESS_CONFIG);
		TWIM_Write(0b10001100);
		TWIM_Stop();
	}
	if (!TWIM_Start(I2CADRESSE, TW_WRITE))
	{
		TWIM_Stop();
	}
	else
	{
		TWIM_Write(START_CONVERT);
		TWIM_Stop();
	}
}

void get_temp(uint8_t *p)
{
	if (!TWIM_Start(I2CADRESSE, TW_WRITE))
	{
		TWIM_Stop();
	}
	else
	{
		TWIM_Write(STOP_CONVERT);
		TWIM_Stop();
	}
	if (!TWIM_Start(I2CADRESSE, TW_WRITE))
	{
		TWIM_Stop();
	}
	else
	{
		TWIM_Write(READ_TEMPERATURE);
		//TWIM_Stop();
	}
	if (!TWIM_Start(I2CADRESSE, TW_READ))
	{
		TWIM_Stop();
	}
	else
	{
		p[0] = TWIM_ReadAck();
		p[1] = TWIM_ReadNack();
		TWIM_Stop();
	}
	if (!TWIM_Start(I2CADRESSE, TW_WRITE))
	{
		TWIM_Stop();
	}
	else
	{
		TWIM_Write(START_CONVERT);
		TWIM_Stop();
	}
}

void get_reg(uint8_t *p)
{
	if (!TWIM_Start(I2CADRESSE, TW_WRITE))
	{
		TWIM_Stop();
	}
	else
	{
		TWIM_Write(ACCESS_CONFIG);
		//TWIM_Stop();
	}
	if (!TWIM_Start(I2CADRESSE, TW_READ))
	{
		TWIM_Stop();
	}
	else
	{
		p[0] = TWIM_ReadNack();
		TWIM_Stop();
	}
}

