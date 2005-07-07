
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "util.h"
#include "uart.h"
#include "spi.h"
#include "can.h"
#include "console.h"

int main(){
	uart_init();
	console_init();
	spi_init();
	can_init();

	sei();
	DDRC = 0xff;

	uart_putc('*');

	while(1) {
		console();
	}
	return 0;
}
