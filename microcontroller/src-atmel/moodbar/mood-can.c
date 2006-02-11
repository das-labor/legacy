#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

#include "spi.h"
#include "util.h"
#include "can.h"
#include "lap.h"
#include "tlv5604.h"
#include "mood-can.h"

#include "exp_table.c"


//cal = (zeroval-maxval-800)*512/800

pl_param_t pl={
	980, 940, 160, 800, 38
};

//returns the value to output on the d/a converter for a specific brightnes
uint16_t bright_calc(pl_param_t * param, uint8_t bright){
	uint16_t val;	
	val = pgm_read_word(&exp_table[bright]);
	val += (param->cal * (uint8_t)(val/4)) / 128;
	val = param->zeroval-val;
	if (val > param->minval) val = param->minval;
	if (val < param->maxval) val = param->maxval;
	return val;
}


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
		//timer0_off(); // 17ms
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
	uint8_t module;
	pdo_message * rmsg;

	switch(msg->cmd) {
	case FKT_MOOD_SET:
		module = msg->data[0];

		da_set (0, bright_calc (&pl, msg->data[1]));
		da_set (1, msg->data[2]<<2);
		da_set (2, msg->data[3]<<2);
		da_set (3, msg->data[4]<<2);
		
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
