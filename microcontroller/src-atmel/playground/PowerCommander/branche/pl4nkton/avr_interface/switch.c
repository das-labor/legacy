/* -*- Mode: C; tab-width: 2 -*- */
#include <avr/io.h>

#include <avrx-io.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "xcan.h"

#include "twi_master/twi_master.h"
#include "queue.h"
#include "PowerCommander.h"

TimerControlBlock switchtimer;
TimerControlBlock switch_timer_vortrag;
TimerControlBlock switch_timer_lounge;
TimerControlBlock taskswitch_timer;

AVRX_GCC_TASKDEF(switch_vortrag, 60, 7)
{
	uint8_t stat_vortrag = 0; // nur an oder aus 
	static t_i2cMessage_out i2c_outdata;

	while (1)
	{
		if ((!(PINB & _BV(PB2))) && (stat_vortrag == 1))
		{
			i2c_outdata.outdata[0]=C_VIRT;
			i2c_outdata.outdata[1]=VIRT_VORTRAG;
			i2c_outdata.outdata[2]=F_SW_OFF;
			i2c_outdata.outdata[3]=0x00;

			AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
			//			AvrXWaitMessageAck(&i2c_outdata.mcb);

			AvrXDelay(&switch_timer_vortrag, 3000);
			stat_vortrag = 0;
		}
		if ((!(PINB & _BV(PB2))) && stat_vortrag == 0)
		{
			i2c_outdata.outdata[0]=C_VIRT;
			i2c_outdata.outdata[1]=VIRT_VORTRAG;
			i2c_outdata.outdata[2]=F_SW_ON;
			i2c_outdata.outdata[3]=0x00;

			AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
			//			AvrXWaitMessageAck(&i2c_outdata.mcb);

			AvrXDelay(&switch_timer_vortrag, 3000);
			stat_vortrag = 1;
		}
		AvrXDelay(&taskswitch_timer, 1);
	}
}

AVRX_GCC_TASKDEF(switch_lounge, 60, 7)
{
	uint8_t stat_lounge=0; // nur an oder aus 
	static t_i2cMessage_out i2c_outdata;
	
	while (1)
	{
		if ((!(PIND & _BV(PD3))) && stat_lounge == 1)
		{
			i2c_outdata.outdata[0]=C_SW;
			i2c_outdata.outdata[1]=SWL_LOUNGE;
			i2c_outdata.outdata[2]=F_SW_OFF;
			i2c_outdata.outdata[3]=0x00;

			AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
			//			AvrXWaitMessageAck(&i2c_outdata.mcb);

			AvrXDelay(&switch_timer_lounge, 3000);
			stat_lounge = 0;
		}
		if ((!(PIND & _BV(PD3))) && stat_lounge == 0)
		{
			i2c_outdata.outdata[0]=C_SW;
			i2c_outdata.outdata[1]=SWL_LOUNGE;
			i2c_outdata.outdata[2]=F_SW_ON;
			i2c_outdata.outdata[3]=0x00;

			AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
			//			AvrXWaitMessageAck(&i2c_outdata.mcb);

			AvrXDelay(&switch_timer_lounge, 3000);
			
			stat_lounge = 1;
		}
		AvrXDelay(&taskswitch_timer, 1);
	}
}

AVRX_GCC_TASKDEF(switchtask, 60, 7)
{
	static t_i2cMessage_out i2c_outdata;
	static t_canMessage_out can_outdata;

	uint8_t stat_haupt = 0;

	while (1)
  {
		if (!(PINA & _BV(PA0)) && stat_haupt == 1)
		{
			PORTA |= _BV(PA3); // red
			PORTA &= ~_BV(PA2); // green

			stat_haupt = 0;

			i2c_outdata.outdata[0] = C_VIRT;
			i2c_outdata.outdata[1] = VIRT_POWER;
			i2c_outdata.outdata[2] = F_SW_OFF;
			i2c_outdata.outdata[3] = 0x00;

			can_outdata.outdata[0]=C_VIRT;
			can_outdata.outdata[1]=VIRT_POWER;
			can_outdata.outdata[2]=F_SW_OFF;
			can_outdata.outdata[3]=0x00;

			/*
				sende nachricht ueber can, das wir ausschalten
			*/
			AvrXSendMessage(&canQueue_out, &can_outdata.mcb);
			//			AvrXWaitMessageAck(&can_outdata.mcb);

			/*
				warte 5 sekunden
			*/
			AvrXDelay(&switchtimer, 5000);

			/*
				schalte aus
			*/
					
			AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
			//			AvrXWaitMessageAck(&i2c_outdata.mcb);
					

			AvrXDelay(&switchtimer, 1000);
		}
		if ((PINA & _BV(PA0)) && stat_haupt == 0)
		{
			PORTA |= _BV(PA2); // green
			PORTA &= ~_BV(PA3); // red

			stat_haupt = 1;

			i2c_outdata.outdata[0] = C_VIRT;
			i2c_outdata.outdata[1] = VIRT_POWER;
			i2c_outdata.outdata[2] = F_SW_ON;
			i2c_outdata.outdata[3] = 0x00;

			can_outdata.outdata[0]=C_VIRT;
			can_outdata.outdata[1]=VIRT_POWER;
			can_outdata.outdata[2]=F_SW_ON;
			can_outdata.outdata[3]=0x00;

			/*
				sende sofort, dass einschlatsignal
			*/
			AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
			//			AvrXWaitMessageAck(&i2c_outdata.mcb);

			/*
				warte 5 sekunden
			*/
			AvrXDelay(&switchtimer, 5000);

			/*
				sende nachricht, dass das eingeschaltet wurde
			*/

			AvrXSendMessage(&canQueue_out, &can_outdata.mcb);
			//			AvrXWaitMessageAck(&can_outdata.mcb); 

			AvrXDelay(&switchtimer, 1000);

		}
		AvrXDelay(&taskswitch_timer, 1);
	}
}

