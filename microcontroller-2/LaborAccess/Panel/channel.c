
#include "channel.h"

#define MAX_CHANNELS 1

typedef struct{
	uint8_t * buffer;
	uint8_t buffer_size;
	uint8_t size;
	Mutex mutex;
}channel_t;



uint8_t client_buf[20];

channel_t Chanels[] ={
	{client_buf, sizeof(client_buf), 0, 0},
}

uint8_t channel_read(uint8_t id, uint8_t * buffer, uint8_t size){
	uint8_t chansize;
	channel_t * chan;
	chan = &Chanels[id];
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
}



void packet_received(uint8_t id, uint8_t * data, uint16_t size){
	if(id < MAX_CHANNELS){
		channel_t * chan = &Channels[id];
		if(chan->buffer_size >= size){
			memcpy(chan->buffer, data, size);
			chan->size = size;
			AvrXSetSemaphore(&chan->mutex);
		}
	}
}
