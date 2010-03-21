/* -*- Mode: C; tab-width: 2 -*- */

#include "twi_master/twi_master.h"

#include "temp_regler.h"

uint8_t ventile[] = {42, 44, 46};

void set_ventil(uint8_t stufe)
{
	uint8_t i;
	for (i = 0; i < 3; i++)
	{
		if (!TWIM_Start(ventile[i], TW_WRITE))
		{
			TWIM_Stop();
		}
		else
		{
			TWIM_Write(stufe);
			TWIM_Stop();
		}
	}
}

void temp_regler()
{
	
}

