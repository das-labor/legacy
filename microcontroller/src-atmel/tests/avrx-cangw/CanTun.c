#include <stdlib.h>
#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"

#include "config.h"

#include "Serial.h"
#include "CanTun.h"

#define CANTUN_HELLO_STR "*can-gw*0.1*\n"


/////////////////////////////////////////////////////////////////////////////
// CAN TX Task
// AVRX_GCC_TASKDEF(canTunRxTask, 10, 3)
// {
// 	CanTunMsg *msg;
// 
//   while (1)
// {
//      msg = (CanTunMsg *)AvrXWaitMessage(&canTunTxQueue);
//
// 		switch(msg->type) {
// 		case CanTunReset:
// 		case CanTunVersion:
// 			SerialPut('*');
// 			putstr(((CanTunMsgVersion *)msg)->name);		
// 			SerialPut('*');
// 			putstr(((CanTunMsgVersion *)msg)->version);		
// 			putstr("*\n");
// 			break;
// 		case CanTunPacket:
// 			SerialPut('P');
// 			break;
// 		case CanTunFilter:
// 			SerialPut('P');
// 		case CanTunStats:
// 			SerialPut('P');
// 			break;
// 		};
// 
  //       PORTC ^= 0x02;		
// 
  //       AvrXAckMessage((MessageControlBlock *)msg);
// 		free(msg);
  //   }
// }
// 
/////////////////////////////////////////////////////////////////////////////
// Interface
void CanTunInit()
{
	SerialInit(BAUD(57600));
//	AvrXRunTask(TCB(canTunRxTask));
}

void CanTunGet(CanMessage *msg)
{
}

char CanTunGetNB(CanMessage *msg)
{
	return -1;
}

void CanTunSend(CanMessage *msg)
{
	SerialPut('*');
	SerialPut('\r');
}

void CanTunReset()
{
	uint8_t i;
	for(i=0; i<16; i++) 
		SerialPut(0);
}

void CanTunHello()
{
	SerialPutStr(CANTUN_HELLO_STR);
}
