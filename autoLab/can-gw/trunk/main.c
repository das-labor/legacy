
#include <avr/io.h>
#include <avr/interrupt.h>
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
	can_message_raw *cmsg;

	switch(msg->cmd) {
		case RS232CAN_SETFILTER:
			break;
		case RS232CAN_SETMODE:
			can_mode = (can_mode_t)(msg->data[0]);
			can_setmode(can_mode);
			break;
		case RS232CAN_PKT:
			cmsg = can_buffer_get_raw();
			rs232can_rs2can(cmsg, msg);
			can_transmit_raw(cmsg);
			break;
	}
}

void process_can_msg(can_message_raw *msg){
	rs232can_msg rmsg;

	rs232can_can2rs(&rmsg, msg);
	canu_transmit(&rmsg);
}

void send_error( unsigned char status )
{
	rs232can_msg rmsg;

	rmsg.cmd     = RS232CAN_ERROR;
	rmsg.len     = 1;
	rmsg.data[0] = status;

	canu_transmit(&rmsg);
}


#define PORT_LEDS PORTD
#define DDR_LEDS DDRD
#define PIN_LEDCL PD5
#define PIN_LEDCK PD6
#define PIN_LEDD PD7


#define PORT_BUSPOWER PORTD
#define DDR_BUSPOWER DDRD
#define BIT_BUSPOWER PD4

void buspower_on(){
	DDR_BUSPOWER |= (1<<BIT_BUSPOWER);
	PORT_BUSPOWER |= (1<<BIT_BUSPOWER);
}

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

void adc_init(){
	DDRC = 0xCF;
	ADMUX = 0;
	ADCSRA = 0x07; //slowest adc clock
	ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADIF);
}



int main(){
	unsigned int x;
	for(x=0;x<60000;x++){
		asm volatile ("nop");
	};
	
	uart_init();
	spi_init();
	
	//adc_init();
	
	led_init();	
	led_set(0xFFFF);
	
	buspower_on();
	
	can_init();
	
	sei();

	canu_reset();
	can_setmode(normal);
	
	can_setled(0,1);
	
	unsigned int muh=0x01;
	
		
	while(1) {
		rs232can_msg *rmsg;
		can_message_raw  *cmsg;


		rmsg = canu_get_nb();
		if (rmsg){ 
			led_set(muh);
			if ((muh<<=1)==0) muh = 1;
			process_rs232_msg(rmsg);
		}
		
		cmsg = can_get_raw_nb();
		if (cmsg){
			process_can_msg(cmsg);
			can_free_raw(cmsg);
		}

		if (can_error) {
			send_error(can_error);
			can_error = 0;
		}
	}

	return 0;
}
