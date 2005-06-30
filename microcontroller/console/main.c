//demo.c
//This is a simple Demo for the Labor Board.
//It displays 2 different light patterns on the LEDs,
//and allows the pattern to be selected, and the speed to be controled
//with the 4 keys. 

#include <inttypes.h>
#include <avr/io.h>
#include "uart.h"
#include "util.h"
#include "console.h"

int main (void)
{	
	DDRB = 0; 	//Port B all inputs
	PORTB = 0x0F;	//Internal Pullups on on key inputs
	
	DDRC = 0xFF; 	//Port C all outputs
	PORTC = 0x00;   //1 LED on to start of Patterns
	
	uart_init();
	uart_putc('*');
	
	
			
	for ( ;; ){		//for ever
		console();	
	};	
}

