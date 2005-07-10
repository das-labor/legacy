
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "util.h"
#include "uart.h"
#include "spi.h"
#include "can.h"
//#include "console.h"
#include "lap.h"

#define NUM_LAMPE 4
#define FKT_PING 0
#define FKT_REBOOT 1


#define stdout_putc     uart_putc
#define stdout_putstr   uart_putstr
#define stdout_putstr_P uart_putstr_P

typedef enum { PORT_MGT=0x30, PORT_LAMPE=0x20 }   ports;
typedef enum { FKT_MGT_PING=0x00, FKT_MGT_REBOOT=0x01 }     fkts_mgt;
typedef enum { FKT_LAMPE_SET=0x00, FKT_LAMPE_SETMASK=0x01 } fkts_lampe;

void set_lampe(unsigned char lampe, unsigned char val)
{
	PORTC ^= (1<<8);
	if (val) {
		PORTC |= (1<<lampe);
	} else {
		PORTC &= ~(1<<lampe);
	}
}

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


void eventloop()
{
	while(1) {
		can_message *msg= can_get();
		PORTC ^= (1<<7);

		hex_dump((char *)msg, 10);

		switch(msg->port_dest) {
		case PORT_MGT:
			// Management
			switch(((pdo_message*)msg)->fkt_id) {
			case FKT_MGT_PING:
				// send pong
				break;
			case FKT_MGT_REBOOT:
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
}

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
		msg->data[0] = 5;
		msg->data[1] = 255;
		msg->flags = 0x01;

		can_transmit();

		wait(1000);
	}


	
}


int main(){
	uart_init();
	spi_init();
	can_init();

	DDRC = 0xff;

	uart_putstr("\nLAMPE> ");

	sei();

//	eventloop();
	testing();
}
