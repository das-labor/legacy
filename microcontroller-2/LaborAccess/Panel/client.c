
#include "avrx.h"

#include "enum.h"

#include "reader.h"
#include "i2csw.h"
#include "7seg.h"

#include "AvrXSerialIo.h"

#include "asn1.h"
#include "channel.h"

#include "protocol.h"

ReaderMsg_t msg;

uint8_t buf[256];



void putbuf(uint8_t * b, uint8_t s){
	while(s--)
		put_char0(*b++);

}

//uint8_t muh[]={0xde,0xad,0xbe,0xef, 0x69, 0x0e ,0x80, 0x02, 0x01, 0x00, 0x81, 0x08, 0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};


uint8_t card_read_credentials(request_cred_t * request){
	if(! i2cEeDetect())
		return 1;
	
	asn1_obj_t root_obj = {4,251};
	asn1_obj_t obj;
		
	if (asn1_get(&obj, &root_obj, 0x69))
		return 1;
			
	if ( asn1_read(&obj, 0x80, (uint8_t *) &request.credentials.index, 2) != 2)
		return 1;
			
	if ( asn1_read(&obj, 0x81, (uint8_t *) &request.credentials.key, 8) != 8 )
		return 1;
		

}


AVRX_GCC_TASKDEF(client, 90, 4){
	while(1){
		ReaderMsg_t *p;
		request_cred_t request;
		reply_t reply;
		
		p = (ReaderMsg_t*)AvrXWaitMessage(&ReaderMsgOutQueue);
		AvrXAckMessage((MessageControlBlock*)p);
	
		CARD_POWER_ON();

		//i2cEeWrite(0,sizeof(muh), muh);
		
		if(! i2cEeDetect())
			goto error;
		
		asn1_obj_t root_obj = {4,251};
		asn1_obj_t obj;
			
		if (asn1_get(&obj, &root_obj, 0x69))
			goto error;
				
		if ( asn1_read(&obj, 0x80, (uint8_t *) &request.credentials.index, 2) != 2)
			goto error;
			
		if ( asn1_read(&obj, 0x81, (uint8_t *) &request.credentials.key, 8) != 8 )
			goto error;
		
		request.type = REQUEST_OPEN_CRED;
		
		seg_putstr("\r" "openrequest");
		
		channel_write(0, (uint8_t *)&request, sizeof(request));
		channel_read(0, (uint8_t *)&reply, sizeof(reply));
		
		seg_putstr("\r");
		seg_putstr(reply.nickname);
		
		
		error:
		
		CARD_POWER_OFF();

		msg.state = COMMAND_EJECT;
		
		AvrXSendMessage(&ReaderMsgInQueue, (MessageControlBlock*)&msg);
		AvrXWaitMessageAck((MessageControlBlock*)&msg);
		
		
	}
			
}
