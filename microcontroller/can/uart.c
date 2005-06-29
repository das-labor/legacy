/* USART-Init beim ATmegaXX */
#ifndef F_CPU
#define F_CPU 16000000           /* Oszillator-Frequenz in Hz */
#endif
#define UART_BAUD_RATE 19200
#define UART_BAUD_CALC(UART_BAUD_RATE,F_OSC) ((F_CPU)/((UART_BAUD_RATE)*16L)-1)

#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>

#define UART_BUFSIZE 10

volatile static char rxbuf[UART_BUFSIZE];
volatile static char txbuf[UART_BUFSIZE];
volatile static char *volatile rxhead, *volatile rxtail;
volatile static char *volatile txhead, *volatile txtail;


SIGNAL(SIG_UART_DATA) {
	PORTC |= 0x01;
	PORTC ^= 0x02;

	if ( txhead == txtail ) {
		UCSRB &= ~(1 << UDRIE);		/* disable data register empty IRQ */
	} else {
		UDR = *txtail;                   /* schreibt das Zeichen x auf die Schnittstelle */
		if (++txtail == (txbuf + UART_BUFSIZE)) txtail = txbuf;
	}
}

void uart_init() {
	PORTD |= 0x01;				//Pullup an RXD an

	UCSRB |= (1<<TXEN);			//UART TX einschalten
	UCSRC |= (1<<URSEL)|(3<<UCSZ0);		//Asynchron 8N1

	UCSRB |= ( 1 << RXEN );			//Uart RX einschalten

	UBRRH=(uint8_t)(UART_BAUD_CALC(UART_BAUD_RATE,F_CPU)>>8);
	UBRRL=(uint8_t)UART_BAUD_CALC(UART_BAUD_RATE,F_CPU);

	// init buffers
	rxhead = rxtail = rxbuf;
	txhead = txtail = txbuf;
}

void uart_putc(char c) {
	volatile int diff ;

	/* buffer full? */
	do {
	cli();
		diff = txhead - txtail;
	sei();
		if ( diff < 0 ) diff += UART_BUFSIZE;
	} while ( diff >= UART_BUFSIZE -1 );

	cli();
	*txhead = c;
 	if (++txhead == (txbuf + UART_BUFSIZE)) txhead = txbuf;

	UCSRB |= (1 << UDRIE);		/* enable data register empty IRQ */
	sei();
}

void uart_putstr(char *str) {
	while(*str) {
		uart_putc(*str++);
	}
}

//
//void uart_putc(uint8_t c){
//	while (!(UCSRA & (1<<UDRE))); /* warten bis Senden moeglich                   */
//	UDR = c;                      /* schreibt das Zeichen x auf die Schnittstelle */
//
//}


/* Zeichen empfangen */
uint8_t uart_getc(){
    while (!(UCSRA & (1<<RXC)));  // warten bis Zeichen verfuegbar
    return UDR;                   // Zeichen aus UDR an Aufrufer zurueckgeben
}

