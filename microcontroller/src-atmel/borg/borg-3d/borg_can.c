#include "can.h"
#include "lap.h"
#include "borg_hw.h"
#include "config.h"
#include "borg_can.h"

#include <setjmp.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>

can_addr myaddr;
extern jmp_buf newmode_jmpbuf;

void bcan_init() 
{
	can_init();

	myaddr = eeprom_read_byte(0x00);
	pdo_message *msg = (pdo_message *)can_buffer_get();
	msg->addr_src = myaddr;
	msg->addr_dst = 0xff;
	msg->port_src = PORT_BORG;
	msg->port_dst = PORT_BORG;
	msg->dlc      = 4;
	msg->cmd      = FKT_BORG_INFO;
	msg->data[0]  = NUM_ROWS;
	msg->data[1]  = NUM_COLS;
	msg->data[2]  = NUM_PLANES;

	can_transmit((can_message *)msg);
}

void process_mgt_msg(pdo_message *msg)
{
	pdo_message *rmsg;

	switch(msg->cmd) {
	case FKT_MGT_RESET:
		timer0_off();
		cli();
		while(1);
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


void process_borg_msg(pdo_message *msg)
{
	switch(msg->cmd) {
	case FKT_BORG_MODE:
		longjmp(newmode_jmpbuf, msg->data[0]);
		break;
	}
}

void bcan_process_messages()
{
	pdo_message *msg = (pdo_message*) can_get_nb();

	while(msg) {
		if (!msg)
			return;

		if(msg->addr_dst == myaddr && msg->port_dst == PORT_MGT) 
			process_mgt_msg(msg);

		if(msg->addr_dst == myaddr && msg->port_dst == PORT_BORG) 
			process_borg_msg(msg);

		msg = (pdo_message*) can_get_nb();
	};

}

void watchdog_enable()
{
	wdt_reset();
	wdt_enable(0x00);  // 17ms

}


void timer0_off(){
	cli();
	DATAPORT = 0;
	CTRLPORT = 0;
	TCCR0 = 0x00;   // CTC Mode, clk/64
	sei();
}

