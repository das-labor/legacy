/* -*- Mode: C; tab-width: 2 -*- */
#include <avr/io.h>

#include <avrx-io.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "xcan.h"


TimerControlBlock switchtimer;

AVRX_GCC_TASKDEF(bewegung, 60, 7)
{


	while (1)
  {

	}		
}

