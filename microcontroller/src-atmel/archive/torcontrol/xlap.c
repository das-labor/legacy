
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/eeprom.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include <string.h>

#include "avrx.h"               // AvrX System calls/data structures

#include "config.h"
#include "spi.h"
#include "xcan.h"
#include "lap.h"

#include "rf.h"
#include "lamp_and_switch.h"

uint8_t myaddr;

static can_message_t msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	

uint16_t dump_addr = 0x60;
	

void memdump(){
	msg.dlc = 8;
	memcpy(msg.data,(void*) dump_addr, 8);
	can_put(&msg);
	dump_addr+=8;
}

void process_mgt_msg(){
	switch(rx_msg.data[0]){
		case FKT_MGT_RESET:
			wdt_enable(0);
			while(1);
		case FKT_MGT_PING:
			msg.addr_dst = rx_msg.addr_src;
			can_put(&msg);
			break;
		case 0x23:
			memdump();
			break;
		case 0x24:
			dump_addr = 0x60;
			break;
	}
}

void process_remote_msg(){
	AvrXPutFifo(rftxfifo, *(uint32_t*)rx_msg.data);
}

void process_lapd_msg(){
	AvrXPutFifo(lapd_fifo, *(uint32_t*)rx_msg.data);
}



void process_gate_msg(){
	if(!memcmp(rx_msg.data, (uint8_t[]){0xde,0xad,0xbe,0xef}, 4)){
		AvrXPutFifo(rftxfifo, 0x010050B5); //Tor auf 101101010101  B55
	}
}

AVRX_GCC_TASKDEF(laptask, 70, 3) /* name, stackspace, pri */
{
    while (1)
    {
		can_get();			//get next canmessage in rx_msg
		if(rx_msg.addr_dst == myaddr){
			if(rx_msg.port_dst == PORT_MGT){
				process_mgt_msg();	
			}else if(rx_msg.port_dst == PORT_REMOTE){
				process_remote_msg();
			}else if(rx_msg.port_dst == PORT_GATE){
				process_gate_msg();
			}else if(rx_msg.port_dst == PORT_LAPD){
				process_lapd_msg();
			}
		}
    }
}


void xlap_init(){
	myaddr = eeprom_read_byte(0x00);
	msg.addr_src = myaddr;
	spi_init();
	can_init();
}
