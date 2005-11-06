#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "spi.h"
#include "util.h"
#include "can.h"
#include "lap.h"
#include "mood-hw.h"

extern uint8_t bright[20][4];   // 0x00 - 0xFF

can_addr myaddr;

void mcan_init() 
{
	spi_init();
	can_init();

	myaddr = eeprom_read_byte(0x00);
	pdo_message *msg = (pdo_message *)can_buffer_get();
	msg->addr_src = myaddr;
	msg->addr_dst = 0xff;
	msg->port_src = PORT_MOOD;
	msg->port_dst = PORT_MOOD;
	msg->dlc      = 2;
	msg->cmd      = FKT_MOOD_INFO;
	msg->data[0]  = 5;

	can_transmit((can_message *)msg);
}

void process_mgt_msg(pdo_message *msg)
{
	pdo_message *rmsg;

	switch(msg->cmd) {
	case FKT_MGT_RESET:
		timer0_off(); // 17ms
		for (;;);
		break;
	case FKT_MGT_PING:
		rmsg = (pdo_message *)can_buffer_get();
		rmsg->addr_dst = msg->addr_src;
		rmsg->addr_src = myaddr;
		rmsg->port_dst = msg->port_src;
		rmsg->port_src = msg->port_dst;
		rmsg->cmd = FKT_MGT_PONG;
		rmsg->dlc = 1;
		can_transmit((can_message *)rmsg);
		break;
	}
}

void process_mood_msg(pdo_message *msg)
{
	uint8_t module, led, value;
	pdo_message * rmsg;

	switch(msg->cmd) {
	case FKT_MOOD_SET:
		module = msg->data[0];

		bright[module][0] = msg->data[1];
		bright[module][1] = msg->data[2];
		bright[module][2] = msg->data[3];
		bright[module][3] = msg->data[4];

		bright_calc(module);		
		
		break;
/*
	case FKT_MOOD_GET:
		rmsg = (pdo_message *)can_buffer_get();
		rmsg->addr_dst = msg->addr_src;
		rmsg->addr_src = myaddr;
		rmsg->port_dst = msg->port_src;
		rmsg->port_src = msg->port_dst;
		rmsg->cmd = FKT_MOOD_GET;
		rmsg->dlc = 6;
		rmsg->data[0] = 0;
		can_transmit((can_message *)rmsg);
		break;
*/
	}
}

void mcan_process_messages()
{
	pdo_message *msg = (pdo_message*) can_get_nb();

	while(msg) {
		if (!msg)
			return;

		if(msg->addr_dst == myaddr && msg->port_dst == PORT_MGT) 
			process_mgt_msg(msg);

		if(msg->addr_dst == myaddr && msg->port_dst == PORT_MOOD) 
			process_mood_msg(msg);

		msg = (pdo_message*) can_get_nb();
	};

}


