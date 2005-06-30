//demo.c
//This is a simple Demo for the Labor Board.
//It displays 2 different light patterns on the LEDs,
//and allows the pattern to be selected, and the speed to be controled
//with the 4 keys. 

#include <inttypes.h>
#include <avr/io.h>
#include "uart.h"
#include "util.h"

int main (void)
{	
	DDRB = 0; 	//Port B all inputs
	PORTB = 0x0F;	//Internal Pullups on on key inputs
	
	DDRC = 0xFF; 	//Port C all outputs
	PORTC = 0x00;   //1 LED on to start of Patterns
	
	uart_init();
	uart_putstr( "\n" );
	uart_putstr( "**********************************\n" );
	uart_putstr( "*** INTERRUPT DRIVEN UART TEST ***\n" );
	uart_putstr( "*\n" );
	uart_putstr( "* PORTC.LED1: TX interrupt\n" );
	uart_putstr( "* PORTC.LED2: RX interrupt\n" );
	uart_putstr( "* PORTC.LED5: Mainloop step\n" );
	uart_putstr( "*\n" );
	uart_putstr( "ECHO TEST> " );
	
	for ( ;; ){		//for ever
		uart_putc(uart_getc());   // Echo char
		PORTC ^= 0x10;            // Toggle LED
	};	
}
