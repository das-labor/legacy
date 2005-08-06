//demo.c
//This is a simple Demo for the Labor Board.

#include <inttypes.h>
#include <avr/io.h>

#include "util.h"

int main (void)
{	
	DDRA  = 0xFF; 	// Port A all outputs
	DDRB  = 0xFF; 	// Port B all outputs
	DDRC  = 0xFF; 	// Port C all outputs
	DDRD  = 0xFF; 	// Port D all outputs
	
	for ( ;; ){		//for ever
		// All Pins to VCC
		PORTA = 0xFF;
		PORTB = 0xFF;  
		PORTC = 0xFF; 
		PORTD = 0xFF;   

		wait(1000);

		// All Pins to low
		PORTA = 0x00;
		PORTB = 0x00;  
		PORTC = 0x00; 
		PORTD = 0x00;   

		wait(1000);
	};	
}
