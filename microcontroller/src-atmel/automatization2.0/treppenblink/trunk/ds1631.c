#include <avr/io.h>

#include "twi_master/twi_master.h"
#include "ds1631.h"
#include "config.h"

// XXX error handling, init
uint8_t init_ds1631(uint8_t addr)
{
	uint8_t ret = 1;
	uint8_t temp = 0;
	if (TWIM_Start(addr + TW_WRITE))
	{
		if (TWIM_Write(DS1631_SOFTWARE_POR))
			ret = 0;
	}
	else
		ret = 0;
	TWIM_Stop();
	if (TWIM_Start(addr + TW_WRITE))
	{
		if (TWIM_Write(DS1631_ACCESS_CONFIG))
			ret = 0;
	}
	TWIM_Stop();
	if (TWIM_Start(addr + TW_READ))
	{
		temp = TWIM_ReadNack();
	}
	else
		ret = 0;
	TWIM_Stop();
	if (temp != DS1631_I2CDEFAULTCONFIG) {
		
	}
	if (TWIM_Start(addr + TW_WRITE))
	{
		if (TWIM_Write(DS1631_START_CONVERT))
			ret = 0;
	}
	else
		ret = 0;
	TWIM_Stop();
	return ret;
}

uint8_t get_temp_ds1631(uint8_t *p, uint8_t addr)
{
	uint8_t ret = 1;
	if (TWIM_Start(addr + TW_WRITE))
	{
		TWIM_Write(DS1631_READ_TEMPERATURE);
	}
	else
	{
		TWIM_Stop();
		p[0] = 0x00;
		p[1] = 0xc4;
		ret = 0;
	} // Repeated start condition
	if (TWIM_Start(addr + TW_READ))
	{
		p[0] = TWIM_ReadAck();
		p[1] = TWIM_ReadNack();
	}
	else
		ret = 0;
	TWIM_Stop();
	return ret;
}

