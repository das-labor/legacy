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


AVRX_GCC_TASKDEF(switch_vortrag, 60, 7)
{
	uint8_t stat_vortrag=0; // nur an oder aus 
	static t_i2cMessage_out i2c_outdata;

	while(1)
		{
			if ((!(PINA & _BV(PA2))) && (stat_vortrag == 1))
				{
					i2c_outdata.outdata[0]=C_SW;
					i2c_outdata.outdata[1]=SWL_TAFEL;
					i2c_outdata.outdata[2]=F_SW_OFF;
					i2c_outdata.outdata[3]=0x00;

					AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
					AvrXWaitMessageAck(&i2c_outdata.mcb);

					i2c_outdata.outdata[0]=C_SW;
					i2c_outdata.outdata[1]=SWL_BEAMER;
					i2c_outdata.outdata[2]=F_SW_OFF;
					i2c_outdata.outdata[3]=0x00;

					AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
					AvrXWaitMessageAck(&i2c_outdata.mcb);

					i2c_outdata.outdata[0]=C_SW;
					i2c_outdata.outdata[1]=SWL_SCHRANK;
					i2c_outdata.outdata[2]=F_SW_OFF;
					i2c_outdata.outdata[3]=0x00;

					AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
					AvrXWaitMessageAck(&i2c_outdata.mcb);

					i2c_outdata.outdata[0]=C_SW;
					i2c_outdata.outdata[1]=SWL_FLIPPER;
					i2c_outdata.outdata[2]=F_SW_OFF;
					i2c_outdata.outdata[3]=0x00;

					AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
					AvrXWaitMessageAck(&i2c_outdata.mcb);

					i2c_outdata.outdata[0]=C_SW;
					i2c_outdata.outdata[1]=SWL_VORTRAG;
					i2c_outdata.outdata[2]=F_SW_OFF;
					i2c_outdata.outdata[3]=0x00;

					AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
					AvrXWaitMessageAck(&i2c_outdata.mcb);

					AvrXDelay(&switch_timer_vortrag, 3000);
					stat_vortrag=0;
				}
			if ((!(PINA & _BV(PA2))) && stat_vortrag == 0)
				{
					i2c_outdata.outdata[0]=C_SW;
					i2c_outdata.outdata[1]=SWL_TAFEL;
					i2c_outdata.outdata[2]=F_SW_ON;
					i2c_outdata.outdata[3]=0x00;

					AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
					AvrXWaitMessageAck(&i2c_outdata.mcb);

					i2c_outdata.outdata[0]=C_SW;
					i2c_outdata.outdata[1]=SWL_BEAMER;
					i2c_outdata.outdata[2]=F_SW_ON;
					i2c_outdata.outdata[3]=0x00;

					AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
					AvrXWaitMessageAck(&i2c_outdata.mcb);

					i2c_outdata.outdata[0]=C_SW;
					i2c_outdata.outdata[1]=SWL_SCHRANK;
					i2c_outdata.outdata[2]=F_SW_ON;
					i2c_outdata.outdata[3]=0x00;

					AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
					AvrXWaitMessageAck(&i2c_outdata.mcb);

					i2c_outdata.outdata[0]=C_SW;
					i2c_outdata.outdata[1]=SWL_FLIPPER;
					i2c_outdata.outdata[2]=F_SW_ON;
					i2c_outdata.outdata[3]=0x00;

					AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
					AvrXWaitMessageAck(&i2c_outdata.mcb);

					i2c_outdata.outdata[0]=C_SW;
					i2c_outdata.outdata[1]=SWL_VORTRAG;
					i2c_outdata.outdata[2]=F_SW_ON;
					i2c_outdata.outdata[3]=0x00;

					AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
					AvrXWaitMessageAck(&i2c_outdata.mcb);

					AvrXDelay(&switch_timer_vortrag, 3000);
					stat_vortrag=1;
				}
				AvrXDelay(&switch_timer_vortrag, 1);
		}

}

AVRX_GCC_TASKDEF(switch_lounge, 60, 7)
{
	uint8_t stat_lounge=0; // nur an oder aus 
	static t_i2cMessage_out i2c_outdata;
	while(1)
		{
			if ((!(PINB & _BV(PB0))) && stat_lounge == 1)
				{
					i2c_outdata.outdata[0]=C_SW;
					i2c_outdata.outdata[1]=SWL_LOUNGE;
					i2c_outdata.outdata[2]=F_SW_OFF;
					i2c_outdata.outdata[3]=0x00;

					AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
					AvrXWaitMessageAck(&i2c_outdata.mcb);

					AvrXDelay(&switch_timer_lounge, 3000);
					stat_lounge=0;
				}
			if ((!(PINB & _BV(PB0))) && stat_lounge == 0)
				{
					i2c_outdata.outdata[0]=C_SW;
					i2c_outdata.outdata[1]=SWL_LOUNGE;
					i2c_outdata.outdata[2]=F_SW_ON;
					i2c_outdata.outdata[3]=0x00;

					AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
					AvrXWaitMessageAck(&i2c_outdata.mcb);

					AvrXDelay(&switch_timer_lounge, 3000);
					
					stat_lounge=1;
				}
				AvrXDelay(&switch_timer_lounge, 1);
		}
	
}



AVRX_GCC_TASKDEF(switchtask, 60, 7)
{
	static t_i2cMessage_out i2c_outdata;


	uint8_t stat_haupt = 0;

	while (1)
  	{
			if (!(PINA & _BV(PA0)) && stat_haupt == 1)
				{
					
					i2c_outdata.outdata[0]=C_VIRT;
					i2c_outdata.outdata[1]=VIRT_POWER;
					i2c_outdata.outdata[2]=F_SW_OFF;
					i2c_outdata.outdata[3]=0x00;

					stat_haupt = 0;
					AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
					AvrXWaitMessageAck(&i2c_outdata.mcb);

					AvrXDelay(&switchtimer, 1000);
				}
			if ((PINA & _BV(PA0)) && stat_haupt == 0)
				{
					i2c_outdata.outdata[0]=C_VIRT;
					i2c_outdata.outdata[1]=VIRT_POWER;
					i2c_outdata.outdata[2]=F_SW_ON;
					i2c_outdata.outdata[3]=0x00;

					stat_haupt = 1;
					AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
					AvrXWaitMessageAck(&i2c_outdata.mcb);

					AvrXDelay(&switchtimer, 1000);

				}
			AvrXDelay(&switchtimer, 1);
  	}
}
