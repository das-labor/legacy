//#define ENABLE_BIT_DEFINITIONS
//#define _SFR_ASM_COMPAT 1
#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"
#include "hardware.h"
/*
 Basic Tasking Tests #4

 Exercises Message API

 The following API covered:
    AvrXSendMessage
    AvrXRecvMessage
    AvrXWaitMessage
    AvrXAckMessage
    AvrXTestMessageAck
    AvrXWaitMessageAck

To run: Step through Task 1.  Set break points on each of the "AvrXAckMessage()"
calls and in the _Overflow0_ routine and run through making sure each is called.
The sequence is not important as long as all four get called regularly.

Alternatively run this on hardware and check the bits on PortB that are toggled
with each sucessfull run.
 */

#define TCNT0_INIT (0xFF-CPUCLK/256/TICKRATE)
#define TMC8_CK256 (1<<CS02)

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

Mutex Sync2, Sync3;     // Synchronization semaphores between task1 and 2&3
MessageControlBlock Message1, Message2, Message3;
MessageQueue TestQueue;

AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();
    TCNT0 = TCNT0_INIT;
//    outp(TCNT0_INIT, TCNT0);

    // This relies on us "priming the queue" in the main task

    if (AvrXTestMessageAck(&Message1) == SEM_DONE)
        AvrXIntSendMessage(&TestQueue, &Message1);
    Epilog();
}

int main(void)
{
    AvrXSetKernelStack(0);

    AvrXRunTask(&task1Tcb);

    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
}

NAKEDFUNC(task1)
{
	DDRB = 0xFF;
	PORTB = 0xFF;
//    outp(0xFF, DDRB);
//    outp(0xFF, PORTB);

    // Verify initial condition of empty

    if (AvrXRecvMessage(&TestQueue) != NOMESSAGE)
        AvrXHalt();

    // Send and receive a message

    AvrXSendMessage(&TestQueue, &Message1);
    if (AvrXRecvMessage(&TestQueue) != &Message1)
        AvrXHalt();

    // Check to see that queue is empty

    if (AvrXRecvMessage(&TestQueue) != NOMESSAGE)
        AvrXHalt();

    // Acknowledge the message

    AvrXAckMessage(&Message1);
    if (AvrXTestMessageAck(&Message1) != SEM_DONE)
        AvrXHalt();

    // Queue up two messages

    AvrXSendMessage(&TestQueue, &Message1);
    AvrXSendMessage(&TestQueue, &Message2);
    if (AvrXWaitMessage(&TestQueue) != &Message1)
        AvrXHalt();
    AvrXAckMessage(&Message1);
    AvrXWaitMessageAck(&Message1);
    AvrXWaitMessage(&TestQueue);

    // Funky test to verify message queue is empty

    if ((unsigned)TestQueue.message || (unsigned)TestQueue.pid)
        AvrXHalt();

    // Ok, basic static queue mechanism checked out.  Now for
    // some dynamic stuff.
    // Task2 and Task3 jam messages onto the queue.  Task 1
    // runs low priority flushing the queue.

    AvrXRunTask(&task2Tcb);
    AvrXRunTask(&task3Tcb);

	TCNT0 = TCNT0_INIT;
	TCCR0 = TMC8_CK256;
	TIMSK = _BV(TOIE0);
//    outp(TCNT0_INIT, TCNT0);
//    outp(TMC8_CK256 , TCCR0);
//    outp((1<<TOIE0), TIMSK);    // Enable Timer overflow interrupt

    AvrXIntSendMessage(&TestQueue, &Message1); //Prime the interrupt queue

    while(1)
    {
        pMessageControlBlock msg;

        msg = AvrXWaitMessage(&TestQueue);
        AvrXAckMessage(msg);
        if ((msg = AvrXRecvMessage(&TestQueue)) != NOMESSAGE)
        {
            AvrXAckMessage(msg);
	        PORTB ^= _BV(PORTB0);
//            outp(inp(PORTB) ^ 1, PORTB);          // Toggle bits
        }
    }
}

NAKEDFUNC(task2)
{
    while(1)
    {
        AvrXSendMessage(&TestQueue, &Message2);
        AvrXWaitMessageAck(&Message2);
        PORTB ^= _BV(PORTB1);
//        outp(inp(PORTB) ^ 2, PORTB);          // Toggle bits
    }
}

NAKEDFUNC(task3)
{
    while(1)
    {
        AvrXSendMessage(&TestQueue, &Message3);
        AvrXWaitMessageAck(&Message3);
        PORTB ^= _BV(PORTB2);
//        outp(inp(PORTB) ^ 4, PORTB);          // Toggle bits
    }
}

