/**
 * UART Echo Test
 */



#include <inttypes.h>
#include <avr/io.h>
#include "uart.h"

int main (void)
{	
	DDRC = 0xFF; 	//Port C all outputs
	PORTC = 0x01;   //1 LED on to start of Patterns
	
	uart_init();
	
	for ( ;; ){		//for ever
		uart_putc(uart_getc());

		if( PORTC == 0x80 ) { 
			PORTC = 0x01
		} else {
			PORTC <<= 1;
		}
	};	
}
