/* USART-Init beim ATmegaXX */
/* modified by siro 25.4.2010 to add atmega48 support */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "uart.h"
#include <util/setbaud.h>

#if defined(__AVR_ATmega128__)
	#ifndef USE_UART_1
		//UART 0 is default
		#define UCSRA UCSR0A
		#define UCSRB UCSR0B
		#define UCSRC UCSR0C
		#define UDR UDR0
		#define UBRRH UBRR0H
		#define UBRRL UBRR0L
		#define URSEL UMSEL
		#define UART_UDRE_VECTOR USART0_UDRE_vect
		#define UART_RECV_VECTOR USART0_RX_vect
	#else
		//UART 1
		#define UCSRA UCSR1A
		#define UCSRB UCSR1B
		#define UCSRC UCSR1C
		#define UDR UDR1
		#define UBRRH UBRR1H
		#define UBRRL UBRR1L
		#define URSEL UMSEL
		#define UART_UDRE_VECTOR USART1_UDRE_vect
		#define UART_RECV_VECTOR USART1_RX_vect
	#endif
#elif defined(__AVR_ATmega48__) | defined(__AVR_ATmega168__) | defined(__AVR_ATmega644__) | defined(__AVR_ATmega644P__)
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
	#define UART_UDRE_VECTOR USART0_UDRE_vect
	#define UART_RECV_VECTOR USART0_RX_vect
#elif defined(__AVR_ATmega162__)
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
	#define UCSZ1 UCSZ01
	#define RXCIE RXCIE0
	#define TXCIE TXCIE0
	#define RXC RXC0
	#define UDRE UDRE0
	#define UART_UDRE_VECTOR USART0_UDRE_vect
	#define UART_RECV_VECTOR USART0_RXC_vect
	#define URSEL URSEL0
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

#ifdef STDFUNCT_IO
FILE uart_output = FDEV_SETUP_STREAM(uart_putc, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getc, _FDEV_SETUP_READ);
void uart_putcs(char c, FILE *stream);
char uart_getcs(FILE *stream);
#endif // STDFUNCT_IO

#ifdef UART_INTERRUPT
static volatile char rxbuf[UART_RXBUFSIZE];
static volatile char txbuf[UART_TXBUFSIZE];
static volatile char *volatile rxhead, *volatile rxtail;
static volatile char *volatile txhead, *volatile txtail;

ISR(UART_UDRE_VECTOR)
{
	if (txhead == txtail) {
#ifdef UART_LEDS
		PORTC ^= 0x01;
#endif
		UCSRB &= ~(1 << UDRIE);		// disable data register empty IRQ
	} else {
		UDR = *txtail;			// send char
		if (++txtail == (txbuf + UART_TXBUFSIZE))
			txtail = txbuf;
	}
}

ISR(UART_RECV_VECTOR)
{
	int diff;

#ifdef UART_LEDS
	PORTC ^= 0x02;
#endif

	/* buffer full? */
	diff = rxhead - rxtail;
	if (diff < 0)
		diff += UART_RXBUFSIZE;
	if (diff < UART_RXBUFSIZE - 1) {// buffer NOT full
		*rxhead = UDR;
		if (++rxhead == (rxbuf + UART_RXBUFSIZE))
			rxhead = rxbuf;
	} else {
		UDR;			// reads the buffer to clear the interrupt condition
	}
}
#endif // UART_INTERRUPT

void uart_init(void)
{
#ifdef UART_LEDS
	DDRC |= 0x03;			// Port C LED outputs
#endif
	PORTD |= 0x01;			// Pullup on RXD

	UCSRB = _BV(TXEN) | _BV(RXEN);	// enable UART RX und TX

	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;

#if USE_2X
	UCSRA = _BV(U2X);
#else
	UCSRA = 0;
#endif

#ifdef URSEL
	UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);	// Asynchron 8N1
#else
	UCSRC = _BV(UCSZ1) | _BV(UCSZ0);
#endif

#ifdef UART_INTERRUPT
	// init buffers
	rxhead = rxtail = rxbuf;
	txhead = txtail = txbuf;

	// activate rx IRQ
	UCSRB |= _BV(RXCIE);
