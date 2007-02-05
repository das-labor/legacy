
#include "avrx.h"

#include "enum.h"

#include "reader.h"
#include "i2csw.h"

#include "AvrXSerialIo.h"

#include "asn1.h"
#include "channel.h"

ReaderMsg_t msg;

uint8_t buf[256];



void putbuf(uint8_t * b, uint8_t s){
	while(s--)
		put_char0(*b++);

}


AVRX_GCC_TASKDEF(client, 90, 4){
	while(1){
		ReaderMsg_t *p;
		p = (ReaderMsg_t*)AvrXWaitMessage(&ReaderMsgOutQueue);
		AvrXAckMessage((MessageControlBlock*)p);
	
		CARD_POWER_ON();

		if(i2cEeDetect()){
			uint8_t buf[30];
		
			asn1_obj_t root_obj = {4,251};
			asn1_obj_t obj;
			
			asn1_get(&obj, &root_obj, 0x60);
			
			asn1_read(&obj, 0x80, buf, 30);
			
			putbuf(buf, 32);
		}
		
		CARD_POWER_OFF();

		msg.state = COMMAND_EJECT;
		
		AvrXSendMessage(&ReaderMsgInQueue, (MessageControlBlock*)&msg);
		AvrXWaitMessageAck((MessageControlBlock*)&msg);
		
		
	}
			
}
