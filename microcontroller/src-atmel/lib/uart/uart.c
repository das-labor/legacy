/* USART-Init beim ATmegaXX */
/* modified by siro 25.4.2010 to add atmega48 support */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "uart.h"

#if defined(__AVR_ATmega128__)
	#define UCSRB UCSR0B
	#define UCSRC UCSR0C
	#define UDR UDR0
	#define UBRRH UBRR0H
	#define UBRRL UBRR0L
	#define URSEL UMSEL
	#define UART_UDRE_VECTOR SIG_UART_DATA
	#define UART_RECV_VECTOR SIG_UART_RECV
#elif defined(__AVR_ATmega48__) | defined(__AVR_ATmega168__) | defined(__AVR_ATmega162__)
	#define UCSRB UCSR0B
	#define UCSRC UCSR0C
	#define UDR UDR0
	#define UBRRH UBRR0H
	#define UBRRL UBRR0L
	#define TXEN TXEN0
	#define RXEN RXEN0
	#define UCSRA UCSR0A
	#define UDRIE UDRIE0
	#define UCSZ0 UCSZ00
	#define RXCIE RXCIE0
	#define TXCIE TXCIE0
	#define RXC RXC0
	#define UDRE UDRE0
	#define UART_UDRE_VECTOR USART_UDRE_vect
	#define UART_RECV_VECTOR USART_RX_vect
#elif defined(__AVR_ATmega8__) | defined(__AVR_ATmega32__) 
	#define UART_UDRE_VECTOR USART_UDRE_vect
	#define UART_RECV_VECTOR USART_RXC_vect
#else
	#define UART_UDRE_VECTOR USART_UDRE_vect
	#define UART_RECV_VECTOR USART_RXC_vect

	#warning Your AVR is not officially supported by the uart-lib, it might not work.
#endif





#ifdef UART_LEDS
	#warning UART_LEDS is truned on, which means that PC0 and PC1 will be toggled by receiving/transmitting bytes!!
#endif


#define UART_BAUD_CALC(UART_BAUD_RATE,F_OSC) ((F_OSC+(UART_BAUD_RATE)*8L)/((UART_BAUD_RATE)*16L)-1)


#ifdef UART_INTERRUPT
volatile static char rxbuf[UART_RXBUFSIZE];
volatile static char txbuf[UART_TXBUFSIZE];
volatile static char *volatile rxhead, *volatile rxtail;
volatile static char *volatile txhead, *volatile txtail;

ISR(UART_UDRE_VECTOR) {
	if ( txhead == txtail ) {
#ifdef UART_LEDS	
	PORTC ^= 0x01;
#endif
	
	UCSRB &= ~(1 << UDRIE);		/* disable data register empty IRQ */
	} else {
		UDR = *txtail;			/* schreibt das Zeichen x auf die Schnittstelle */
		if (++txtail == (txbuf + UART_TXBUFSIZE)) txtail = txbuf;
	}
}

ISR(UART_RECV_VECTOR) {
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
#ifdef UART_LEDS	
	DDRC |= 0x03; 	// Port C LED outputs
#endif
	PORTD |= 0x01;				//Pullup an RXD an

	UCSRA = 0;
	UCSRB = (1<<TXEN) | ( 1 << RXEN); //UART RX und TX einschalten
	UCSRC = (3<<UCSZ0);		//Asynchron 8N1


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


#ifdef UART_HEXDUMP
//hexdump utility
void uart_hexdump(unsigned char *buf, int len)
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
#endif
