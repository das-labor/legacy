#include <avr/io.h>
#include <string.h>

#include "twi_master/twi_master.h"
#include "../include/PowerCommander.h"
#include "i2c_com.h"

static t_outputdata current_outputdata, next_outputdata;

void sync_output_cache()
{
	if (TWIM_Start(TWI_ADDRESS + TW_READ))
	{
		current_outputdata.ports = TWIM_ReadAck();
		current_outputdata.ports += TWIM_ReadAck() << 8;
		uint8_t i;
		for (i = 0; i < PWM_CHAN - 1; i++)
			current_outputdata.pwmval[i] = TWIM_ReadAck();
		current_outputdata.pwmval[i] = TWIM_ReadNack();
	}
	TWIM_Stop();
	
	memcpy(&next_outputdata, &current_outputdata, sizeof (t_outputdata));
}


static void twi_send()
{
	if (TWIM_Start(TWI_ADDRESS + TW_WRITE))
	{
		TWIM_Write(next_outputdata.ports);
		TWIM_Write(next_outputdata.ports >> 8);
		uint8_t i;
		for (i = 0; i < PWM_CHAN; i++)
			TWIM_Write(next_outputdata.pwmval[i]);
	}
	TWIM_Stop();
}

void update_slave_outputs () {
	if ( memcmp(&next_outputdata, &current_outputdata, sizeof (t_outputdata)) ){
		// at least one output is different
		twi_send();
	
		memcpy( &current_outputdata, &next_outputdata, sizeof (t_outputdata));
	}
}

//set an output. val = true or false
void set_output (uint8_t num, uint8_t val) {
	if (val)
		next_outputdata.ports |=  (1 << num);
	else
		next_outputdata.ports &= ~(1 << num);
}

void set_pwm (uint8_t num, uint8_t val) {
	next_outputdata.pwmval[num] = val;
}
