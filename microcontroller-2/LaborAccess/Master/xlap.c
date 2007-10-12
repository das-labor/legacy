
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include "avrx.h"               // AvrX System calls/data structures

#include "xcan.h"
#include "xlap.h"
#include "../../lib/lap/lap.h"

#include "AvrXFifo.h"

AVRX_DECL_FIFO(stream_fifo, 20);


can_addr_t myaddr;

void process_mgt_msg(){
	static can_message_t msg = {0, 0, CHANNEL_MGT, 0, 1, {FKT_MGT_PONG}};
	switch(rx_msg.data[0]){
		case FKT_MGT_RESET:
			wdt_enable(0);
			while(1);
		case FKT_MGT_PING:
			msg.addr_src = myaddr;
			msg.addr_dst = rx_msg.addr_src;
			can_put(&msg);
			break;
	}	
}

void process_stream_msg(){
	uint8_t x;
	if(rx_msg.subchannel == 1){
		for(x=0; x<rx_msg.dlc; x++){
			AvrXPutFifo(stream_fifo, rx_msg.data[x]);
		}
	}
}

void xlap_init(){
	myaddr = eeprom_read_byte(0x00);
	AVRX_INIT_FIFO(stream_fifo);
	can_init();
}

#define CHANNEL_STREAM 0x17

AVRX_GCC_TASKDEF(laptask, 200, 3)
{
	printf("Langer String Test und so\n");
	while (1)
    {
		can_get();			//get next canmessage in rx_msg
		if(rx_msg.addr_dst == myaddr){
			if(rx_msg.channel == CHANNEL_MGT){
				process_mgt_msg();	
			}
			if(rx_msg.channel == CHANNEL_STREAM){
				process_stream_msg();
			}
		}
    }
};

uint8_t stream_getc(){
	return AvrXWaitPullFifo(stream_fifo);
}

uint8_t stream_putc(uint8_t c){
	static can_message_t msg = {0, 0xff, CHANNEL_STREAM, 1, 1, {0}};
	msg.addr_src = myaddr;
	msg.data[0] = c;
	can_put(&msg);
	return 0;
}
