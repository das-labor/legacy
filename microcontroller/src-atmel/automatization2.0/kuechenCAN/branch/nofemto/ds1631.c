#include <avr/io.h>

#include "twi_master/twi_master.h"
#include "ds1631.h"
#include "config.h"

// XXX error handling, init
void init_ds1631(uint8_t addr)
{
	uint8_t temp = 0;
	if (TWIM_Start(addr + TW_WRITE))
	{
		TWIM_Write(DS1631_SOFTWARE_POR);
	}
	TWIM_Stop();
	if (TWIM_Start(addr + TW_WRITE))
	{
		TWIM_Write(DS1631_ACCESS_CONFIG);
	}
	TWIM_Stop();
	if (TWIM_Start(addr + TW_READ))
	{
		temp = TWIM_ReadAck();
	}
	TWIM_Stop();
	/*if (temp != DS1631_I2CDEFAULTCONFIG)
		
	*/
	if (TWIM_Start(addr + TW_WRITE))
	{
		TWIM_Write(DS1631_START_CONVERT);
	}
	TWIM_Stop();
}

void get_temp_ds1631(uint8_t *p, uint8_t addr)
{
	if (!TWIM_Start(addr + TW_WRITE))
	{
		TWIM_Stop();
		*p = 0xc400;
		return ;
	}
	else
	{
		TWIM_Write(DS1631_READ_TEMPERATURE);
	}
	if (TWIM_Start(addr + TW_READ))
	{
		p[0] = TWIM_ReadAck();
		p[1] = TWIM_ReadNack();
	}
	TWIM_Stop();
}

