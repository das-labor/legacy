#include <stdlib.h>
#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"

#include "config.h"

#include "Serial.h"
#include "CanTun.h"

#define RESET_STR "*can-gw*0.1*\n"

void putstr(char *str)
{
	while (*str != 0)
		(*str++);
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
		case CanTunReset:
		case CanTunVersion:
			SerialPut('*');
			putstr(((CanTunMsgVersion *)msg)->name);		
			SerialPut('*');
			putstr(((CanTunMsgVersion *)msg)->version);		
			putstr("*\n");
			break;
		case CanTunPacket:
			SerialPut('P');
			break;
		case CanTunFilter:
			SerialPut('P');
		case CanTunStats:
			SerialPut('P');
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
	SerialInit(BAUD(57600));
	AvrXRunTask(TCB(canTunTxTask));
}

void CanTunSend(CanTunMsg *msg)
{
	AvrXSendMessage(&canTunTxQueue, &(msg->mcb));
}
