/* -*- Mode: C; tab-width: 2 -*- */

#include <avr/io.h>
#include "can/can_handler.h"
#include <util/delay.h>


#include "twi_master/twi_master.h"
#include "can/can_handler.h"
#include "../include/PowerCommander.h"

union {
		unsigned hauptschalter:1; // 1 Bit für bStatus_1
		unsigned vortrag:1; // 1 Bit für bStatus_2
		unsigned lounge:1; // Und hier noch mal ein Bit
		unsigned power_ok:1;    // Dieses Feld ist 2 Bits breit
		unsigned rcd_server:1;
		unsigned rcd_power:1;
		unsigned rcd_licht:1;
		unsigned empty:1;

	uint8_t bla;
} stat_switches;




void switch_handler()
{
	
	static uint8_t outdata[8];
// Vortragsraum
	if ((!(PINB & _BV(PB2))) && stat_switches.vortrag)
	{
		outdata[0]=C_SW;
		outdata[1]=SWL_LOUNGE;
		outdata[2]=F_SW_OFF;
		outdata[3]=0x00;
		
		twi_send(outdata);
		stat_switches.vortrag = 0;
		_delay_ms(200);
	}
	if ((!(PINB & _BV(PB2))) && !stat_switches.vortrag)
	{
		outdata[0]=C_SW;
		outdata[1]=SWL_LOUNGE;
		outdata[2]=F_SW_ON;
		outdata[3]=0x00;
		
		twi_send(outdata);
		stat_switches.vortrag = 1;
		_delay_ms(200);
	}
// Lounge
	if ((!(PIND & _BV(PD3))) && stat_switches.lounge)
	{
		outdata[0]=C_VIRT;
		outdata[1]=VIRT_VORTRAG;
		outdata[2]=F_SW_OFF;
		outdata[3]=0x00;
		
		twi_send(outdata);
		stat_switches.lounge = 0;
		_delay_ms(200);
	}
	if ((!(PIND & _BV(PD3))) && (stat_switches.lounge == 0))
	{
		outdata[0]=C_VIRT;
		outdata[1]=VIRT_VORTRAG;
		outdata[2]=F_SW_ON;
		outdata[3]=0x00;
		
		twi_send(outdata);
		stat_switches.lounge = 1;
		_delay_ms(200);
	}
// Hauptschalter
	if (!(PINA & _BV(PA0)) && stat_switches.hauptschalter == 1)
	{
		stat_switches.hauptschalter = 0;
		
		twi_send(outdata);
		uint8_t msg[] = {1, 0};
		msg[1] =  stat_switches.bla;
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
	if ((PINA & _BV(PA0)) && !(stat_switches.hauptschalter))
	{
		stat_switches.hauptschalter = 1;
		
		twi_send(outdata);
		uint8_t msg[] = {1, 0};
		msg[1] =  stat_switches.bla;
		can_send(msg);
		
		outdata[0]=C_VIRT;
		outdata[1]=VIRT_POWER;
		outdata[2]=F_SW_ON;
		outdata[3]=0x00;
		
		PORTA |= _BV(PA2); // green
		PORTA &= ~_BV(PA3); // red
		_delay_ms(500);
	}
// PC2 - 24V power good
	if (!(PINC & _BV(PC2)) && stat_switches.power_ok)
	{

		stat_switches.power_ok = 0;

		PORTA |= _BV(PA3); // red
		PORTA &= ~_BV(PA2); // green
	}
	if ((PINC & _BV(PC2)) && !stat_switches.power_ok)
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
	if ((PIND & _BV(PD6)) && !stat_switches.rcd_server)
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
	if ((PINA & _BV(PA1)) && !stat_switches.rcd_power)
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
	if ((PIND & _BV(PD7)) && !stat_switches.rcd_licht)
	{
		stat_switches.rcd_licht = 1;

		PORTA |= _BV(PA2); // green
		PORTA &= ~_BV(PA3); // red
	}
}

