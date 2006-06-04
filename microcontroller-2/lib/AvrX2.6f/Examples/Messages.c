/*
	Example of using different MessageControlBlocks as
	specific messages.  In this case "Switch Up" and "Switch Down"

	Pay attention to the AvrXStack defined in the makefile on the
	linker/loader command line.  In general this should match the
	stack passed to main() (end of ram).

	The task definitions in this example have 20 extra bytes for
	the task stack.	Typically only one or two extra words of stack
	are needed for AvrX calls.  All other is for user code.
*/
//#define ENABLE_BIT_DEFINITIONS
//#define _SFR_ASM_COMPAT 1
#include <avrx-io.h>			// This defers to avr/io.h for GCC
#include <avrx-signal.h>		// This defers to avr/signal.h for GCC
#include "Avrx.h"
#include "serialio.h"           // From AvrX...
#include "hardware.h"

#include <avr/pgmspace.h>

AVRX_IAR_TASK(Monitor, 0, 20, 0);
AVRX_GCC_TASK(Monitor, 40, 0);//20

TimerControlBlock   MyTimer;    // Declare the control blocks needed for timers
MessageControlBlock SwitchUp,   // Simple messages (no internal data)
                    SwitchDown;
MessageQueue        MyQueue;    // The message queue

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
    AvrXTimerHandler();         // Call Time queue manager
    Epilog();                   // Return to tasks
}
/*
 Task 1 Waits for a message, parses it and takes action.
 */
AVRX_IAR_TASKDEF(task1, 0, 20, 3)
AVRX_GCC_TASKDEF(task1, 20, 3)
{
    MessageControlBlock *p;

    while (1)
    {
        p = AvrXWaitMessage(&MyQueue);
        if (p == &SwitchUp)
        	LED = 0xFF;
        else if (p == &SwitchDown)
        	LED = 0x00;
        else
            AvrXHalt();
        AvrXAckMessage(p);
    }
}
/*
 Task 2 Checks switches every 10ms and sends a message SWITCH:0 changes
 state
 */
AVRX_IAR_TASKDEF(task2, 0, 20, 3)
AVRX_GCC_TASKDEF(task2, 20, 3)
{
    unsigned char previous, current;

    previous = SWITCH;     // Keep compiler happy

    while (1)
    {
        AvrXDelay(&MyTimer, 10);             // 10ms delay
        if (previous != (current = (SWITCH & 0x01)))
        {
			_PrintString(PSTR("Hallo"));
            if (current == 0x01)
            {
                AvrXSendMessage(&MyQueue, &SwitchUp);
                AvrXWaitMessageAck(&SwitchUp);
            }
            else
            {
                AvrXSendMessage(&MyQueue, &SwitchDown);
                AvrXWaitMessageAck(&SwitchDown);
            }
            previous = current;
        }
    }
}

int main(void)                 // Main runs under the AvrX Stack
{
    AvrXSetKernelStack(0);

	MCUCR = _BV(SE);
	TCNT0 = TCNT0_INIT;
	TCCR0 = TMC8_CK256;
	TIMSK = _BV(TOIE0);
	LEDDDR = 0xFF;
	LED = 0xFF;
	SWITCHP = 0x0F;

    AvrXRunTask(TCB(task1));
    AvrXRunTask(TCB(task2));
    //AvrXRunTask(TCB(Monitor));

    InitSerialIO(UBRR_INIT);    // Initialize USART baud rate generator

    /* Needed for EEPROM access in monitor */

    AvrXSetSemaphore(&EEPromMutex);

    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
}

