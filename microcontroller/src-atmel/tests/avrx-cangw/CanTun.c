#include <stdlib.h>
#include <string.h>
#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"

#include "config.h"

#include "Serial.h"
#include "CanTun.h"

#define CANTUN_HELLO_STR "*can-gw*0.1*\n"

// Recieving messages
CanMessage canTunRxMsg;
Mutex      canTunRxEmpty, canTunRxFull;

// Sending messages
Mutex  canTunTxData;

/////////////////////////////////////////////////////////////////////////////
// CanTun Rx Task (the only one reading from serial)
AVRX_GCC_TASKDEF(canTunRxTask, 40, 3)
{
   void *p;
 
   while(1) {
	uint8_t len, cmd;
	
	// Read length byte
	SerialGet(&len);
	if (len == 0)
		continue;

	if (len > 16) {
		CanTunReset();
		continue;
	}

	// Read and handle cmd token
	SerialGet(&cmd); len--;

	switch(cmd) {
	case 0:  // NOP
		break;
	case 10: // Hello 
	case 13:
		CanTunHello();
		break;
	case 23:  // rcv. packet 
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

	// Semaphores
	AvrXSetSemaphore(&canTunRxEmpty);     
	AvrXResetSemaphore(&canTunRxFull);

	AvrXSetSemaphore(&canTunTxData);

	// Start protokol
	CanTunHello();
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
	AvrXWaitSemaphore(&canTunTxData);
	SerialPut('*');
	SerialPut('\r');
	AvrXSetSemaphore(&canTunTxData);
}

void CanTunReset()
{
	uint8_t i;

	AvrXWaitSemaphore(&canTunTxData);
	for(i=0; i<16; i++) 
		SerialPut(0);
	AvrXSetSemaphore(&canTunTxData);
}

void CanTunHello()
{
	AvrXWaitSemaphore(&canTunTxData);
	SerialPutStr(CANTUN_HELLO_STR);
	AvrXSetSemaphore(&canTunTxData);
}
