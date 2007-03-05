
#include <avr/io.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures
#include "serialio.h"           // From AvrX...

#include "config.h"
#include "xcan.h"
#include "xlap.h"
#include "radio.h"

//AVRX_GCC_TASK(Monitor, 20, 0);          // External Task: Debug Monitor


AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Save interrupted context, switch stacks
    TCNT0 = TCNT0_INIT;		// Reload the timer counter

    radio_tick();

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

    DDRC = 0xFF;
    DDRD |= _BV(PD0);           // Control pin for rf module
    DDRB = 0x00;                // ??

    radio_txcount = 0;          // Tor-Open commands to be send
	
    // Initialize LAP
    xlap_init();
    AvrXRunTask(TCB(laptask));
    AvrXRunTask(TCB(sesame));

    /* Needed for EEPROM access in monitor */
    AvrXSetSemaphore(&EEPromMutex);
	
    //AvrXRunTask(TCB(Monitor));
    //InitSerialIO(UBRR_INIT);    // Initialize USART baud rate generator

    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
};
