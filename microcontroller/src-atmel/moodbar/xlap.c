
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "config.h"
#include "spi.h"
#include "xcan.h"
#include "lap.h"
#include "xlap.h"
#include "tlv5604.h"

#include "pl_dim.h"




can_addr myaddr;


void process_mgt_msg(){
	static can_message_t msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
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

void process_lamp_msg(){
	switch(rx_msg.data[0]){
		case FKT_LAMPE_SET:
			lampe_set(rx_msg.data[1]%8, rx_msg.data[2]);
			break;
		case FKT_LAMPE_ADD:
			lampe_dim(rx_msg.data[1]%8, rx_msg.data[2]);
			break;
	}
}

void process_mood_msg(){
	uint8_t module;
	switch(rx_msg.data[0]){
		case FKT_MOOD_SET:
			module = rx_msg.data[1];
			switch (module){
				case 0:
					da_set (0, rx_msg.data[2]<<2);
					da_set (1, rx_msg.data[3]<<2);
					da_set (2, rx_msg.data[4]<<2);
					da_set (3, rx_msg.data[5]<<2);
					break;
				case 1:
					//da_set1 (0, rx_msg.data[2]<<2);
					//da_set1 (1, rx_msg.data[3]<<2);
					//da_set1 (2, rx_msg.data[4]<<2);
					//da_set1 (3, rx_msg.data[5]<<2);
					break;
			}
			break;
		case 0:
			DDRB |= 3;
			PORTB &= ~3;		
				
			break;
	}
}

AVRX_GCC_TASKDEF(laptask, 50, 3)
{
    while (1)
    {
		can_get();			//get next canmessage in rx_msg
		if(rx_msg.addr_dst == myaddr){
			if(rx_msg.port_dst == PORT_MGT){
				process_mgt_msg();	
			}else if(rx_msg.port_dst == PORT_MOOD){
				process_mood_msg();
			}else if(rx_msg.port_dst == PORT_LAMPE){
				process_lamp_msg();
			}
		}
    }
};

void xlap_init(){
	myaddr = eeprom_read_byte(0x00);
	spi_init();
	can_init();
}
