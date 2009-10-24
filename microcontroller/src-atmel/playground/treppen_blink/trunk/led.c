/* -*- Mode: C; tab-width: 2 -*- */
#include <avr/io.h>

#include <avrx-io.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "xcan.h"



TimerControlBlock switchtimer3;


AVRX_GCC_TASKDEF(led, 100, 4)
{
	while (1)
	{

	}
}
