
#include <stdint.h>
#include "avrx.h"
#include "AvrXSerialIo.h"
#include "channel.h"

#define BUFFERSIZE 50
uint8_t buffer[BUFFERSIZE];


AVRX_GCC_TASKDEF(qport, 50, 1){
	static uint8_t test;
	uint8_t size;
	uint8_t id;
	while(1){
		size = get_char0();
		if(size>0){
			id = get_char0();
			if(size<=BUFFERSIZE){
				uint8_t x;
				for(x=0;x<size;x++){
					buffer[x] = get_char0();
				}
				packet_received(id, buffer, size);
			}
		}
	}
}

void qport_write(uint8_t id, uint8_t * buffer, uint8_t size){
	put_char0(size);
	put_char0(id);
	while(size--)
		put_char0(*buffer++);	
}
