/* -*- Mode: C; tab-width: 2 -*- */
#include <avr/io.h>

#include <avrx-io.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "xcan.h"

#include "config.h"

TimerControlBlock switchtimer;

AVRX_GCC_TASKDEF(bewegung, 60, 7)
{

	int i, j, k, x = 0;
	while (1)
  {
		for (k = 0; k < LAMPS; k++)
		{
			for (i = 0; i < 3; i++)
			{
				x++;
				for (j = 0; j < 12; j++)
				{
					if (((i == 0) && (x & 1)) || ((i == 1) && (x & 2)) || ((i == 2) && (x & 4)))
						PORTD &= ~_BV(DATA);
					else
						PORTD |= _BV(DATA);
					PORTD |= _BV(CLK);
//					_delay_us(2);
					PORTD &= ~_BV(CLK);
				}
				x *= 3;
			}
		}
	
		// Trigger uebernehmen
		AvrXDelay(&switchtimer, 10);
		for (j = 0; j < 5; j++)
		{
			PORTD |= _BV(DATA);
//			_delay_us(2);
			PORTD &= ~_BV(DATA);
		}
		AvrXDelay(&switchtimer, 1000);
	}		
}

