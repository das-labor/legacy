
#include <stdlib.h>
#include <string.h>
#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "config.h"

#include "CanTun.h"

AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Save interrupted context, switch stacks
    TCNT0 = TCNT0_INIT;			// Reload the timer counter
    AvrXTimerHandler();         // Process Timer queue
    Epilog();                   // Restore context of next running task
};


// AVRX_IAR_TASK(Monitor, 0, 20, 0);      // external Debug Monitor
// AVRX_GCC_TASK(Monitor, 20, 0);      // external Debug Monitor

// void uart_putstr(char *str)
// {
// 	while (*str != 0)
//		put_char0(*str++);
// }

AVRX_GCC_TASKDEF(task1, 40, 3)
{
	CanTunMsgVersion *msg;
	TimerControlBlock timer;

	CanTunInit();

	while(1)
	{
		AvrXDelay(&timer, 500);

		msg = (CanTunMsgVersion *)malloc(sizeof(CanTunMsgVersion));
		msg->type    = CanTunVersion;
		strcpy(msg->name, "can-gw");
		strcpy(msg->version, "0.1");
		CanTunSend((CanTunMsg *)msg);

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
	
//	InitSerialIO(UBRR_INIT);    // Initialize USART baud rate generator
//    AvrXRunTask(TCB(Monitor));
    AvrXRunTask(TCB(task1));

    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
};
