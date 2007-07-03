#include <stdlib.h>
#include <string.h>
#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"

#include "config.h"

#include "Serial.h"
#include "CanTun.h"

#define CANTUN_HELLO_STR "*can-gw*0.1*\n"


// MessageQueue canTunTxQueue;

// Recieving messages
CanMessage canTunRxMsg;
Mutex      canTunRxEmpty, canTunRxFull;

// Sending messages
Mutex  canTunTxData;

/////////////////////////////////////////////////////////////////////////////
// CanTun Rx Task (the only one reading from serial)
AVRX_GCC_TASKDEF(canTunRxTask, 20, 3)
{
   void *p;
 
   while(1) {
	uint8_t len, cmd;
	
	// Read length byte
	SerialGet(&len);
	if (len > 16) {
		CanTunReset();
		continue;
	}

	// Read and handle cmd token
	SerialGet(&cmd);

	switch(cmd) {
	case 0:  // NOP
		break;
	case 1:  // rcv. packet 
		p = &canTunRxMsg;

		AvrXWaitSemaphore(&canTunRxEmpty);
		for(len=len; len>0; len--) {
		    SerialGet(p); p++;
		}
   		AvrXSetSemaphore(&canTunRxFull);

		break;
	default:
		CanTunReset();
		continue;
	}
   }
}



/////////////////////////////////////////////////////////////////////////////
// Interface
void CanTunInit()
{
	SerialInit(BAUD(57600));

	AvrXSetSemaphore(&canTunRxEmpty);     
	AvrXResetSemaphore(&canTunRxFull);

	AvrXSetSemaphore(&canTunTxData);

	AvrXRunTask(TCB(canTunRxTask));
}

void CanTunGet(CanMessage *msg)
{
	AvrXWaitSemaphore(&canTunRxFull);
	memcpy(msg, &canTunRxMsg, sizeof(CanMessage));
	AvrXSetSemaphore(&canTunRxEmpty);
}

char CanTunGetNB(CanMessage *msg)
{
        if (AvrXTestSemaphore(&canTunRxFull) == SEM_DONE) {
		memcpy(msg, &canTunRxMsg, sizeof(CanMessage));
		AvrXSetSemaphore(&canTunRxEmpty);
		return 0;
	} else
		return -1;
}

void CanTunSend(CanMessage *msg)
{
	char *p;
	uint8_t len, i;
	       
	len = 1 + 4 + 1 + msg->dlc;   // len = cmd + id[4] + dlc + data

	AvrXWaitSemaphore(&canTunTxData);
	SerialPut(len);
	SerialPut(1);

	p = (char *)msg;
        for (i=len-1; i>0; i--)
                SerialPut(*(p++));

	AvrXSetSemaphore(&canTunTxData);
}

void CanTunReset()
{
	uint8_t i;

	AvrXWaitSemaphore(&canTunTxData);
	for(i=0; i<1; i++) 
		SerialPut(0);
	AvrXSetSemaphore(&canTunTxData);
}

