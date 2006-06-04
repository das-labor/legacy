/*
    Timer Message Example

    This example shows:

    1. A task that waits on a message queue for information or a timeout.
    2. Shows how to build messages that contain data
    3. Shows how to include the AvrX Debug monitor

    The task "Switcher" examines the state of the STK switches, connected to
    the port defined in "hardware.h" on a 10ms interval.  When the state of
    the switches change, it sends a message to the second task with the
    current switch state

    The second task "Flasher", starts a special timer that send's itself as a
    message to a message queue when it expires.  The task waits on the
    message queue for either the switch message or the timer message.

    Note: in this example I do not use AvrXCancelTimerMessage.  So the code
    has been arranged so that the timer is restarted only after it has expired
    and been read off the message queue.

    This example also show how to wire in the debug monitor for AvrX.  If the
    kernel library has been compiled with the symbol SINGLESTEPSUPPORT defined
    the monitor will allow you to halt a task and single step it through the
    code

    To remove the monitor code, comment out the AVRX_TASK() definition, the
    call AvrXRunTask and the call to InitSerialIO();
*/
//#define ENABLE_BIT_DEFINITIONS
//#define _SFR_ASM_COMPAT 1
#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures
#include "serialio.h"           // From AvrX...
#include "hardware.h"           // Sample code hardware defines

// Declare all my AvrX data structures

AVRX_IAR_TASK(Monitor, 0, 20, 0);       // External Task: Debug Monitor
AVRX_GCC_TASK(Monitor, 20, 0);          // External Task: Debug Monitor

TimerControlBlock SwTimer;      // Plain vanilla timer
TimerMessageBlock Timer;        // TimerMessage timer (just a timer with a
                                // message queue pointer appended)

// This is how you build up a message element that transports data

typedef struct MyMessage
{
    MessageControlBlock mcb;
    unsigned char data;
}
*pMyMessage, MyMessage;

MyMessage SwitchMsg;            // Declare the actual message
MessageQueue MyQueue;           // The message queue itself

/*
    Tasks and AvrX Interrupt handlers exist in their own context (registers
    & stack) hence don't need to save anything and never return, hence "naked"
*/
#pragma optimize=z 4
AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Save interrupted context, switch stacks
    TCNT0 = TCNT0_INIT;
//    outp(TCNT0_INIT, TCNT0);    // Reload the timer counter
    AvrXTimerHandler();         // Process Timer queue
    Epilog();                   // Restore context of next running task
}


AVRX_IAR_TASKDEF(flasher, 4, 16, 2)
AVRX_GCC_TASKDEF(flasher, 20, 2)    // Note I added r_stack and c_stack!
{
    pMyMessage pMsg;
    unsigned char led = 0;

	LEDDDR = 0xFF;
//    outp(0xFF, LEDDDR);         // Initialize LED port to outputs

    AvrXStartTimerMessage(&Timer, 150, &MyQueue);

    while(1)
    {
    	LED = ~led;
//	    outp(~led, LED);
        pMsg = (pMyMessage)AvrXWaitMessage(&MyQueue);
        if (pMsg == &SwitchMsg)
        {
            led ^= ~(pMsg->data);
            AvrXAckMessage(&pMsg->mcb);
        }
        else if ((pTimerMessageBlock)pMsg == &Timer)
        {
#ifdef __IAR_SYSTEMS_ICC__
            led = led+led+((led & 0x80)?1:0);
#else
            asm("rol %0\n"\
                "\tbrcc .+2\n"\
                "\tinc %0\n"  ::"r" (led));
#endif
            AvrXStartTimerMessage(&Timer, 150, &MyQueue);
        }
        else
        {
            AvrXHalt();
        }
    }
}

AVRX_IAR_TASKDEF(switcher, 4, 6, 3)
AVRX_GCC_TASKDEF(switcher, 10, 3)
{
	SWITCHP = 0xFF;
//    outp(0xFF, SWITCHP);            // Enable pullups on switch inputs
    while(1)
    {
        AvrXDelay(&SwTimer, 10);        // Delay 10 milliseconds
//        if (SwitchMsg.data != inp(SWITCH)) // On change, send message
        if (SwitchMsg.data != SWITCH) // On change, send message
        {
//            SwitchMsg.data = inp(SWITCH);
            SwitchMsg.data = SWITCH;
            AvrXSendMessage(&MyQueue, &SwitchMsg.mcb);
            AvrXWaitMessageAck(&SwitchMsg.mcb);
        }
    }
}

int main(void)
{
    AvrXSetKernelStack(0);

    MCUCR = 1<<SE;
    TCNT0 = TCNT0_INIT;
    TCCR0 = TMC8_CK256;
    TIMSK = 1<<TOIE0;
/*
    outp((1<<SE) , MCUCR);      // Enable "sleep" mode (low power when idle)
    outp(TCNT0_INIT, TCNT0);    // Load overflow counter of timer0
    outp(TMC8_CK256 , TCCR0);   // Set Timer0 to CPUCLK/256
    outp((1<<TOIE0), TIMSK);    // Enable interrupt flag
*/
    InitSerialIO(UBRR_INIT);    // Initialize USART baud rate generator

    AvrXRunTask(TCB(flasher));
    AvrXRunTask(TCB(switcher));
    AvrXRunTask(TCB(Monitor));

    /* Needed for EEPROM access in monitor */

    AvrXSetSemaphore(&EEPromMutex);

    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
}

