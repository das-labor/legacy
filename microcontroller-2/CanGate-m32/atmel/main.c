
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


// AVRX_GCC_TASK(Monitor, 20, 0);      // external Debug Monitor

AVRX_GCC_TASKDEF(TaskCan2Tun, 60, 1)
{
	CanMessage cmsg;

//	CanTunHello();
	CanTunReset();

	while(1) {
		CanGet(&cmsg);
		cmsg.dlc = 4;
		cmsg.data[0] = cmsg.id[3];
		cmsg.data[1] = cmsg.id[3];
		cmsg.data[2] = cmsg.id[3];
		cmsg.data[3] = cmsg.id[3];
		CanTunSend(&cmsg);
	}
}

AVRX_GCC_TASKDEF(TaskTun2Can, 60, 1)
{
	CanMessage cmsg;

	while(1) {
		CanTunGet(&cmsg);
		CanSend(&cmsg);
	}
}

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

    Epilog();                   // Switch from AvrX Stack to first task
};

