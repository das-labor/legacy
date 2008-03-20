
#include <string.h>

#include "AvrXSerialIo.h"
#include "myint.h"


/****** Main Buffer **************/

u08 buffer[BUFFERSIZE];

/*********** Global Varaibles ****************/

//This mutex interlocks the channels against each other, so their packets won't get
//intermixed.
Mutex ChannelInterlock;


void  channel_init(){
	AvrXSetSemaphore(&ChannelInterlock);
}

void send_reset(){
	put_char0(0x23);
}

void send_char(u08 c){
	switch(c){
		case 0x23:
		case 0x42:
			put_char0(0x42);
			put_char0(c ^ 0x55);
			break;
		default:
			put_char0(c);
	}
}


uint8_t channel_write(uint8_t id, uint8_t * buffer, uint8_t size){
	//request the Comport
	AvrXWaitSemaphore(&ChannelInterlock);
	
	send_reset();
	
	send_char(size);
	send_char(id);
	while(size--)
		send_char(*buffer++);

	//free the Comport
	AvrXSetSemaphore(&ChannelInterlock);
	return 0;
}

//read from channel, size actually read is returned.
//acks the message after copying the data, marking the buffer as empty again.
uint8_t channel_read(uint8_t id, uint8_t * buffer, uint8_t max_size){
	uint8_t chansize;
	channel_t * chan;
	chan = &Channels[id];
	
	chansize = chan->size;
	if(chansize <= max_size){
		memcpy(buffer, chan->buffer, chansize);
		AvrXAckMessage(&chan->mcb);
		return chansize;
	}
	AvrXAckMessage(&chan->mcb);
	return 0;
}


void packet_received(uint8_t id, uint8_t * data, uint8_t size){
	if(id < NUM_CHANNELS){
		channel_t * chan = &Channels[id];
		if(chan->buffer_size >= size){
			if(AvrXTestMessageAck( &chan->mcb )){
				//store packet in buffer, if it is free. Otherwise packet is dropped.
				memcpy(chan->buffer, data, size);
				chan->size = size;
				AvrXSendMessage(chan->queue, &chan->mcb);
			}
		}
	}
}

//decode escape codes. returns original chars or 0xffff if reset occurred.
u16 receive_char(){
	u08 c;
	c = get_char0();
	
	switch(c){
		case 0x23:
			return 0xffff;
			break;
		case 0x42:
			c = 0x55 ^ get_char0();
			break;
		default:
			break;
	}
	return c;
}

AVRX_GCC_TASKDEF(channel, 50, 1){
	u16 r;
	u08 size;
	u08 id;
	u08 x;
	while(1){
start:
		if((r = receive_char()) > 0xff) goto start;
		size = r;
		
		if(size>0){
			if((r = receive_char()) > 0xff) goto start;
			id = r;
			if(size<=BUFFERSIZE){
				uint8_t x;
				for(x=0;x<size;x++){
					if((r = receive_char()) > 0xff) goto start;
					buffer[x] = r;
				}
				packet_received(id, buffer, size);
			}else{
				for(x=0; x<size; x++){
					if((r = receive_char()) > 0xff) goto start;
				}
			}
		}
	}
}

