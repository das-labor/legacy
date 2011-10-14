#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <string.h>

#include "avrx.h"               // AvrX System calls/data structures

#include "config.h"
#include "spi.h"
#include "xcan.h"
#include "lap.h"

#include "fifo.h"
#include "rf.h"
#include "pump.h"
#include "feuchte.h"

uint8_t myaddr;

void process_sdo_message()
{
	// feedback function?
	// someday when we've got a humidity sensor...
}

void process_mgt_msg()
{
	static can_message_t msg = {0, 0, PORT_MGT, PORT_MGT, 1, {FKT_MGT_PONG}};
	switch(rx_msg.data[0])
	{
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

void process_gate_msg()
{
	static uint8_t pump_state;
	
	PUMP_DDR |= PUMP_PIN;
	pump_state ^= 1;
	
	if(pump_state){
		if(feuchte < 50){ //don't drown poor chuck
			PUMP_PORT |= (PUMP_PIN);
		}
	}else{
		PUMP_PORT &= ~(PUMP_PIN);
	}
}

AVRX_GCC_TASKDEF(laptask, 100, 3) /* name, stackspace, pri */
{
    while (1)
    {
		can_get();			//get next canmessage in rx_msg
		if(rx_msg.addr_dst == myaddr){
			if(rx_msg.port_dst == PORT_MGT){
				process_mgt_msg();	
			}else if(rx_msg.port_dst == PORT_REMOTE){
//				process_remote_msg();
			}else if(rx_msg.port_dst == PORT_GATE){
				process_gate_msg();
				can_setled (1,1);
			}
		}
    }
};

void xlap_init()
{
	myaddr = eeprom_read_byte(0x00);
	spi_init();
	can_init();
}
