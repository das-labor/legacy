#include <avrx.h>
#include <stdlib.h>
#include "Event.h"
#include "serial.h"

uint16_t eventIgnoreMask;
uint16_t eventReplayMask;
uint16_t eventAcceptMask;

MessageQueue sendMsgQueue;
MessageQueue eventAcceptQueue;
MessageQueue eventReplayQueue;


inline void SendEvent(Event *ev)
{
	AvrXSendMessage( &sendMsgQueue, (MessageControlBlock *)ev );
}


inline Event *WaitEvent()
{
	Event *ev;
	ev = (Event *)AvrXWaitMessage( &eventAcceptQueue );

	return ev;
}

///////////////////////////////////////////////////////////////////////
// Event tasks and serial communication

inline int8_t SerialGetEscaped(uint8_t *c)
{
	SerialGet(c);
	if (*c == 0x23) 
		return -1;

	if (*c == 0x42) 
		SerialGet(c);

	return 0;
}

inline void SerialPutEscaped(uint8_t c)
{
	if (c==0x23) {
		SerialPut(0x42);
		SerialPut(0x23);
	} else if (c==0x42) {
		SerialPut(0x42);
		SerialPut(0x42);
	}
}



AVRX_GCC_TASKDEF(SerialToEventTask, 20, 0)       
{
	Event *ev;
	uint8_t dummy;

	while (SerialGetEscaped(&dummy) != -1) {};
	for(;;) {
		uint8_t size;
		uint8_t *p;

		// Start of packet: read size byte
		if ( SerialGetEscaped(&size)==-1 ) 
			continue;
		
		// Packet = (size, type, [..data..]) ; size in respect to [..data..]
		ev = (Event *)malloc(size + sizeof(Event));
		ev->size = size;

		p = (uint8_t *)&(ev->type);
		for(uint8_t i=0; i<size+2; i++) {
			if (SerialGetEscaped(p++) == -1) 
				goto error;
		}

		if (ev->type & eventReplaytMask)
			AvrXSendMessage( &eventReplayQueue, (MessageControlBlock *)ev );

		if (ev->type & eventAcceptMask)
			AvrXSendMessage( &eventAcceptQueue, (MessageControlBlock *)ev );

		if (ev->type eventIgnoreMask)
			;

		eventError = 


		while (SerialGetEscaped(&dummy) != -1) {};
		continue;

error:
		eventEr
		free(ev);
	}
}

AVRX_GCC_TASKDEF(EventToSerialTask, 10, 0)       
{
	for(;;) {
		uint8_t *p;
		Event *ev;

		ev = (Event *)AvrXWaitMessage( &sendMsgQueue );

		SerialPut(0x23);

		p = (uint8_t *)ev;
		for(uint8_t i=0; i<ev->size+2; i++)
			SerialPutEscaped( *p );

		free(ev);
	}
}
