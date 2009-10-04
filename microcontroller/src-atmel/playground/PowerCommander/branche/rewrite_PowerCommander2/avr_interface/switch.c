/* -*- Mode: C; tab-width: 2 -*- */
#include <avr/io.h>

#include <avrx-io.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "xcan.h"

#include "twi_master/twi_master.h"
#include "queue.h"


TimerControlBlock switchtimer;


AVRX_GCC_TASKDEF(switchtask, 60, 7)
{
	static t_i2cMessage_out i2c_outdata;


	uint8_t stat_haupt = 0;

	while (1)
  	{
			if (!(PINA & _BV(PA0)) && stat_haupt == 1)
				{
					
					i2c_outdata.outdata[0]=0x02;
					i2c_outdata.outdata[1]=0x00;
					i2c_outdata.outdata[2]=0x00;
					i2c_outdata.outdata[3]=0x00;

					stat_haupt = 0;
					AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
					AvrXWaitMessageAck(&i2c_outdata.mcb);

					AvrXDelay(&switchtimer, 1000);
				}
			if ((PINA & _BV(PA0)) && stat_haupt == 0)
				{
					i2c_outdata.outdata[0]=0x02;
					i2c_outdata.outdata[1]=0x00;
					i2c_outdata.outdata[2]=0x01;
					i2c_outdata.outdata[3]=0x00;

					stat_haupt = 1;
					AvrXSendMessage(&i2cQueue_out, &i2c_outdata.mcb);
					AvrXWaitMessageAck(&i2c_outdata.mcb);

					AvrXDelay(&switchtimer, 1000);

				}

  	}
}
