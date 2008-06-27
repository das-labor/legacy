/* USART-Init beim ATmegaXX */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "uart.h"

#define UART_BAUD_RATE 19200
//#define UART_INTERRUPT
#define UART_RXBUFSIZE 50
#define UART_TXBUFSIZE 50


#define UART_BAUD_CALC(UART_BAUD_RATE,F_OSC) ((F_OSC)/((UART_BAUD_RATE)*16L)-1)


#ifdef UART_INTERRUPT
volatile static char rxbuf[UART_RXBUFSIZE];
volatile static char txbuf[UART_TXBUFSIZE];
volatile static char *volatile rxhead, *volatile rxtail;
volatile static char *volatile txhead, *volatile txtail;


SIGNAL(SIG_UART_DATA) {
#ifdef UART_LEDS	
	PORTC ^= 0x01;
#endif
	
	if ( txhead == txtail ) {
		UCSRB &= ~(1 << UDRIE);		/* disable data register empty IRQ */
	} else {
		UDR = *txtail;			/* schreibt das Zeichen x auf die Schnittstelle */
		if (++txtail == (txbuf + UART_TXBUFSIZE)) txtail = txbuf;
	}
}

SIGNAL(SIG_UART_RECV) {
	int diff; 

#ifdef UART_LEDS
	PORTC ^= 0x02;
#endif
	
	/* buffer full? */
	diff = rxhead - rxtail;
	if ( diff < 0 ) diff += UART_RXBUFSIZE;
	if (diff < UART_RXBUFSIZE -1) {
		// buffer NOT full
		*rxhead = UDR;
		if (++rxhead == (rxbuf + UART_RXBUFSIZE)) rxhead = rxbuf;
	} else {
		UDR;				//reads the buffer to clear the interrupt condition
	}
}

#endif // UART_INTERRUPT


void uart_init() {
	PORTD |= 0x01;				//Pullup an RXD an

	UCSRB |= (1<<TXEN);			//UART TX einschalten
	UCSRC |= (1<<URSEL)|(3<<UCSZ0);		//Asynchron 8N1

	UCSRB |= ( 1 << RXEN );			//Uart RX einschalten

	UBRRH=(uint8_t)(UART_BAUD_CALC(UART_BAUD_RATE,F_CPU)>>8);
	UBRRL=(uint8_t)(UART_BAUD_CALC(UART_BAUD_RATE,F_CPU));

#ifdef UART_INTERRUPT
	// init buffers
	rxhead = rxtail = rxbuf;
	txhead = txtail = txbuf;

	// activate rx IRQ
	UCSRB |= (1 << RXCIE);
#endif // UART_INTERRUPT
}

#ifdef UART_INTERRUPT
void uart_putc(char c) {
	volatile int diff;

	/* buffer full? */
	do {
		diff = txhead - txtail;
		if ( diff < 0 ) diff += UART_TXBUFSIZE;
	} while ( diff >= UART_TXBUFSIZE -1 );

	cli();
	*txhead = c;
 	if (++txhead == (txbuf + UART_TXBUFSIZE)) txhead = txbuf;

	UCSRB |= (1 << UDRIE);		/* enable data register empty IRQ */
	sei();
}
#else  // WITHOUT INTERRUPT
void uart_putc(char c) {
	while (!(UCSRA & (1<<UDRE))); /* warten bis Senden moeglich                   */
	UDR = c;                      /* schreibt das Zeichen x auf die Schnittstelle */
}
#endif // UART_INTERRUPT


void uart_putstr(char *str) {
	while(*str) {
		uart_putc(*str++);
	}
}

void uart_putstr_P(PGM_P str) {
	char tmp;
	while((tmp = pgm_read_byte(str))) {
		uart_putc(tmp);
		str++;
	}
}

void uart_hexdump(char *buf, int len)
{
	unsigned char x=0;
	char sbuf[3];

	while(len--){
		itoa(*buf++, sbuf, 16);
		if (sbuf[1] == 0) uart_putc(' ');
		uart_putstr(sbuf);
		uart_putc(' ');
		if(++x == 16) {
			uart_putstr_P(PSTR("\r\n"));
			x = 0;
		}
	}
}


#ifdef UART_INTERRUPT
char uart_getc()
{
	char val;

	while(rxhead==rxtail) ;

	val = *rxtail;
 	if (++rxtail == (rxbuf + UART_RXBUFSIZE)) rxtail = rxbuf;

	return val;
}
#else  // WITHOUT INTERRUPT
char uart_getc()
{
	while (!(UCSRA & (1<<RXC)));	// warten bis Zeichen verfuegbar
	return UDR;			// Zeichen aus UDR zurueckgeben
}
#endif // UART_INTERRUPT

// returns 1 on success
#ifdef UART_INTERRUPT
char uart_getc_nb(char *c)
{
	if (rxhead==rxtail) return 0;

	*c = *rxtail;
 	if (++rxtail == (rxbuf + UART_RXBUFSIZE)) rxtail = rxbuf;

	return 1;
}
#else  // WITHOUT INTERRUPT
char uart_getc_nb(char *c)
{
	if (UCSRA & (1<<RXC)) {		// Zeichen verfuegbar
		*c = UDR;
		return 1;
	}

	return 0;
}
#endif // UART_INTERRUPT
