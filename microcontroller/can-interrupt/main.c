
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "util.h"
#include "uart.h"
#include "spi.h"
#include "can.h"

int main(){
	uart_init();
	spi_init();
	can_init();

	sei();
	DDRC = 0xff;

	uart_putc('*');

	while(1) {
		char c = uart_getc();
		can_send(&c, 1);

//		uart_putstr("Ohne M4te? 0xff 0x88 0x44 0x22 0x11\n");
//
		
//		uart_putc(c);
//		char *rcv = can_rcvpacket();
//		uart_putstr(rcv);
	}

	return 0;
}
