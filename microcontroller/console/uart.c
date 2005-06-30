/* USART-Init beim ATmegaXX */
#ifndef F_CPU
#define F_CPU 16000000           /* Oszillator-Frequenz in Hz */
#endif
#define UART_BAUD_RATE 19200
#define UART_BAUD_CALC(UART_BAUD_RATE,F_OSC) ((F_CPU)/((UART_BAUD_RATE)*16L)-1)

#define UART_LEDS           /* LED1 and LED2 toggle on tx and rx interrupt */

#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include "uart.h"

#define UART_RXBUFSIZE 16
#define UART_TXBUFSIZE 16
#define LINE_BUFFER_SIZE 40

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
		UDR = *txtail;                   /* schreibt das Zeichen x auf die Schnittstelle */
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
		volatile char c = UDR;
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

	// activate rx IRQ
	UCSRB |= (1 << RXCIE);		/* disable data register empty IRQ */
}

void uart_putc(char c) {
	volatile int diff ;

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

char uart_getc()
{
	char val;

	while(rxhead==rxtail) ;

	val = *rxtail;
 	if (++rxtail == (rxbuf + UART_RXBUFSIZE)) rxtail = rxbuf;

	return val;
}

//returns 1 on success
unsigned char uart_getc_nb(char *c)
{
	if (rxhead==rxtail) return 0;

	*c = *rxtail;
 	if (++rxtail == (rxbuf + UART_RXBUFSIZE)) rxtail = rxbuf;

	return 1;
}

//get one Cariage return terminated line
//echo charakters back on Uart
//returns buffer with zero terminated line on success, 0 pointer otherwise
char * uart_getline_nb(){
	static char buffer[LINE_BUFFER_SIZE];
	static char * pos = buffer;
	char tmp;
	while(uart_getc_nb(&tmp)){
		if(tmp == '\r'){
			*pos = 0;	//terminate line
			pos = buffer;   //reset pointer
			return buffer;  //and return the buffer
		}
		if(pos < buffer+LINE_BUFFER_SIZE-1){ //buffer full?
			*pos++ = tmp;		//no: write character to buffer
			uart_putc (tmp);
		}
	}
	return 0;
}
