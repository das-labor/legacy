
#include <avr/io.h>

#include "avrx.h"               // AvrX System calls/data structures
#include "AvrXSerialIo.h"

#include "config.h"

#include "borg_hw.h"

#include "reader.h"
#include "panel.h"
#include "client.h"
#include "channel.h"

int main(void)
{
    AvrXSetKernelStack(0);

    MCUCR = 1<<SE;      	// Enable "sleep" mode (low power when idle)
	
	InitSerial0(BAUD(4800));

	AvrXRunTask(TCB(reader));
	//AvrXRunTask(TCB(panel));
	AvrXRunTask(TCB(client));
	AvrXRunTask(TCB(channel));
	
	//init Panel Ports and Timer
	borg_hw_init();
	client_init();
	channel_init();

    /* Needed for EEPROM access in monitor */
	AvrXSetSemaphore(&EEPromMutex);
	
    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
};
