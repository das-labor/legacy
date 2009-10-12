/* -*- Mode: C; tab-width: 2 -*- */
#include <avr/io.h>

#include <avrx-io.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "xcan.h"



TimerControlBlock switchtimer3;


AVRX_GCC_TASKDEF(led, 10, 4)
{

	while (1) // blink blue
	{
		PORTA |= _BV(PA4);
		AvrXDelay(&switchtimer3, 2000);
		PORTA &= ~_BV(PA4);
		AvrXDelay(&switchtimer3, 2000);
	}
}

