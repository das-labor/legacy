#include <avr/io.h>

#include "twi_master/twi_master.h"
#include "i2c_temp.h"
#include "config.h"


void init_sensor(uint8_t addr)
{
	if (!TWIM_Start(addr + TW_WRITE))
	{
		TWIM_Stop();
	}
	else
	{
		TWIM_Write(SOFTWARE_POR);
		TWIM_Stop();
	}
	if (!TWIM_Start(addr + TW_WRITE))
	{
		TWIM_Stop();
	}
	else
	{
		TWIM_Write(ACCESS_CONFIG);
		TWIM_Write(0b10001100); // XXX nicht soo toll
		TWIM_Stop();
	}
	if (!TWIM_Start(addr + TW_WRITE))
	{
		TWIM_Stop();
	}
	else
	{
		TWIM_Write(START_CONVERT);
		TWIM_Stop();
	}
}

void get_temp(uint8_t *p, uint8_t addr)
{
	if (TWIM_Start(addr + TW_WRITE))
	{
		TWIM_Write(STOP_CONVERT);
	}
	TWIM_Stop();
	if (!TWIM_Start(addr + TW_WRITE))
	{
		TWIM_Stop();
		return;
	}
	else
	{
		TWIM_Write(READ_TEMPERATURE);
	}
	if (TWIM_Start(addr + TW_READ))
	{
		p[0] = TWIM_ReadAck();
		p[1] = TWIM_ReadNack();
	}
	TWIM_Stop();
	if (TWIM_Start(addr + TW_WRITE))
	{
		TWIM_Write(START_CONVERT);
	}
	TWIM_Stop();
}

