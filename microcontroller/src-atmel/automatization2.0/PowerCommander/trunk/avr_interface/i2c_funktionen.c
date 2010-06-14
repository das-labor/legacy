/* -*- Mode: C; tab-width: 2 -*- */

#include <avr/io.h>

#include "i2c_funktionen.h"
#include "twi_master/twi_master.h"
#include "../include/PowerCommander.h"

#define I2C_INDATACOUNT 1

void sync_stat_cache()
{

}


void twi_send(uint8_t *p)
{
	uint8_t i;
	if (!TWIM_Start(TWI_ADDRESS + TW_WRITE))
	{
		TWIM_Stop();
	}
	else
	{
		/*
			daten wirklich raus schreiben
		*/
		for (i = 0; i < 4; i++)
		{
			TWIM_Write(p[i]);
		}
		
		TWIM_Stop();
	}
}

void twi_get(uint8_t *p)
{
	uint8_t i;
	if (!TWIM_Start(TWI_ADDRESS + TW_READ))
	{
		TWIM_Stop();
	}
	else
	{
		for (i = 0; i < (I2C_INDATACOUNT - 1); i++)
		{
			p[i] = TWIM_ReadAck();
		}
		
		//	die letzte via ReadNack

		p[i] = TWIM_ReadNack();
		TWIM_Stop();
	}
}
