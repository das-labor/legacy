
#include <string.h>

#include "channel.h"
#include "dummy_qport.h"
#include "channels.h"


//read from channel, size actually read is returned.
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



void packet_received(uint8_t id, uint8_t * data, uint16_t size){
	if(id < NUM_CHANNELS){
		channel_t * chan = &Channels[id];
		if(chan->buffer_size >= size){
			memcpy(chan->buffer, data, size);
			chan->size = size;
			AvrXSetSemaphore(&chan->mutex);
		}
	}
}
