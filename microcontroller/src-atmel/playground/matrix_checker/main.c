
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "util.h"


typedef struct{
	volatile uint8_t * port;
	uint8_t	bit;
}iopin;

iopin pins[24] ={
	{&PORTA, 0},
	{&PORTA, 1},
	{&PORTA, 2},
	{&PORTA, 3},
	{&PORTA, 4},
	{&PORTA, 5},
	{&PORTA, 6},
	{&PORTA, 7},
	{&PORTE, 0},
	{&PORTC, 4},
	{&PORTC, 3},
	{&PORTC, 2},
	{&PORTC, 1},
	{&PORTC, 0},
	{&PORTB, 0},
	{&PORTB, 1},
	{&PORTB, 2},
	{&PORTB, 3},
	{&PORTB, 4},
	{&PORTB, 5},
	{&PORTB, 6},
	{&PORTB, 7},
	{&PORTD, 2},
	{&PORTD, 3},
};

#define DDR(p) (p-1)
#define PIN(p) (p-2)

int main(){

	uart_init();
	sei();
	uart_putstr("matrix checker ready\r\n");

	wait(1000);
	
	uint8_t x,y;
	char buf[6];
	
	//set all Pins to Input and turn pullups on
	for(x=0;x<24;x++){
		*DDR(pins[x].port) &= ~(1<<pins[x].bit);
		*(pins[x].port) |= (1<<pins[x].bit);
	}
	
	while(1){
		for(x=0;x<24;x++){
			*(pins[x].port) &= ~(1<<pins[x].bit);
			*DDR(pins[x].port) |= (1<<pins[x].bit);

			wait(10);
			
			for(y=0;y<24;y++){
				
				if( (x != y) &&
					( !( (x == 1 && y ==16) | (x == 16 && y == 1 ) | (x == 17 && y == 23)      ) ) &&	
					( !((*PIN(pins[y].port)) & (1<<pins[y].bit)) ) ){
					uart_putstr_P(PSTR("row "));
					utoa(x,buf,10);
					uart_putstr(buf);
					uart_putstr_P(PSTR("\tcol "));
					utoa(y,buf,10);
					uart_putstr(buf);
					uart_putstr_P(PSTR("\r\n"));
				}
			}
			
			*DDR(pins[x].port) &= ~(1<<pins[x].bit);
			*(pins[x].port) |= (1<<pins[x].bit);
			
		}
	}
}

