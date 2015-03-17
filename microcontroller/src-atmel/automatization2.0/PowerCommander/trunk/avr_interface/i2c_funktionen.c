#include <avr/io.h>

#include "twi_master/twi_master.h"
#include "../include/PowerCommander.h"
#include "i2c_funktionen.h"
#include "can_handler.h"

t_outputdata outputdata;

void sync_stat_cache()
{
	if (TWIM_Start(TWI_ADDRESS + TW_READ))
	{
		outputdata.ports = TWIM_ReadAck();
		outputdata.ports += TWIM_ReadAck() << 8;
		uint8_t i;
		for (i = 0; i < PWM_CHAN - 1; i++)
			outputdata.pwmval[i] = TWIM_ReadAck();
		outputdata.pwmval[i] = TWIM_ReadNack();
	}
	TWIM_Stop();
}


void twi_send()
{
	uint8_t *p_od = (uint8_t *) &outputdata;
	static uint8_t lastdata[PWM_CHAN + 2];
	for (uint8_t i = 0; i < PWM_CHAN + 2; i++)
		if (*(p_od + i) != lastdata[i])
			goto send;
	return;
send:
	if (TWIM_Start(TWI_ADDRESS + TW_WRITE))
		for (uint8_t i = 0; i < PWM_CHAN + 2; i++) {
			TWIM_Write(*(p_od + i));
			lastdata[i] = *(p_od + i);
		}
	TWIM_Stop();
	can_send_output_status();
}
