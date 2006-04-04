
#include <avr/io.h>
#include "serial.h"

void InitSerial0(uint16_t ubrr){
	UBRR0L = ubrr;
	UBRR0H = (uint8_t)(ubrr>>8);
	UCSR0A = (BAUDX == 8)?(1<<U2X0):0;
	UCSR0B = ((1<<TXEN0) | (1<<RXEN0) );

	UCSR0C = (1<<URSEL0) | (1<<UCSZ01) | (1<<UCSZ00);

}


int put_c0(char c){
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = c;
	return 0;
}
