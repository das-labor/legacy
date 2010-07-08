
#include "avrx.h"
#include <inttypes.h>


/********* main interface *************/

void  channel_init();
AVRX_EXTERNTASK(channel);

/***** Interface with using task *****/

//read from channel, size actually read is returned.
uint8_t channel_read(uint8_t num, uint8_t * buffer, uint8_t size);
uint8_t channel_write(uint8_t num, uint8_t * buffer, uint8_t size);


/****** Interface with receiving task *****/
void packet_received(uint8_t id, uint8_t * data, uint8_t size);


/****** Internal Structures ***************/



typedef struct{
	uint8_t * buffer;
	uint8_t buffer_size;//maximum size that fits in the allocated buffer
	uint8_t size;//size of an actual packet in the buffer
	MessageControlBlock mcb;
		//this mcb is put in the message queue whenever a packet is received.
		//it needs to be acked by the receiver to make the channels buffer
		//accessible again for new packets. As long as the mcb isn't acked,
		//the cahnnel will discard incoming packets.
	MessageQueue * queue;
		//this is a pointer to the message queue on which the messages are to be put.
}channel_t;


