/* -*- Mode: C; tab-width: 2 -*- */

#include <avr/io.h>
#include "can/can_handler.h"
#include <util/delay.h>


#include "twi_master/twi_master.h"
#include "can/can_handler.h"
#include "../include/PowerCommander.h"
	static uint8_t stat_vortrag = 0; // nur an oder aus 
	static uint8_t stat_lounge = 0; // nur an oder aus
	static uint8_t stat_haupt = 0;

void switch_handler()
{

	static uint8_t outdata[8];

		if ((!(PINB & _BV(PB2))) && (stat_vortrag == 1))
		{
			outdata[0]=C_SW;
			outdata[1]=SWL_LOUNGE;
			outdata[2]=F_SW_OFF;
			outdata[3]=0x00;
			
			twi_send(outdata);
			stat_vortrag = 0;
			_delay_ms(200);
		}
		if ((!(PINB & _BV(PB2))) && stat_vortrag == 0)
		{
			outdata[0]=C_SW;
			outdata[1]=SWL_LOUNGE;
			outdata[2]=F_SW_ON;
			outdata[3]=0x00;
			
			twi_send(outdata);
			stat_vortrag = 1;
			_delay_ms(200);
		}
		
		if ((!(PIND & _BV(PD3))) && stat_lounge == 1)
		{
			outdata[0]=C_VIRT;
			outdata[1]=VIRT_VORTRAG;
			outdata[2]=F_SW_OFF;
			outdata[3]=0x00;
			
			twi_send(outdata);
			stat_lounge = 0;
			_delay_ms(200);
		}
		if ((!(PIND & _BV(PD3))) && stat_lounge == 0)
		{
			outdata[0]=C_VIRT;
			outdata[1]=VIRT_VORTRAG;
			outdata[2]=F_SW_ON;
			outdata[3]=0x00;
			
			twi_send(outdata);
			stat_lounge = 1;
			_delay_ms(200);
		}
		
		if (!(PINA & _BV(PA0)) && stat_haupt == 1)
		{
			outdata[0]=C_VIRT;
			outdata[1]=VIRT_POWER;
			outdata[2]=F_SW_OFF;
			outdata[3]=0x00;
			
			twi_send(outdata);
			stat_haupt = 0;

			PORTA |= _BV(PA3); // red
			PORTA &= ~_BV(PA2); // green
			_delay_ms(500);
		}
		if ((PINA & _BV(PA0)) && stat_haupt == 0)
		{
			outdata[0]=C_VIRT;
			outdata[1]=VIRT_POWER;
			outdata[2]=F_SW_ON;
			outdata[3]=0x00;
			
			twi_send(outdata);

			stat_haupt = 1;

			PORTA |= _BV(PA2); // green
			PORTA &= ~_BV(PA3); // red
			_delay_ms(500);
		}
}

