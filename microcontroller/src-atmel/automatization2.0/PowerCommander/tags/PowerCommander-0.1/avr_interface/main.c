
#include <avr/io.h>

#include <avrx-io.h>
#include <avrx-signal.h>
//#include <util/delay.h>
#include "avrx.h"               // AvrX System calls/data structures
//#include "serialio.h"           // From AvrX...

#include "config.h"
#include "xcan.h"
#include "xlap.h"
#include "switch.h"


#include "twi_master/twi_master.h"





AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Save interrupted context, switch stacks
    TCNT0 = TCNT0_INIT;			// Reload the timer counter
    AvrXTimerHandler();         // Process Timer queue
    Epilog();                   // Restore context of next running task
};

int main(void)
{
    AvrXSetKernelStack(0);

    MCUCR = 1<<SE;      	// Enable "sleep" mode (low power when idle)
    TCNT0 = TCNT0_INIT;		// Load overflow counter of timer0
    TCCR0 = TMC8_CK256;		// Set Timer0 to CPUCLK/256
    TIMSK = 1<<TOIE0;		// Enable interrupt flag

DDRA |= _BV(PA7) | _BV(PA6);

//	_delay_ms(1000);


/*
** Initiate TWI Master Interface with bitrate of 100000 Hz
*/
	if (!TWIM_Init(100000))
	{
		while (1);
	}
    //InitSerialIO(UBRR_INIT);    // Initialize USART baud rate generator
	xlap_init();

	AvrXRunTask(TCB(laptask));
//	AvrXRunTask(TCB(switchtask));
    
	

    /* Needed for EEPROM access in monitor */
	AvrXSetSemaphore(&EEPromMutex);
	

    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
};
