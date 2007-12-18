
#include <avr/io.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures
#include "serialio.h"           // From AvrX...

#include "config.h"
#include "xcan.h"
#include "xlap.h"
#include "rf.h"
#include "switch.h"
#include "lamp_and_switch.h"

//AVRX_GCC_TASK(Monitor, 20, 0);          // External Task: Debug Monitor



void my_init (void) __attribute__ ((naked)) \
    __attribute__ ((section (".init2")));

void
my_init (void)
{
	uint16_t x;
	for(x=0x60;x<0x460;x++)
		*(uint8_t*)x = 0x55;
}


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

	DDRC = 0xFF;
	
    //InitSerialIO(UBRR_INIT);    // Initialize USART baud rate generator
	xlap_init();
	rf_init();
	lamp_and_switch_init();
    //AvrXRunTask(TCB(Monitor));
	AvrXRunTask(TCB(laptask));
	//AvrXRunTask(TCB(rfrxtask));
	AvrXRunTask(TCB(rftxtask));
	AvrXRunTask(TCB(switchtask));
	AvrXRunTask(TCB(lapd_task));
	

    /* Needed for EEPROM access in monitor */
	AvrXSetSemaphore(&EEPromMutex);
	

    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
};
