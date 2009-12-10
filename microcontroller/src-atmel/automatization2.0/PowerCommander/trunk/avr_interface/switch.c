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
			_delay_ms(1000);
			stat_vortrag = 0;
		}
		if ((!(PINB & _BV(PB2))) && stat_vortrag == 0)
		{
			outdata[0]=C_SW;
			outdata[1]=SWL_LOUNGE;
			outdata[2]=F_SW_ON;
			outdata[3]=0x00;
			
			twi_send(outdata);
						_delay_ms(1000);
			stat_vortrag = 1;
		}
		
		if ((!(PIND & _BV(PD3))) && stat_lounge == 1)
		{
			outdata[0]=C_VIRT;
			outdata[1]=VIRT_VORTRAG;
			outdata[2]=F_SW_OFF;
			outdata[3]=0x00;
			
			twi_send(outdata);
						_delay_ms(1000);
			stat_lounge = 0;
		}
		if ((!(PIND & _BV(PD3))) && stat_lounge == 0)
		{
			outdata[0]=C_VIRT;
			outdata[1]=VIRT_VORTRAG;
			outdata[2]=F_SW_ON;
			outdata[3]=0x00;
			
			twi_send(outdata);
						_delay_ms(1000);
			stat_lounge = 1;
		}
		
		if (!(PINA & _BV(PA0)) && stat_haupt == 1)
		{
			outdata[0]=C_VIRT;
			outdata[1]=VIRT_POWER;
			outdata[2]=F_SW_OFF;
			outdata[3]=0x00;
			
			twi_send(outdata);
						_delay_ms(1000);
			stat_haupt = 0;

			PORTA |= _BV(PA3); // red
			PORTA &= ~_BV(PA2); // green
		}
		if ((PINA & _BV(PA0)) && stat_haupt == 0)
		{
			outdata[0]=C_VIRT;
			outdata[1]=VIRT_POWER;
			outdata[2]=F_SW_ON;
			outdata[3]=0x00;
			
			twi_send(outdata);
						_delay_ms(1000);
			stat_haupt = 1;

			PORTA |= _BV(PA2); // green
			PORTA &= ~_BV(PA3); // red
		}
}

