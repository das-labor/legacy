
#include <stdint.h>
#include "avrx.h"
#include "AvrXSerialIo.h"
#include "channel.h"

#define BUFFERSIZE 50
uint8_t buffer[BUFFERSIZE];


AVRX_GCC_TASKDEF(qport, 100, 1){
	uint8_t size;
	uint8_t id;
	while(1){
		size = get_char0();
		printf("size %d\r",size);
		if(size>0){
			id = get_char0();
			printf("id %d\r",id);
			if(size<=BUFFERSIZE){
				printf("size <= BUFFERSIZE\r");
				uint8_t x;
				for(x=0;x<size;x++){
					buffer[x] = get_char0();
					printf("%x ",buffer[x]);
				}
				printf("calling packet_received\r");
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
