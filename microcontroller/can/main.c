
#include <avr/io.h>
#include <stdlib.h>
#include "uart.h"
#include "spi.h"
#include "can.h"

int main(){
	uart_init();
	spi_init();
	can_init();

	uart_putc('*');

	while(1) {
		ping();
		char *rcv = can_rcvpacket();
		uart_putstr(rcv);
	}

	return 0;
}
