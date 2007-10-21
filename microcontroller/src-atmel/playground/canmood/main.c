
#include <avr/io.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures
//#include "serialio.h"           // From AvrX...

#include "config.h"
#include "xcan.h"
#include "xlap.h"
#include "pwm.h"
#include "static_scripts.h"
//#include "mood.h"
#include "testscript.h"

//AVRX_GCC_TASK(Monitor, 20, 0);          // External Task: Debug Monitor


AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Save interrupted context, switch stacks
    TCNT0 = TCNT0_INIT;			// Reload the timer counter
    AvrXTimerHandler();         // Process Timer queue
    Epilog();                   // Restore context of next running task
}

int main(void) {
	AvrXSetKernelStack(0);

	MCUCR = 1<<SE;      	// Enable "sleep" mode (low power when idle)
	TCNT0 = TCNT0_INIT;		// Load overflow counter of timer0
	TCCR0 = TMC8_CK256;		// Set Timer0 to CPUCLK/256
	TIMSK = 1<<TOIE0;		// Enable interrupt flag

    //InitSerialIO(UBRR_INIT);    // Initialize USART baud rate generator

	init_pwm();
	xlap_init();
	init_script_threads();
	script_threads[0].handler.execute = &memory_handler_flash;
	script_threads[0].handler.position = (uint16_t) &colorchange_red;
	script_threads[0].flags.disabled = 0;
    //AvrXRunTask(TCB(Monitor));
	AvrXRunTask(TCB(laptask));
	AvrXRunTask(TCB(pwmtask));
	//AvrXRunTask(TCB(mood));

    /* Needed for EEPROM access in monitor */
	AvrXSetSemaphore(&EEPromMutex);

	Epilog();                   // Switch from AvrX Stack to first task
	while(1);
}
