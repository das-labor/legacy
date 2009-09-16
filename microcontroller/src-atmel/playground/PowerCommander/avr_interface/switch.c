#include <avr/io.h>

#include <avrx-io.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "xcan.h"

#define PIN_SWITCH PIND
#define BIT_SWITCH PD0

TimerControlBlock switchtimer;


AVRX_GCC_TASKDEF(switchtask, 60, 8)
{


  	while (1)
  	{
       	    can_message_t msg = {0x02, 0x00, 0x00, 0x00};
	    msg.dlc = 2;
            
			AvrXDelay(&switchtimer, 1000);
			msg.data[0] = 0x00;
			msg.data[1] = 0x00; //NULLBYTE
			can_put(&msg);
		
  	}
}
