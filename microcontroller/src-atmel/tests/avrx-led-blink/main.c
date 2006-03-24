
#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "config.h"


AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Save interrupted context, switch stacks
    TCNT0 = TCNT0_INIT;			// Reload the timer counter
    AvrXTimerHandler();         // Process Timer queue
    Epilog();                   // Restore context of next running task
};


TimerControlBlock   timer1;

AVRX_GCC_TASKDEF(task1, 8, 3)
{
    while (1)
    {
        AvrXStartTimer(&timer1, 800);       // 800 ms delay
        AvrXWaitTimer(&timer1);
        PORTC ^= 0x01;
        AvrXStartTimer(&timer1, 200);       // 200 ms delay
        AvrXWaitTimer(&timer1);
        PORTC ^= 0x01;
    }
}


int main(void)
{
    AvrXSetKernelStack(0);

    MCUCR = 1<<SE;      	// Enable "sleep" mode (low power when idle)
    TCNT0 = TCNT0_INIT;		// Load overflow counter of timer0
    TCCR0 = TMC8_CK256;		// Set Timer0 to CPUCLK/256
    TIMSK = 1<<TOIE0;		// Enable interrupt flag

    DDRC = 0xFF;
	
	// InitSerialIO(UBRR_INIT);    // Initialize USART baud rate generator
    // AvrXRunTask(TCB(Monitor));
    AvrXRunTask(TCB(task1));

    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
};
