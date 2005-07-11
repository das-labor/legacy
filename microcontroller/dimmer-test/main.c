//demo.c
//This is a simple Demo for the Labor Board.
//It displays 2 different light patterns on the LEDs,
//and allows the pattern to be selected, and the speed to be controled
//with the 4 keys. 

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
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
	sei();
	
	//uart_putc('*');
	
	//char buf[10];
	//itoa(uart_getc(), buf, 16);
	//uart_putstr(buf);
	
	
	
			
	for ( ;; ){		//for ever
		console();	
	};	
}

void timer1_init(){
	

}



void dimmer_test(){
	
	while(1){
		PORTC |= 1;
		wait(20);
		PORTC &= ~1;
		wait(20);
 	}
 }
 