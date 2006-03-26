#include <stdlib.h>
#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"

#include "config.h"

#include "AvrXSerialIo.h"
#include "CanTun.h"

#define RESET_STR "*can-gw*0.1*\n"

void putstr(char *str)
{
	while (*str != 0)
		put_char0(*str++);
}

MessageQueue canTunTxQueue;
MessageQueue canTunRxQueue;


/////////////////////////////////////////////////////////////////////////////
// CAN TX Task
AVRX_GCC_TASKDEF(canTunTxTask, 10, 3)
{
	CanTunMsg *msg;

    while (1)
    {
        msg = (CanTunMsg *)AvrXWaitMessage(&canTunTxQueue);

		switch(msg->type) {
		case CanTunVersion:
			put_char0('*');
			putstr(((CanTunMsgVersion *)msg)->name);		
			put_char0('*');
			putstr(((CanTunMsgVersion *)msg)->version);		
			putstr("*\n");
			break;
		case CanTunPacket:
			put_char0('P');
			break;
		case CanTunFilter:
			put_char0('P');
		case CanTunStats:
			put_char0('P');
			break;
		};

        PORTC ^= 0x02;		

        AvrXAckMessage((MessageControlBlock *)msg);
		free(msg);
    }
}

/////////////////////////////////////////////////////////////////////////////
// Interface
void CanTunInit()
{
	InitSerial0(BAUD(57600));
	AvrXRunTask(TCB(canTunTxTask));
}

void CanTunSend(CanTunMsg *msg)
{
	AvrXSendMessage(&canTunTxQueue, &(msg->mcb));
}
