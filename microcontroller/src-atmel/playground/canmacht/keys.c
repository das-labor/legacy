
#include <avr/io.h>
#include <string.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "xcan.h"
#include "xlap.h"
#include "keys.h"


TimerControlBlock switchtimer;
uint8_t mprog = 1;

AVRX_GCC_TASKDEF(keys, 20, 4) {

	while(1) {

		AvrXDelay(&switchtimer, 20);
		
	

	}
}

