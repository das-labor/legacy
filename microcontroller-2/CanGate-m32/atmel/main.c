
#include <stdlib.h>
#include <string.h>
#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "config.h"

#include "spi.h"
#include "CanTun.h"
#include "Can.h"

AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Save interrupted context, switch stacks
    TCNT0 = TCNT0_INIT;		// Reload the timer counter
    AvrXTimerHandler();         // Process Timer queue
    Epilog();                   // Restore context of next running task
};


//////////////////////////////////////////////////////////////////////////////
// 

typedef struct {
	MessageControlBlock	mcb;
	CanMessage		msg;
} QueuedMessage;

MessageQueue msgQueue;

AVRX_GCC_TASKDEF(TaskLAP, 60, 23)
{
	QueuedMessage *qmsg;
	CanMessage    *msg;

	for(;;) {
		qmsg = (QueuedMessage*)AvrXWaitMessage(&msgQueue);
		msg  = &(qmsg->msg);

		PORTC ^= 1;
		free(qmsg);
	}
}


//////////////////////////////////////////////////////////////////////////////
// Gateway Tasks
AVRX_GCC_TASKDEF(TaskCan2Tun, 30, 2)
{
	CanMessage msg;
	QueuedMessage *qmsg;

	CanTunReset();
	for(;;) {
		CanGet(&msg);
		CanTunSend(&msg);

		if ((qmsg=malloc(sizeof(QueuedMessage)))) {
			memcpy(&(qmsg->msg), &msg, sizeof(CanMessage));
			AvrXSendMessage(&msgQueue, qmsg);
		}
	}
}

AVRX_GCC_TASKDEF(TaskTun2Can, 30, 2)
{
	CanMessage   msg;

	for(;;) {
		CanTunGet(&msg);
		CanSend(&msg);
	}
}

//////////////////////////////////////////////////////////////////////////////
// Main

int main(void)
{
	AvrXSetKernelStack(0);

 	DDRC = 0xFF;

	spi_init();

	MCUCR = 1<<SE;      	// Enable "sleep" mode (low power when idle)
	TCNT0 = TCNT0_INIT;		// Load overflow counter of timer0
	TCCR0 = TMC8_CK256;		// Set Timer0 to CPUCLK/256
	TIMSK = 1<<TOIE0;		// Enable interrupt flag

	CanInit();
	CanTunInit();

	AvrXRunTask(TCB(TaskCan2Tun));
	AvrXRunTask(TCB(TaskTun2Can));
	AvrXRunTask(TCB(TaskLAP));

	Epilog();                   // Switch from AvrX Stack to first task
};

