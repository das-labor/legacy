//#define ENABLE_BIT_DEFINITIONS
//#define _SFR_ASM_COMPAT 1
#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"
#include "hardware.h"
/*
 Basic Tasking Tests #3

 Exercises Timer Queue operations

 The following API covered:
    AvrXStartTimer
    AvrXCancelTimer
    AvrXTimerHandler
    AvrXDelay

 */
#define TLE(A) (A)     /* used to need little endien values, no longer */

/*
   One group of task def's will be dropped depending upon which
   compiler is being used.
*/
AVRX_GCC_TASK(task1, 20, 3);
AVRX_GCC_TASK(task2, 20, 2);
AVRX_GCC_TASK(task3, 20, 2);

AVRX_IAR_TASK(task1, 14, 6, 3);
AVRX_IAR_TASK(task2, 14, 6, 2);
AVRX_IAR_TASK(task3, 14, 6, 2);

TimerControlBlock timer1, timer2, timer3, timer4;
Mutex TimerSemaphore;

int main(void)
{
    AvrXSetKernelStack(0);

	MCUCR = _BV(SE);
//    outp((1<<SE) , MCUCR);      // Enable "Sleep" instruction

    AvrXRunTask(&task1Tcb);
    AvrXInitTask(&task2Tcb);    // Init, but don't run until end of Task1
    AvrXInitTask(&task3Tcb);

    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
}

AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();
    TCNT0 = TCNT0_INIT;
//    outp(TCNT0_INIT, TCNT0);
    AvrXSetSemaphore(&TimerSemaphore);  // Signal test task of tick
    AvrXTimerHandler();
    Epilog();
}

NAKEDFUNC(task1)
{
	PORTB = 0xFF;
	DDRB = 0xFF;
//    outp(0xFF, DDRB);
//    outp(0xFF, PORTB);

    AvrXStartTimer(&timer1, 4); // Add timer to empty queue
    AvrXStartTimer(&timer2, 2); // Insert timer in front
    if ((timer2.count != TLE(2)) ||
        (timer1.count != TLE(2)))
        AvrXHalt();
    AvrXStartTimer(&timer3, 5); // Append timer on tail
    if ((timer3.count != TLE(1)) ||
        (timer2.count != TLE(2)) ||
        (timer1.count != TLE(2)))
        AvrXHalt();
    AvrXCancelTimer(&timer3);   // Cancel last timer
    if ((timer2.count != TLE(2)) ||
        (timer1.count != TLE(2)))
        AvrXHalt();
    AvrXStartTimer(&timer3, 5);
    AvrXCancelTimer(&timer2);   // Cancel first timer
    if ((timer3.count != TLE(1)) ||
        (timer1.count != TLE(4)))
        AvrXHalt();
    AvrXStartTimer(&timer2, 2);
    AvrXCancelTimer(&timer1);   // Cancel middle timer
    if ((timer3.count != TLE(3)) ||
        (timer2.count != TLE(2)))
        AvrXHalt();
    AvrXStartTimer(&timer1, 5); // Timer 1 & 3 are same values
    if ((timer1.count != TLE(0)) ||
        (timer3.count != TLE(3)) ||
        (timer2.count != TLE(2)))
        AvrXHalt();
    AvrXStartTimer(&timer4, 6); // Append another timer
    if ((timer4.count != TLE(1)) ||
        (timer1.count != TLE(0)) ||
        (timer3.count != TLE(3)) ||
        (timer2.count != TLE(2)))
        AvrXHalt();

    // Enable clock hardware and observe timers being processed

	TCNT0 = TCNT0_INIT;
	TCCR0 = TMC8_CK256;
	TIMSK = _BV(TOIE0);
//    outp(TCNT0_INIT, TCNT0);
//    outp(TMC8_CK256 , TCCR0);
//    outp((1<<TOIE0), TIMSK);    // Enable Timer overflow interrupt

    AvrXWaitSemaphore(&TimerSemaphore);
    if ((timer4.count != TLE(1)) ||
        (timer1.count != TLE(0)) ||
        (timer3.count != TLE(3)) ||
        (timer2.count != TLE(1)))   // Timer2 dec by one
        AvrXHalt();
    AvrXWaitTimer(&timer2);     // Wait two more ticks
    if ((timer4.count != TLE(1)) ||
        (timer1.count != TLE(0)) ||
        (timer3.count != TLE(3)))
        AvrXHalt();
    AvrXWaitTimer(&timer1);
    AvrXWaitTimer(&timer3);     // Timer 1 & 3 expire together

    if ((unsigned)timer2.next | (unsigned)timer3.next)  // Check both dequeued.
        AvrXHalt();
    if (timer4.count != TLE(1))      // Timer 4 left to go.
        AvrXHalt();

    AvrXWaitTimer(&timer4);

    // Ok, static queue operations seem Ok.  Now try
    // hammering the queue while working on active timers.

    AvrXResume(&task2Pid);
    AvrXResume(&task3Pid);

    while(1)
    {
        AvrXStartTimer(&timer4, 3);
        AvrXCancelTimer(&timer4);
        PORTB ^= _BV(PORTB0);
//        outp(inp(PORTB) ^ 1, PORTB);          // Toggle bits
    }
}

NAKEDFUNC(task2)
{
    while(1)
    {
        AvrXStartTimer(&timer1, 5);
        AvrXDelay(&timer2, 3);
        AvrXCancelTimer(&timer1);
        if (AvrXTestTimer(&timer1) != SEM_DONE)    // Canceling sets semaphore..
            AvrXHalt();
        PORTB ^= _BV(PORTB1);
//        outp(inp(PORTB) ^ 2, PORTB);            // Toggle bits
    }
}

NAKEDFUNC(task3)
{
    while(1)
    {
        AvrXDelay(&timer3, 3);
        PORTB ^= _BV(PORTB2);
//        outp(inp(PORTB) ^ 4, PORTB);          // Toggle bits
    }
}

