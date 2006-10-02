#include "avrx.h"

#define TypeError           0x0000
#define TypeErrorComm       0x0001   
#define TypeErrorReboot     0x0002
#define TypeCard            0x0100
#define TypeCardInserted    0x0100   /* Terminal -> Master */
#define TypeCardEjected     0x0101   /* Terminal -> Master */
#define TypeCardJamed       0x0102   /* Terminal -> Master */
#define TypeCardEject       0x0103   /* Master -> Terminal */
#define TypeCardCollect     0x0104   /* Master -> Terminal */
#define TypeCardRead        0x0105
#define TypeCardWrite       0x0106
#define TypeKey             0x0200
#define TypeKeyPressed      0x0201
#define TypeLed             0x0300
#define TypePanel           0x0400
#define TypePanelDoorMode   0x0401
#define TypePanelNormalMode 0x0402

typedef struct {       // Message
	MessageControlBlock mcb;
	uint16_t type;	
	uint8_t size;   //bytes in data
	
	uint8_t data[];
} Message;


typedef struct {       // MsgCardData
	MessageControlBlock msb;
	uint16_t type;	
	uint8_t size;   //bytes in message from here including this one

	uint16_t start;	//offset in card
	uint8_t  dataSize;//requested or transmitted number of bytes
	uint8_t  data[];
} MsgCardData;
	

MessageQueue masterMsgQueue;
extern MessageQueue commMsgQueue;

//get bytes from card in slave
//returns 0 on success, or error message we got
Message * cardRead(uint8_t * buffer, uint16_t offset, uint8_t size){
	MsgCardData *tmsg;
	MsgCardData *rmsg;
		
	tmsg = malloc( sizeof(MsgCardData) );
	tmsg->type  = TypeCardRead;
	tmsg->size  = sizeof(MsgCardData)-sizeof(Message);
	tmsg->start = offset;
	tmsg->dataSize = size;
	AvrXSendMessage(commMsgQueue, tmsg);
	
	rmsg = (MsgCardData*) AvrXWaitMessage(masterMsgQueue);
	
	//return message on error
	if(rmsg->type != TypeCardRead)
		return (Message *)rmsg;
	
	if()
	
	memcpy(buffer, rmsg->data, size);
	

}

AVRX_GCC_TASKDEF(Master, 20, 0){
	Message *tmsg;
	tmsg = malloc( sizeof(Message) );
	tmsg->type = TypeCardEject;
	tmsg->size = 0;
	AvrXSendMessage(commMsgQueue, tmsg);
	
	tmsg = malloc( sizeof(Message) );
	tmsg->type = TypePanelNormalMode;
	tmsg->size = 0;
	AvrXSendMessage(commMsgQueue, tmsg);

	while(1){
		Message *rmsg;
		rmsg = (Message*) AvrXWaitMessage(masterMsgQueue);
		
		if (rmsg->type != TypeCardInserted)
			goto error;
			
		tmsg = malloc( sizeof(Message) );
		tmsg->type = TypeLedDoorMode;
		tmsg->size = 0;
		AvrXSendMessage(commMsgQueue, tmsg);
		
		
		
		
				
	}
	
	}
}
