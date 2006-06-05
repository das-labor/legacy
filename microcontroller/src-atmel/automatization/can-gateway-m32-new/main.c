
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <stdlib.h>


#include "config.h"
#include "util.h"
#include "spi.h"
#include "can.h"

#include "uart.h"
#include "cantun.h"
#include "can-uart.h"

void process_cantun_msg(cantun_msg_t *msg)
{
	can_message_t *cmsg;

	switch(msg->type) {
		case CANTUN_SETFILTER:
			break;
		case CANTUN_SETMODE:
			can_setmode(msg->mode);
			break;
		case CANTUN_PKT:
			cmsg = can_buffer_get();
			cantun_unwrap(cmsg, msg);
			can_transmit(cmsg);
			break;
	}
}

void process_can_msg(can_message_t *msg){
	cantun_msg_t tunmsg;

	cantun_wrap(&tunmsg, msg);
	canu_transmit(&tunmsg);
}

int main(){
	uart_init();
	spi_init();
	can_init();

	DDRC = 0xff;

	sei();

	canu_reset();
	can_setmode(normal);

	while(1) {
		cantun_msg_t  *rmsg;
		can_message_t *cmsg;


		rmsg = canu_get_nb();
		if (rmsg){ 
			process_cantun_msg(rmsg);
		}
		
		cmsg = can_get_nb();
		if (cmsg){
			PORTC ^= 0x80;
			process_can_msg(cmsg);
			can_free(cmsg);
		}
	}

	return 0;
}
