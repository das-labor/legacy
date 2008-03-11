
#include <string.h>

#include "channel.h"
#include "dummy_qport.h"
#include "channels.h"


/****** Actual Channels **********/
uint8_t client_buf[20];

channel_t Channels[NUM_CHANNELS] ={
	{client_buf, sizeof(client_buf), {0, SEM_DONE}, 0},
};




//read from channel, size actually read is returned.
//acks the message after copying the data, marking the buffer as empty again.
uint8_t channel_read(uint8_t id, uint8_t * buffer, uint8_t size){
	uint8_t chansize;
	channel_t * chan;
	chan = &Channels[id];
	do{
		AvrXWaitSemaphore(&chan->mutex);
	}while(chan->size == 0);
	
	chansize = chan->size;
	if(chansize <= size){
		memcpy(buffer, chan->buffer, chansize);
		chan->size = 0;
		return chansize;
	}
	return 0;
}

uint8_t channel_write(uint8_t id, uint8_t * buffer, uint8_t size){
	qport_write(id, buffer, size);
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
