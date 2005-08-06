/**
 * UART Echo Test
 */

#include <inttypes.h>
#include <avr/io.h>

#include "config.h"
#include "uart.h"

int main (void)
{	
	uart_init();

	DDRC = 0xFF; 	// Port C all outputs
	PORTC = 0x04;   // Start LED 3 -- LEDs1+2 machen interrupt debugging

	uart_putc('*');
	
	for ( ;; ){		//for ever
		uart_putc(uart_getc());

		PORTC ^= 0x04;
	};	
}
