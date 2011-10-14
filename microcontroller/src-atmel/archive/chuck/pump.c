#include <avr/io.h>
#include <string.h>

#include "avrx.h"               // AvrX System calls/data structures

#include "fifo.h"
#include "xcan.h"
#include "xlap.h"

#define PUMP_C

#include "rf.h"
#include "pump.h"


TimerControlBlock timer;

AVRX_DECL_FIFO(pumpstat, 1);

AVRX_GCC_TASKDEF(pumptask, 100, 2)
{
	uint8_t x = 0x00;

    	PUMP_DDR |= PUMP_PIN;
	AVRX_INIT_FIFO (pumpstat);

	while(1)
	{
		x = AvrXWaitPullFifo(pumpstat);
		if ((x & STAT_PUMP_ENABLE))
			PUMP_PORT |= (PUMP_PIN);

		if (!(x & STAT_PUMP_ENABLE))
			PUMP_PORT &= ~(PUMP_PIN);
		
		for(x=0;x<CONF_PUMP_TICKS;x++)
		{
			AvrXDelay(&timer, CONF_PUMPTIME);
		}
	}
}
