/*
    Messages with data included example

    This example shows:

    1. How to extend the message queue element (MessageControlBlock)
        with data for passing between tasks

    2. Uses the AvrX macros to declare interrupt handlers and tasks

*/
//#define ENABLE_BIT_DEFINITIONS
//#define _SFR_ASM_COMPAT 1
#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"
#include "serialio.h"           // From AvrX...
#include "hardware.h"

AVRX_IAR_TASK(Monitor, 0, 20, 0);      // external Debug Monitor
AVRX_GCC_TASK(Monitor, 20, 0);      // external Debug Monitor

TimerControlBlock   MyTimer;    // Declare the control blocks needed for timers

typedef struct                  // Declare a new message type with data
{
    MessageControlBlock mcb;
    unsigned char         switches;
}
MyMessage;

MessageQueue        MyQueue;    // The message queue

/*
 Timer 0 Overflow Interrupt Handler

 This is a prototypical interrupt handler:
  . Switch to the AvrX kernel context - IntProlog()
  . Deal with the interrupt (the timer counter reload in this case)
  . return to the interrupted context - Epilog())
 */
#pragma optimize=z 4
AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Switch to kernel stack/context
    TCNT0 = TCNT0_INIT;
//    outp(TCNT0_INIT, TCNT0);    // Reset timer overflow count
    AvrXTimerHandler();         // Call queue manager
    Epilog();                   // Return to tasks
}
/*
 Task 1 Waits for a message, then updates LED's with the contents.
 */
AVRX_IAR_TASKDEF(task1, 0, 10, 3)
AVRX_GCC_TASKDEF(task1, 10, 3)
{
    MessageControlBlock *p;

    while (1)
    {
        p = AvrXWaitMessage(&MyQueue);
        LED = ((MyMessage*)p)->switches;
//        outp(((MyMessage*)p)->switches, LED);
        AvrXAckMessage(p);
    }
}
/*
 Task 2 Checks switches every 10ms and sends a message whenever SWITCH changes
 */
AVRX_IAR_TASKDEF(task2, 0, 10, 3)
AVRX_GCC_TASKDEF(task2, 10, 3)
{
    static MyMessage SwitchMessage;
    unsigned char current;

	SWITCHP = 0xFF;
//    outp(0xFF, SWITCHP);            // Enable pullups on switch inputs

    while (1)
    {
        AvrXDelay(&MyTimer, 10);             // 10ms delay
//        current = inp(SWITCH);
        current = SWITCH;
        if (current != SwitchMessage.switches)
        {
            SwitchMessage.switches = current;
            AvrXSendMessage(&MyQueue, &SwitchMessage.mcb);
            AvrXWaitMessageAck(&SwitchMessage.mcb);
        }
    }
}

int main(void)                 // Main runs under the AvrX Stack
{
    AvrXSetKernelStack(0);

    MCUCR = 1<<SE;
    TCNT0 = TCNT0_INIT;
    TCCR0 = TMC8_CK256;
    TIMSK = 1<<TOIE0;

    LEDDDR = 0xFF;
    LED   = 0xFF;
/*
    outp((1<<SE) , MCUCR);      // Enable "Sleep" instruction for idle loop

    outp(TCNT0_INIT, TCNT0);
    outp(TMC8_CK256 , TCCR0);   // Set up Timer0 for CLK/256 rate
    outp((1<<TOIE0), TIMSK);    // Enable0 Timer overflow interrupt

    outp(0xFF, LEDDDR);	        // Make LED port output and
    outp(0xFF, LED);              // drive high (LEDs off)
*/
    AvrXRunTask(TCB(task1));	// Init and run our tasks
    AvrXRunTask(TCB(task2));
    AvrXRunTask(TCB(Monitor));

    InitSerialIO(UBRR_INIT);    // Initialize USART baud rate generator

    /* Needed for EEPROM access in monitor */

    AvrXSetSemaphore(&EEPromMutex);

    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
}

