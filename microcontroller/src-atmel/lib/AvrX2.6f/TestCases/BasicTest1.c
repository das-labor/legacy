//#define ENABLE_BIT_DEFINITIONS
//#define _SFR_ASM_COMPAT 1
#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"
#include "hardware.h"
/*
 Basic Tasking Tests #1

 Exercises basic semaphore constructs, Interrupt handling and tasking

 The following API covered:
        AvrXRunTask()
            AvrXResume()        //Indirectly covered
        Epilog()
        IntProlog()
        AvrXIntTestSemaphore()
            AvrXTestSemaphore() //Indirectly covered
        AvrXIntSetSemaphore()
            AvrXSetSemaphore()  //Indirectly covered
        AvrXWaitSemaphore()
        AvrXChangePriority()
        AvrXSelf()

 The code queues tasks on the run queue and the Semaphore:
        in the front
        inserted in the middle
        appended to the end

 The timer code flushes the tasks off the semaphore to start
 the process over again.

 To run this test simply examine the state of the LED bits
 (PORTB) and make sure each task is running regularly.

 20050918 - Added "hardware.h" to project which covers major hardware
 			variations for timer setup, etc.

 20010524 - Make sure the AvrXTestSemaphore() is returning SEM_WAIT
 when multiple tasks are blocked.

 */
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

AVRX_MUTEX(TimerSemaphore);


int main(void)
{
    AvrXSetKernelStack(0);

	MCUCR = _BV(SE);      // Initialize Timer Hardware
	TCNT0 = TCNT0_INIT;
	TCCR0 = TMC8_CK256;
	TIMSK = _BV(TOIE0);    // Enable Timer overflow interrupt

//    outp((1<<SE) , MCUCR);
//    outp(TCNT0_INIT, TCNT0);
//    outp(TMC8_CK256 , TCCR0);
//    outp((1<<TOIE0), TIMSK);

    AvrXRunTask(&task1Tcb);
    AvrXRunTask(&task2Tcb);
    AvrXRunTask(&task3Tcb);

    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
}

AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();
    TCNT0 = TCNT0_INIT;
//    outp(TCNT0_INIT, TCNT0);
    while(AvrXIntTestSemaphore(&TimerSemaphore) == SEM_WAIT)
        AvrXIntSetSemaphore(&TimerSemaphore);   // Flush all waiting tasks
    AvrXIntSetSemaphore(&TimerSemaphore);       // Set Semaphore (short path)
    Epilog();
}

NAKEDFUNC(task1)
{
	DDRB = 0xFF;
	PORTB = 0xFF;
//    outp(0xFF, DDRB);
//    outp(0xFF, PORTB);

    while(1)
    {
        AvrXWaitSemaphore(&TimerSemaphore);
        PORTB ^= _BV(PORTB0);
//        outp(inp(PORTB) ^ 1, PORTB);          // Toggle bits
    }
}

// Task two ping pongs between 1 and 3 priority
NAKEDFUNC(task2)
{
    unsigned char tPri = 3;
    while(1)
    {
        AvrXWaitSemaphore(&TimerSemaphore);
        tPri = AvrXChangePriority(AvrXSelf(), tPri);
        PORTB ^= _BV(PORTB1);
//        outp(inp(PORTB) ^ 2, PORTB);          // Toggle bits
    }
}

// Task three ping pongs between 4 and 0 priority

NAKEDFUNC(task3)
{
    unsigned char tPri = 0;
    while(1)
    {
        AvrXWaitSemaphore(&TimerSemaphore);
        tPri = AvrXChangePriority(AvrXSelf(), tPri);
        PORTB ^= _BV(PORTB2);
//        outp(inp(PORTB) ^ 4, PORTB);          // Toggle bits
    }
}