#endif // UART_INTERRUPT
#ifdef STDFUNCT_IO
	stdout = &uart_output;
	stdin  = &uart_input;
#endif // STDFUNCT_IO
}


#ifdef STDFUNCT_IO
void uart_putcs(char c, FILE *stream)
{
	(void) stream;
	uart_putc(c);
}
#endif // STDFUNCT_IO

void uart_putc(char c)
{
#ifdef UART_INTERRUPT
	volatile int diff;

	// buffer full?
	do {
		diff = txhead - txtail;
		if (diff < 0)
			diff += UART_TXBUFSIZE;
	} while (diff >= UART_TXBUFSIZE - 1);

	cli();
	*txhead = c;
	if (++txhead == (txbuf + UART_TXBUFSIZE))
		txhead = txbuf;

	UCSRB |= _BV(UDRIE);		// enable data register empty IRQ
	sei();
#else  // WITHOUT INTERRUPT
	while (!(UCSRA & _BV(UDRE)));	// wait for empty tx reg
	UDR = c;			// send char
#endif // UART_INTERRUPT
}

void uart_putstr(char *str)
{
	while (*str)
		uart_putc(*str++);
}

void uart_putstr_P(PGM_P str)
{
	char tmp;
	while ((tmp = pgm_read_byte(str))) {
		uart_putc(tmp);
		str++;
	}
}

#ifdef STDFUNCT_IO
char uart_getcs(FILE *stream)
{
	(void) stream;
	return uart_getc();
}
#endif // STDFUNCT_IO

char uart_getc(void)
{
#ifdef UART_INTERRUPT
	char val;

	while (rxhead == rxtail);

	val = *rxtail;
	if (++rxtail == (rxbuf + UART_RXBUFSIZE))
		rxtail = rxbuf;

	return val;
#else // WITHOUT INTERRUPT
	while (!(UCSRA & (1 << RXC)));	// wait for char
	return UDR;			// return received char
#endif // UART_INTERRUPT
}


// returns 1 on success
#ifdef UART_INTERRUPT
char uart_getc_nb(char *c)
{
	if (rxhead == rxtail)
		return 0;

	*c = *rxtail;
	if (++rxtail == (rxbuf + UART_RXBUFSIZE))
		rxtail = rxbuf;

	return 1;
}
#else  // WITHOUT INTERRUPT
char uart_getc_nb(char *c)
{
	if (UCSRA & _BV(RXC)) {		// char received
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

	while (len--)
	{
		itoa(*buf++, sbuf, 16);
		if (sbuf[1] == 0) uart_putc(' ');
		uart_putstr(sbuf);
		uart_putc(' ');
		if (++x == 16)
		{
			uart_putstr_P(PSTR("\r\n"));
			x = 0;
		}
	}
}
#endif

#ifdef UART_GETLINE
// get one Cariage return terminated line
// echo charakters back on Uart
// returns buffer with zero terminated line on success, 0 pointer otherwise
char *uart_getline(void)
{
	static char buffer[UART_LINE_BUFFER_SIZE];
	static char *pos = buffer;
	char tmp;
	while ((tmp = uart_getc())) {
		if (tmp == '\r') {
			*pos = 0;	// terminate line
			pos = buffer;   // reset pointer
			return buffer;  // and return the buffer
		}
		if ((tmp != '\n') && (pos < buffer + UART_LINE_BUFFER_SIZE - 1)) { // buffer full?
			*pos++ = tmp;		// no: write character to buffer
			// uart_putc (tmp);
		}
	}
	return 0;
}
#endif


#ifdef UART_INTERRUPT
char uart_rxbuffer_notempty(void)
{
	if (rxhead == rxtail)
		return 0;
	else
		return 1;
}
#endif

#ifdef UART_INTERRUPT
#if UART_RXBUFSIZE < 255
uint8_t uart_getrxqueuelength(void) 
#else
uint16_t uart_getrxqueuelength(void) 
#endif
{
	return (rxhead - rxtail);
}
#endif
