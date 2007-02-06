
#include <avr/io.h>

#include "avrx.h"               // AvrX System calls/data structures
#include "AvrXSerialIo.h"

#include "config.h"

#include "borg_hw.h"

#include "reader.h"
#include "panel.h"
#include "client.h"

//AVRX_GCC_TASK(Monitor, 20, 0);          // External Task: Debug Monitor


int main(void)
{
    AvrXSetKernelStack(0);

    MCUCR = 1<<SE;      	// Enable "sleep" mode (low power when idle)
    TCNT0 = TCNT0_INIT;		// Load overflow counter of timer0
    TCCR0 = TMC8_CK256;		// Set Timer0 to CPUCLK/256
    TIMSK = 1<<TOIE0;		// Enable interrupt flag
	
	InitSerial0(BAUD(4800));

	AvrXRunTask(TCB(reader));
	AvrXRunTask(TCB(panel));
	AvrXRunTask(TCB(client));
	AvrXRunTask(TCB(qport));
	
	borg_hw_init();

    /* Needed for EEPROM access in monitor */
	AvrXSetSemaphore(&EEPromMutex);
	

    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
};
