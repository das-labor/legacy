#include <avr/io.h>
#include <stdlib.h>

#include "util.h"
#include "timing.h"
#include "can.h"


void timing_test()
{
	unsigned char i;
	for(i=0;;i++) {
		PORTC |= 0x10;
		PORTC &= ~0x10;
		PORTC |= 0x10;
		PORTC &= ~0x10;

		can_message *msg = can_buffer_get();
		msg->addr_src = i;
		msg->addr_dest = ~i;
		msg->port_src = 0x11;
		msg->port_dest = 0x3f;
		msg->dlc = 8;
		msg->data[0] = 0xFF;
		msg->data[1] = 0xAA;
		msg->data[2] = 0x00;
		msg->data[3] = 0x00;
		msg->data[4] = 0x00;
		msg->data[5] = 0xFF;
		msg->data[6] = 0xFF;
		msg->data[7] = 0xFF;
		msg->flags = 0x01;

		can_transmit();

		PORTC |= 0x10;
		PORTC &= ~0x10;
		PORTC |= 0x10;
		PORTC &= ~0x10;
		
		msg = can_message_get_nb();
		wait(3);
	}
}

void r_test(){
	
	can_message *msg = can_message_get_nb();
	while(1){
		if(msg){
			hex_dump(msg->addr_src,2);
	
		}
	}
}

