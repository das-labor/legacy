
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

typedef enum { PORT_MGT=0x30, PORT_LAMPE=0x20 }   ports;
typedef enum { FKT_MGT_PING=0x00, FKT_MGT_REBOOT=0x01 }     fkts_mgt;
typedef enum { FKT_LAMPE_SET=0x00, FKT_LAMPE_SETMASK=0x01 } fkts_lampe;

void set_lampe(unsigned char lampe, unsigned char val)
{
	if (val) {
		PORTC |= (1<<lampe);
	} else {
		PORTC &= ~(1<<lampe);
	}
}

void hexdump(char *buf, unsigned char len)
{
	char *c = buf;
	char str[3];
	while(c < buf+len) {
		itoa(*c, str, 16);
		uart_putstr(str);
		c++;
	}
}


int main(){
	uart_init();
//	console_init();
	spi_init();
	can_init();

	uart_putstr("\nLAMPE> ");

	sei();
	DDRC = 0xff;

	while(1) {
		can_message *msg= can_get();

		hexdump((char *)msg, 10);

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
