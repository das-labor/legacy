/* -*- Mode: C; tab-width: 2 -*- */

#include <avr/io.h>
#include "can/can_handler.h"
#include <util/delay.h>


#include "twi_master/twi_master.h"
#include "can/can_handler.h"
#include "../include/PowerCommander.h"

union {
	struct {
		uint8_t hauptschalter:1; // 1 Bit für bStatus_1
		uint8_t power_ok:1;    // Dieses Feld ist 2 Bits breit
		uint8_t rcd_server:1;
		uint8_t rcd_power:1;
		uint8_t rcd_licht:1;
	};
	uint8_t bla;
} stat_switches;




void switch_handler()
{
	
	static uint8_t outdata[8];
	
// Hauptschalter
	if (!(PINA & _BV(PA0)) && stat_switches.hauptschalter)
	{
		stat_switches.hauptschalter = 0;

		uint8_t msg[2];
		msg[0] =  stat_switches.bla;
		can_send(msg);
		
		outdata[0]=C_VIRT;
		outdata[1]=VIRT_POWER;
		outdata[2]=F_SW_OFF;
		outdata[3]=0x00;
		
		twi_send(outdata);

		PORTA |= _BV(PA3); // red
		PORTA &= ~_BV(PA2); // green
		_delay_ms(500);
	}
	if ((PINA & _BV(PA0)) && stat_switches.hauptschalter == 0)
	{
		stat_switches.hauptschalter = 1;

		uint8_t msg[2];
		msg[0] =  stat_switches.bla;
		can_send(msg);
		
		outdata[0]=C_VIRT;
		outdata[1]=VIRT_POWER;
		outdata[2]=F_SW_ON;
		outdata[3]=0x00;
		
		twi_send(outdata);
		
		PORTA |= _BV(PA2); // green
		PORTA &= ~_BV(PA3); // red
		_delay_ms(500);
	}
	/*
// PC2 - 24V power good
	if (!(PINC & _BV(PC2)) && stat_switches.power_ok)
	{

		stat_switches.power_ok = 0;

		PORTA |= _BV(PA3); // red
		PORTA &= ~_BV(PA2); // green
	}
	if ((PINC & _BV(PC2)) && stat_switches.power_ok == 0)
	{
		stat_switches.power_ok = 1;

		PORTA |= _BV(PA2); // green
		PORTA &= ~_BV(PA3); // red
	}
// PD6 - RCD Server
	if (!(PIND & _BV(PD6)) && stat_switches.rcd_server)
	{
		stat_switches.rcd_server = 0;

		PORTA |= _BV(PA3); // red
		PORTA &= ~_BV(PA2); // green
	}
	if ((PIND & _BV(PD6)) && stat_switches.rcd_server == 0)
	{
		uint8_t msg[] = {1, 0};
		msg[1] =  stat_switches.bla;
		can_send(msg);
		
		stat_switches.rcd_server = 1;

		PORTA |= _BV(PA2); // green
		PORTA &= ~_BV(PA3); // red
	}
// PA1 - RCD Steckdosen 
	if (!(PINA & _BV(PA1)) && stat_switches.rcd_power)
	{
		stat_switches.rcd_power = 0;

		PORTA |= _BV(PA3); // red
		PORTA &= ~_BV(PA2); // green
	}
	if ((PINA & _BV(PA1)) && stat_switches.rcd_power == 0)
	{
		stat_switches.rcd_power = 1;

		PORTA |= _BV(PA2); // green
		PORTA &= ~_BV(PA3); // red
	}
//  PD7 - RCD Licht
	if (!(PIND & _BV(PD7)) && stat_switches.rcd_licht)
	{
		stat_switches.rcd_licht = 0;

		PORTA |= _BV(PA3); // red
		PORTA &= ~_BV(PA2); // green
		_delay_ms(500);
	}
	if ((PIND & _BV(PD7)) && stat_switches.rcd_licht == 0)
	{
		stat_switches.rcd_licht = 1;

		PORTA |= _BV(PA2); // green
		PORTA &= ~_BV(PA3); // red
	}
	*/
}

