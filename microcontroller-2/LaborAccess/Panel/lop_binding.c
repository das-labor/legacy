
#include <stdint.h>
#include "avrx.h"
#include "AvrXSerialIo.h"
#include "channel.h"
#include <stdlib.h>
#include <string.h>


#include "lop.h"

/******************************************************************************/
/* GLOBAL VARIABLES                                                           */
/******************************************************************************/

uint8_t onsync=0;

lop_ctx_t lop0={
	idle, idle, idle, 0, 0, NULL, 0, 
	NULL, NULL, NULL, NULL};


#define BUFFERSIZE 50
uint8_t buffer[BUFFERSIZE];

	
	
void lop0_sendrawbyte(uint8_t b){
	put_char0(b);
}


void lop0_messagerx(uint16_t length, uint8_t * msg){
	uint8_t id;
	if(length > sizeof(buffer)){
		free(msg);
		return;		
	}
	
	memcpy(buffer, msg, length);
	
	free(msg);
	
	packet_received(0, buffer, length);
	
}




AVRX_GCC_TASKDEF(loptask, 50, 1){
	//set handlers for the outputs from lop
	//lop0.on_streamrx = lop0_streamrx;
	lop0.sendrawbyte = lop0_sendrawbyte;
	//lop0.on_streamsync = lop0_streamsync;
	lop0.on_msgrx = lop0_messagerx;
	
	lop_sendreset(&lop0);

	uint8_t c;
	while(1){
		c = get_char0();
		lop_recieve_byte(&lop0,c);
	}
}



void lop_write(uint8_t id, uint8_t * buffer, uint8_t size){
	
	
	lop_sendmessage(&lop0, size, buffer);
	
}