AVRX_GCC_TASKDEF(watchtask, 40, 7)
{

	static t_canMessage_out can_outdata;

	uint8_t stat_rcd = 0, stat_rcd_l = 0, stat_power = 0;
	can_outdata.outdata[0] = C_STAT;

	while (1)
  {
		if (!(PIND & _BV(PD6)) && stat_rcd == 1)
		{				
			PORTA |= _BV(PA3); // red
			PORTA &= ~_BV(PA2); // green

			stat_rcd = 0;

			/*
				hier muss das rein das nachrichten nochmal ueber 
				CAN rausgesendet werden
			*/

			can_outdata.outdata[1] = RCD_HAUPT;
			can_outdata.outdata[2] = ERROR;

			AvrXSendMessage(&canQueue_out, &can_outdata.mcb);
			//AvrXWaitMessageAck(&can_outdata.mcb);
		}
		if ((PIND & _BV(PD6)) && stat_rcd == 0)
		{
			PORTA |= _BV(PA2); // green
			PORTA &= ~_BV(PA3); // red


			stat_rcd = 1;

			/*
				hier muss das rein das nachrichten nochmal ueber 
				CAN rausgesendet werden
			*/
			can_outdata.outdata[1] = RCD_LICHT;
			can_outdata.outdata[2] = OK;

			AvrXSendMessage(&canQueue_out, &can_outdata.mcb);
			//			AvrXWaitMessageAck(&can_outdata.mcb);
		}
		if (!(PIND & _BV(PD7)) && stat_rcd_l == 1)
		{				
			PORTA |= _BV(PA3); // red
			PORTA &= ~_BV(PA2); // green

			stat_rcd_l = 0;

			/*
				hier muss das rein das nachrichten nochmal ueber 
				CAN rausgesendet werden
			*/

			can_outdata.outdata[1] = RCD_LICHT;
			can_outdata.outdata[2] = ERROR;

			AvrXSendMessage(&canQueue_out, &can_outdata.mcb);
			//			AvrXWaitMessageAck(&can_outdata.mcb);
		}
		if ((PIND & _BV(PD7)) && stat_rcd_l == 0)
		{
			PORTA |= _BV(PA2); // green
			PORTA &= ~_BV(PA3); // red


			stat_rcd_l = 1;

			/*
				hier muss das rein das nachrichten nochmal ueber 
				CAN rausgesendet werden
			*/
			can_outdata.outdata[1] = RCD_HAUPT;
			can_outdata.outdata[2] = OK;

			AvrXSendMessage(&canQueue_out, &can_outdata.mcb);
			//			AvrXWaitMessageAck(&can_outdata.mcb);
		}
		if (!(PINC & _BV(PC2)) && stat_power == 1)
		{				
			PORTA |= _BV(PA3); // red
			PORTA &= ~_BV(PA2); // green

			stat_power = 0;

			/*
				hier muss das rein das nachrichten nochmal ueber 
				CAN rausgesendet werden
			*/

			can_outdata.outdata[1] = POWER;
			can_outdata.outdata[2] = ERROR;

			AvrXSendMessage(&canQueue_out, &can_outdata.mcb);
			//			AvrXWaitMessageAck(&can_outdata.mcb);
		}
		if ((PINC & _BV(PC2)) && stat_power == 0)
		{
			PORTA |= _BV(PA2); // green
			PORTA &= ~_BV(PA3); // red


			stat_power = 1;

			/*
				hier muss das rein das nachrichten nochmal ueber 
				CAN rausgesendet werden
			*/
			can_outdata.outdata[1] = POWER;
			can_outdata.outdata[2] = OK;

			AvrXSendMessage(&canQueue_out, &can_outdata.mcb);
			//			AvrXWaitMessageAck(&can_outdata.mcb);
		}
		AvrXDelay(&taskswitch_timer, 1);
	}
}
