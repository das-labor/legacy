/*
	Example code for setting up and using timers

	Two tasks each waiting on a timer, flashing LED's on
	the evaluation board.  See "hardware.h" for LED definitions

    The kernel Stack is defined by the IAR linker file, or Top or SRAM
    in GCC (can be altered in the GCC link files, or defined symbol)
*/
//#define _SFR_ASM_COMPAT 1
#include <avrx-io.h>
#include <avrx-signal.h>

#include <avr/pgmspace.h>

#include "avrx.h"
#include "serialio.h"       // From AvrX...
#include "hardware.h"

AVRX_IAR_TASK(Monitor, 0, 20, 0);       // External Task: Debug Monitor
AVRX_GCC_TASK(Monitor, 20, 0);          // External Task: Debug Monitor

TimerControlBlock   timer1,             // Declare the control blocks needed for timers
                    timer2;
/*
 Timer 0 Overflow Interrupt Handler

 Prototypical Interrupt handler:
 . Switch to kernel context
 . handle interrupt
 . switch back to interrupted context.
 */

#pragma optimize=z 4
AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Switch to kernel stack/context
    TCNT0 = TCNT0_INIT;
//    outp(TCNT0_INIT, TCNT0);
    AvrXTimerHandler();         // Call Time queue manager
    Epilog();                   // Return to tasks
}

/*
 Task 1 simply flashes the light off for 1/5 second and then on for 4/5th
 for a 1 second cycle time.
 */
AVRX_IAR_TASKDEF(task1, 0, 6, 3)
AVRX_GCC_TASKDEF(task1, 8, 3)
{
    while (1)
    {
        AvrXStartTimer(&timer1, 800);       // 800 ms delay
        AvrXWaitTimer(&timer1);
        LED = LED ^ 0x01;
//        outp(inp(LED) ^ 0x01, LED);
        AvrXStartTimer(&timer1, 200);       // 200 ms delay
        AvrXWaitTimer(&timer1);
        LED = LED ^ 0x01;
//        outp(inp(LED) ^ 0x01, LED);
    }
}
/*
 Task 2 cycles the light on/off over 4 seconds.  It uses a simplified
 form of the delay API
 */
AVRX_IAR_TASKDEF(task2, 0, 6, 2)
AVRX_GCC_TASKDEF(task2, 8, 2)
{
    while (1)
    {
        AvrXDelay(&timer2, 2000);           // 2 second delay
        LED = LED ^ 0x02;
//        outp((inp(LED) ^ 0x02), LED);
    }
}

int main(void)                 // Main runs under the AvrX Stack
{
    AvrXSetKernelStack(0);
/*
    outp((1<<SE), MCUCR);       // Enable "Sleep" instruction
    outp(TCNT0_INIT, TCNT0);
    outp(TMC8_CK256, TCCR0);    // Set up Timer0 for CLK/256 rate
    outp((1<<TOIE0), TIMSK);     // Enable Timer0 overflow interrupt

    outp(0xFF, LEDDDR);         // Make LED output and
    outp(0xFF, LED);            // drive high (LEDs off)
*/
    MCUCR = 1<<SE;
    TCNT0 = TCNT0_INIT;
    TCCR0 = TMC8_CK256;
    TIMSK = 1<<TOIE0;

    LEDDDR = 0xFF;
    LED   = 0xFF;

    AvrXRunTask(TCB(task1));
    AvrXRunTask(TCB(task2));
    AvrXRunTask(TCB(Monitor));

    InitSerialIO(UBRR_INIT);    // Initialize USART baud rate generator
	
	Epilog();                   // Switch from AvrX Stack to first task
    while(1);
}

