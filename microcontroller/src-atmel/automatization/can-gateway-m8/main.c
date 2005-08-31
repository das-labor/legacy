
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <stdlib.h>


#include "config.h"
#include "util.h"
#include "spi.h"
#include "can.h"

#include "uart.h"
#include "can-uart.h"
#include "can-encap.h"

// Timer Interrupt
SIGNAL(SIG_OUTPUT_COMPARE2)
{
	static int count = 0;
	static char z = 0;

	if ( count++ > 0x40 ) {
		count = 0;
		PORTC ^= 0x08;

		can_message *msg = can_buffer_get();

		msg->addr_src  = 0x11;
		msg->addr_dst  = 0x22;
		msg->port_src  = 0x01;
		msg->port_dst  = 0x02;
		msg->dlc       = 4;
		msg->data[0]   = z++;

		can_transmit(msg);
	}
}

void timer0_on(){
/* 	TCCR0: FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00
		CS02 CS01 CS00
		 0    0    0	       stop
		 0    0    1       clk
		 0    1    0       clk/8
		 0    1    1       clk/64
		 1    0    0       clk/256
		 1    0    1       clk/1024
	
*/
	TCCR2 = 0x08 | 0x05 ;// CTC Mode, clk/1024
	TCNT2 = 0;	// reset timer
	OCR2  = 0xff;	// Compare with this value
	TIMSK = 0x80;	// Compare match Interrupt on
}




void process_rs232_msg( rs232can_msg *msg )
{
	can_mode_t  can_mode;
	can_message *cmsg;

	switch(msg->cmd) {
		case RS232CAN_SETFILTER:
			break;
		case RS232CAN_SETMODE:
			can_mode = (can_mode_t)(msg->data[0]);
			can_setmode(can_mode);
			break;
		case RS232CAN_PKT:
			cmsg = can_buffer_get();
			rs232can_rs2can(cmsg, msg);
			can_transmit(cmsg);
			break;
	}
}

void process_can_msg(can_message *msg){
	rs232can_msg rmsg;

	rs232can_can2rs(&rmsg, msg);
	canu_transmit(&rmsg);
}

int main(){
	uart_init();
	spi_init();
	can_init();

	DDRC = 0xff;

	sei();

	canu_reset();
	can_setmode(normal);
	
	can_setled(0,1);

	timer0_on();
	
	while(1);
	
	while(1) {
		rs232can_msg *rmsg;
		can_message  *cmsg;


		rmsg = canu_get_nb();
		if (rmsg){ 
			process_rs232_msg(rmsg);
		}
		
		cmsg = can_get_nb();
		if (cmsg){
			process_can_msg(cmsg);
			can_free(cmsg);
		}
	}

	return 0;
}
