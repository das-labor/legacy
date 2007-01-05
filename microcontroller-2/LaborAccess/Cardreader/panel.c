

#include "panel.h"
#include "AvrXSerialIo.h"
#include <avr/pgmspace.h>




TimerControlBlock   myTimer;

AVRX_GCC_TASKDEF(panel, 50, 4)
{
    InitSerial0(BAUD(9600));
   // fdevopen(put_char0, get_c0, 0);		// Set up standard I/O
	
	while(1){
		console();
		AvrXDelay(&myTimer, 10);
	}
	
}
