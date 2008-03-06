

#include <avr/io.h>
#include "uart.h"
#include "bus_access.h"

#define BIT_OE PB4


void io_init(){
	DDRB = 0xbc;
	PORTB |= (1<<BIT_OE);
	
	DDRA = 0x80;
	
	//pullups on on data lines
	PORTC = 0xff;
	PORTD |= 0x80;

}

uint16_t geti(){
	uint16_t i=0;
	i = uart_getc() << 8;
	i |= (uint8_t)uart_getc();
	return i;
}

int main(){

	io_init();
	spi_init();
	
	uart_init();
	
	while(1){
		uint8_t c;
		c = uart_getc();
		switch(c){
			case 'd':{
				uint16_t start, end, i;
				start = geti();
				end = geti();
				for(i=start;i != end;i++){
					uart_putc(bus_read(i));
				}
			
			}
			break;
			case 'w':{
				uint16_t start, end, i;
				start = geti();
				end = geti();
				for(i=start;i != end;i++){
					bus_write(i, uart_getc());
				}
			
			}
			break;		
		}
	}
}
