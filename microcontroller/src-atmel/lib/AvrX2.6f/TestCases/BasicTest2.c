//#define ENABLE_BIT_DEFINITIONS
//#define _SFR_ASM_COMPAT 1
#include <avrx-io.h>
#include "avrx.h"
#include "hardware.h"
/*
 Basic Tasking Tests #2

 Tests each semaphore call (not aliases)

 With Basic Test 1, this test pretty much covers basic tasking and semaphore
 operation.

 Runs through Set/Reset of Semaphores with things waiting and
 not waiting.  Tests actual state of semaphores to verify their action

 Test out other tasking stuff:
    AvrXTerminate
    AvrXResume      // Shouldn't resume a terminated task
    AvrXSuspend
    AvrXResetSemaphore

 NOTE: This test will generate a bunch of casting errors (warnings) when
 compiled.  No big deal...
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

AVRX_MUTEX(Simple);
AVRX_TIMER(Object);
AVRX_MUTEX(Done);


int main(void)
{
    AvrXSetKernelStack(0);

    AvrXRunTask(&task1Tcb);

    Epilog();           // Switch from AvrX Stack to first task
    while(1);
}

NAKEDFUNC(task1)
{
	DDRB = 0xFF;
	PORTB = 0xFF;

//    outp(0xFF, DDRB);     // Initialize LED port
//    outp(0xFF, PORTB);

    while(1)
    {
        AvrXRunTask(&task2Tcb);       // Task blocks on Simple semaphore
        if (AvrXTestSemaphore(&Simple) != SEM_WAIT)
            AvrXHalt();
        if (AvrXTestSemaphore(&Done) != SEM_PEND)
            AvrXHalt();

        AvrXSuspend(&task2Pid);     // Still Queued, but marked for suspension
        AvrXSetSemaphore(&Simple);  // Ok, released, but now suspended
        if (AvrXTestSemaphore(&Done) != SEM_PEND)
            AvrXHalt();
        if (AvrXTestSemaphore(&Simple) != SEM_PEND)
            AvrXHalt();

        AvrXResume(&task2Pid);      // Ok, done and waiting on Simple again.
        if (AvrXTestSemaphore(&Done) != SEM_DONE)
            AvrXHalt();
        if (AvrXTestSemaphore(&Done) != SEM_PEND) // Test resets _DONE to _PEND
            AvrXHalt();
        if (AvrXTestSemaphore(&Simple) != SEM_WAIT)
            AvrXHalt();

        AvrXSuspend(&task2Pid);     // Still Queued, but marked for suspension
        AvrXSetSemaphore(&Simple);  // Now _PEND
        AvrXSetSemaphore(&Simple);  // Now _DONE
        AvrXResetSemaphore(&Simple);// Now _PEND
        if (AvrXTestSemaphore(&Simple) != SEM_PEND)
            AvrXHalt();
        AvrXResume(&task2Pid);      // Now blocked on Simple
        AvrXResetSemaphore(&Done);   // Should have been _DONE
        AvrXResetSemaphore(&Simple); // this should do nothing
        if (AvrXTestSemaphore(&Done) != SEM_PEND)
            AvrXHalt();
        if (AvrXTestSemaphore(&Simple) != SEM_WAIT)
            AvrXHalt();
        AvrXTerminate(&task2Pid);   // Mark task for termination
        AvrXSetSemaphore(&Simple);  // Release from Semaphore so it terminates.
        AvrXResume(&task2Pid);      // This should have no effect

        //
        // Now do it all again but with an Object...
        // use "Timer" since there is no data type "object"
        //

        AvrXRunTask(&task3Tcb);       // Task blocks on Simple semaphore
        if (AvrXTestTimer(&Object) != SEM_WAIT)
            AvrXHalt();
        if (AvrXTestSemaphore(&Done) != SEM_PEND)
            AvrXHalt();

        AvrXSuspend(&task3Pid);     // Still Queued, but marked for suspension
        AvrXSetSemaphore(&Object.semaphore);  // Ok, released, but now suspended
        if (AvrXTestSemaphore(&Done) != SEM_PEND)
            AvrXHalt();
        if (AvrXTestTimer(&Object) != SEM_PEND)
            AvrXHalt();

        AvrXResume(&task3Pid);      // Ok, done and waiting on Object again.
        if (AvrXTestSemaphore(&Done) != SEM_DONE)
            AvrXHalt();
        if (AvrXTestSemaphore(&Done) != SEM_PEND) // Test resets _DONE to _PEND
            AvrXHalt();
        if (AvrXTestTimer(&Object) != SEM_WAIT)
            AvrXHalt();

        AvrXSuspend(&task3Pid);     // Still Queued, but marked for suspension
        AvrXSetSemaphore(&Object.semaphore);  // Now _PEND
        AvrXSetSemaphore(&Object.semaphore);  // Now _DONE
        AvrXResetSemaphore(&Object.semaphore);// Now _PEND
        if (AvrXTestTimer(&Object) != SEM_PEND)
            AvrXHalt();
        AvrXResume(&task3Pid);      // Now blocked on Object
        AvrXResetSemaphore(&Done);   // Should have been _DONE
        AvrXResetSemaphore(&Object.semaphore); // this should do nothing
        if (AvrXTestSemaphore(&Done) != SEM_PEND)
            AvrXHalt();
        if (AvrXTestTimer(&Object) != SEM_WAIT)    // Should be waiting on semaphore
            AvrXHalt();
        AvrXTerminate(&task3Pid);
        AvrXSetSemaphore(&Object.semaphore);
        AvrXResume(&task3Pid);

		PORTB ^= _BV(PORTB0);
//        outp(inp(PORTB) ^ 1, PORTB);          // Toggle bits
    }
}

NAKEDFUNC(task2)
{
    while(1)
    {
        AvrXWaitSemaphore(&Simple);
		PORTB ^= _BV(PORTB1);
//        outp(inp(PORTB) ^ 2, PORTB);          // Toggle bits
        AvrXSetSemaphore(&Done);
    }
}

NAKEDFUNC(task3)
{
    while(1)
    {
        AvrXWaitTimer(&Object);
		PORTB ^= _BV(PORTB2);
//        outp(inp(PORTB) ^ 4, PORTB);          // Toggle bits
        AvrXSetSemaphore(&Done);
    }
}

