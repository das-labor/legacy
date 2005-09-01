
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


#define PORT_LEDS PORTD
#define DDR_LEDS DDRD
#define PIN_LEDCL PD5
#define PIN_LEDCK PD6
#define PIN_LEDD PD7


void led_init(){
	DDR_LEDS |= (1<<PIN_LEDD)|(1<<PIN_LEDCL)|(1<<PIN_LEDCK);
	PORT_LEDS |= (1<<PIN_LEDCL);
}

void led_set(unsigned int stat){
	unsigned char x;
	for(x=0;x<16;x++){
		if(stat & 0x01){
			PORT_LEDS |= (1<<PIN_LEDD);
		}else{
			PORT_LEDS &= ~(1<<PIN_LEDD);
		}
		stat>>=1;
		PORT_LEDS |= (1<<PIN_LEDCK);
		PORT_LEDS &= ~(1<<PIN_LEDCK);
	}
}

int main(){
	uart_init();
	spi_init();
	can_init();
	
	led_init();
	
	DDRC = 0xff;

	sei();

	canu_reset();
	can_setmode(normal);
	
	can_setled(0,1);
	
	unsigned int muh=0x01;
	
	led_set(0xFFFF);
		
	while(1) {
		rs232can_msg *rmsg;
		can_message  *cmsg;


		rmsg = canu_get_nb();
		if (rmsg){ 
			led_set(muh);
			if ((muh<<=1)==0) muh = 1;
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
