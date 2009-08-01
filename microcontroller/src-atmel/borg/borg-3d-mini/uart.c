/* USART-Init beim ATmegaXX */
#ifndef F_CPU
#define F_CPU 16000000           /* Oszillator-Frequenz in Hz */
#endif

#define UART_BAUD_RATE 19200
#define UART_BAUD_CALC(UART_BAUD_RATE,F_OSC) ((F_CPU)/((UART_BAUD_RATE)*16L)-1)

#include <avr/io.h>


void uart_init() {
	PORTD |= 0x01;					//Pullup an RXD an

	UCSRB |= (1<<TXEN);				//UART TX einschalten
	UCSRC |= (1<<URSEL)|(3<<UCSZ0);	//Asynchron 8N1

	UCSRB |= ( 1 << RXEN );			//Uart RX einschalten

	UBRRH  = (uint8_t) (UART_BAUD_CALC(UART_BAUD_RATE,F_CPU) >> 8);
	UBRRL  = (uint8_t)  UART_BAUD_CALC(UART_BAUD_RATE,F_CPU);
}

void uart_putc(uint8_t c){
	while (!(UCSRA & (1<<UDRE))); /* warten bis Senden moeglich                   */
	UDR = c;                      /* schreibt das Zeichen x auf die Schnittstelle */

}

void uart_putstr(char *p) {
	while (*p != 0) {
		uart_putc(*p++);

	}
}

/* Zeichen empfangen */
uint8_t uart_getc() {
    while (!(UCSRA & (1<<RXC)));  // warten bis Zeichen verfuegbar
    return UDR;                   // Zeichen aus UDR an Aufrufer zurueckgeben
}
