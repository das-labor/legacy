
#include <avr/io.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures
//#include "serialio.h"           // From AvrX...

#include "config.h"
#include "xcan.h"
#include "xlap.h"
#include "keys.h"
#include "menu.h"
#include "s1d1565.h"

//AVRX_GCC_TASK(Monitor, 20, 0);          // External Task: Debug Monitor


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

//	PORTD = 0xff;
    //InitSerialIO(UBRR_INIT);    // Initialize USART baud rate generator
	dispInit(); // Display initialisierung
	//xlap_init();
	PORTA = 0xff; // Tastenport
	DDRA = 0x00;
	
	//AvrXRunTask(TCB(laptask));
	//AvrXRunTask(TCB(keys));
	AvrXRunTask(TCB(menu));

    /* Needed for EEPROM access in monitor */
	AvrXSetSemaphore(&EEPromMutex);
	

    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
};
