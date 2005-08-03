#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "util.h"
#include "spi.h"
#include "can.h"
#include "lap.h"
#include "dimmer.h"

#define NUM_LAMPS 4
#define NUM_LAMPE 4
#define PORT_LEDS PORTD

#ifdef DEBUG
 #include "uart.h"
#endif


#define stdout_putc     uart_putc
#define stdout_putstr   uart_putstr
#define stdout_putstr_P uart_putstr_P

void set_lampe(unsigned char lampe, unsigned char val)
{
	Bright[lampe] = 64-(val>>2);
}

#ifdef DEBUG
void hex_dump(unsigned char * addr, unsigned char size){
	unsigned char x=0, sbuf[3];
	
	while(size--){
		itoa(*addr++, sbuf, 16);
		if (sbuf[1] == 0) stdout_putc(' ');
		stdout_putstr(sbuf);
		stdout_putc(' ');
		if(++x == 16){
			stdout_putstr_P(PSTR("\r\n"));
			x = 0;
		}
	}
}
#endif

#define LED_B1 0xF0
#define LED_B2 0xCC
#define LED_B3 0xAA
#define LED_B4 0xC0
#define LED_B5 0xCC
#define LED_B6 0x80

unsigned char FLAGS;
#define F_RCV_CAN 0x01
#define F_MODE_ON 0x02

#define LED_GREEN PD1

void blink_leds(){
	static unsigned char rol;
	if(TIFR & (1<<TOV1)){
		TIFR = (1<<TOV1); //clear flag
		if((rol>>=1) == 0) rol = 0x80;
		if(FLAGS & F_RCV_CAN){
			FLAGS &= ~ F_RCV_CAN;
			PORT_LEDS ^= (1<<LED_GREEN);
		}else{
			if(FLAGS & F_MODE_ON){
				PORT_LEDS |= (1<<LED_GREEN);
			}else{
				if(rol & LED_B4){
					PORT_LEDS |= (1<<LED_GREEN);
				}else{
					PORT_LEDS &= ~(1<<LED_GREEN);
				}
			}
		}
	}
}

void relais_check(){
	unsigned char x, on = 0;
	for(x=0;x<NUM_LAMPS;x++){
		if(Bright[x] != 64){
			on = 1;
		}
	}
	if(on){
		FLAGS |= F_MODE_ON;
		PORTD |=  0x10;
	}else{
		FLAGS &= ~F_MODE_ON;
		PORTD &= ~0x10;
	}

}

void eventloop()
{
	while(1) {
		can_message *msg = can_get_nb();
		if(msg){
			FLAGS |= F_RCV_CAN;
			switch(msg->port_dest) {
			case PORT_MGT:
				// Management
				switch(((pdo_message*)msg)->fkt_id) {
				case FKT_MGT_PING:
					// send pong
					break;
				case FKT_MGT_RESET:
					// reboot
					break;
				}
			case PORT_LAMPE:
				// Lampen steuern
				switch(((pdo_message*)msg)->fkt_id) {
				case FKT_LAMPE_SET: {
					if (msg->dlc != 4) continue;
					unsigned char value;
					unsigned char lampe;
					lampe = ((pdo_message*)msg)->data[0];
					value = ((pdo_message*)msg)->data[1];
	
					set_lampe(lampe, value);
					break;
				}
				case FKT_LAMPE_SETMASK: {
					unsigned char i;
					if (msg->dlc != NUM_LAMPE+2) continue;
					for(i=0; i<NUM_LAMPE; i++) {
						unsigned char value = ((pdo_message*)msg)->data[i];
						set_lampe(i, value);
					}
					break;
				}
				}
			}
		}
		blink_leds();
		relais_check();
	}
}


/*
void testing()
{
	while(1) {
		pdo_message *msg = (pdo_message *) can_buffer_get();
		msg->addr_src  = 0x00;
		msg->addr_dest = 0xaa;
		msg->port_src  = 0x00;
		msg->port_dest = PORT_LAMPE;
		msg->dlc = 4;
		msg->fkt_id = FKT_LAMPE_SET; 
		msg->data[0] = 6;
		msg->data[1] = 255;
		msg->flags = 0x01;

		can_transmit();

		wait(1000);
	}
}
*/

void timer1_init(){
	ICR1 = 8000;
	TCCR1B = 0x02 ;//| (1<<CTC1); //CTC Mode, clk/1024
	TCNT1 = 0;
}



int main(){
	DDRD = 0x73;
	DDRB = 0x0F;//rest will be set by spi_init
	PORTD = 0x12;
	
	timer1_init();
	
	spi_init();
	can_init();

#ifdef DEBUG
	uart_init();
	uart_putstr("\n<LAMPE>\n");
#endif

	dimmer_init();

	sei();

	
	
	eventloop();
	return 0;
//	testing();
}

