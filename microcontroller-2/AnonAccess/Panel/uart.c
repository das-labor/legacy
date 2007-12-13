/* USART-Init beim ATmegaXX */

#include "config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "uart.h"

#ifdef ATMEGA128
#define UCSRB UCSR0B
#define UCSRC UCSR0C
#define UDR UDR0
#define UBRRH UBRR0H
#define UBRRL UBRR0L
#define URSEL UMSEL
#endif

#ifdef ATMEGA644
#define UCSRB UCSR0B
#define UCSRC UCSR0C
#define UDR UDR0
#define UBRRH UBRR0H
#define UBRRL UBRR0L
#define URSEL UMSEL00
#define SIG_UART_DATA USART0_UDRE_vect
#define SIG_UART_RECV USART0_RX_vect
#define UDRIE UDRIE0
#define TXEN TXEN0
#define UMSEL UMSEL0
#define RXEN RXEN0
#define RXCIE RXCIE0
#define UCSZ0 UCSZ00
#define UCSRA UCSR0A
#define UDRE UDRE0
#define RXC RXC0
#endif


#ifdef UART_XON_XOFF
	#define DC1 0x11
	#define DC3 0x13
	#define XON  DC1
	#define XOFF DC3
#endif

#ifdef UART_XON_XOFF
 #ifdef UART_INTERRUPT
  void uart_insertc(char c);
 #else
  #define uart_insertc uart_putc
 #endif /* UART_INTERRUPT */
#endif

#define UART_BAUD_CALC(UART_BAUD_RATE,F_OSC) ((F_OSC)/((UART_BAUD_RATE)*16L)-1)

#ifdef UART_XON_XOFF
	typedef enum{go=1,nogo=0} gonogo;
	static gonogo txon=go;
	static gonogo rxon=go;
#endif

#ifdef UART_INTERRUPT
volatile static char rxbuf[UART_RXBUFSIZE];
volatile static char txbuf[UART_TXBUFSIZE];
volatile static char *volatile rxhead, *volatile rxtail;
volatile static char *volatile txhead, *volatile txtail;

#ifdef UART_HOOK	
	 void (*uart_hook) (uint8_t) = (void*)0;	/* this is a pointer to a function ;-) */
#endif

ISR(SIG_UART_DATA) {
#ifdef UART_LEDS	
	PORTC ^= 0x01;
#endif
	
	if ( txhead == txtail ) {
		UCSRB &= ~(1 << UDRIE);		/* disable data register empty IRQ */
	} else {
		#ifdef UART_XON_XOFF
			while(txon==nogo);
		#endif
		UDR = *txtail;			/* schreibt das Zeichen x auf die Schnittstelle */
		if (++txtail == (txbuf + UART_TXBUFSIZE)) txtail = txbuf;
	}
}

ISR(SIG_UART_RECV) {
	int diff; 
	char c;
#ifdef UART_HOOK	
	static volatile uint8_t hook_running=0;
#endif
#ifdef UART_LEDS
	PORTC ^= 0x02;
#endif
	c=UDR;
	#ifdef UART_XON_XOFF
	 if (c==XON)  txon=go;
	 if (c==XOFF) txon=nogo;
	#endif
	/* buffer full? */
	diff = rxhead - rxtail;
	if ( diff < 0 ) diff += UART_RXBUFSIZE; /* diff is the amount of bytes in buffer */
	if (diff < UART_RXBUFSIZE -1) {
		// buffer NOT full
#ifdef UART_HOOK
		if(!hook_running && uart_hook){
			uint8_t t=c;	
			hook_running = 1;
			sei(); /* reenable interrupts, avoid recursion!!! */
			do {
				uart_hook(t);
			} while(uart_getc_nb((char*)&t));
			hook_running = 0;
		} else {
			*rxhead = c;
			if (++rxhead == (rxbuf + UART_RXBUFSIZE)) rxhead = rxbuf;
		}
#else	
		*rxhead = c;
		if (++rxhead == (rxbuf + UART_RXBUFSIZE)) rxhead = rxbuf;
#endif
	} else {
						//reads the buffer to clear the interrupt condition
	}
	if((diff > UART_XON_XOFF_THRESHOLD_1) && (rxon==go)){
		rxon=nogo;
		uart_insertc(XOFF);
	}
	if((diff < UART_XON_XOFF_THRESHOLD_2) && (rxon==nogo)){
		rxon=go;
		uart_insertc(XON);
	}

}

#endif // UART_INTERRUPT


void uart_init() {
	PORTD |= 0x01;				//Pullup an RXD an
	
	UCSRB |= (1<<TXEN);			//UART TX einschalten
#ifdef ATMEGA644
	UCSRA = 0;
	UCSRC = (3<<UCSZ0);		    //Asynchron 8N1
#else	
	UCSRC |= (1<<URSEL)|(3<<UCSZ0);		//Asynchron 8N1
#endif
	UCSRB |= ( 1 << RXEN );			//Uart RX einschalten

	UBRRH=(uint8_t)(UART_BAUD_CALC(UART_BAUD_RATE,F_CPU)>>8);
	UBRRL=(uint8_t)(UART_BAUD_CALC(UART_BAUD_RATE,F_CPU));

#ifdef UART_INTERRUPT
	// init buffers
	rxhead = rxtail = rxbuf;
	txhead = txtail = txbuf;

	// activate rx IRQ
	UCSRB |= _BV(RXCIE);
	#ifdef ATMEGA644
	UCSRB |= _BV(UDRIE0);
	#endif
#endif // UART_INTERRUPT
}

#ifdef UART_INTERRUPT
#ifdef UART_XON_XOFF

void uart_insertc(char c){
	volatile int diff;
	do {
		diff = txhead - txtail;
		if ( diff < 0 ) diff += UART_TXBUFSIZE;
	} while ( diff >= UART_TXBUFSIZE -1 );

	cli();
 	if (--txtail == (txbuf-1)) txtail += UART_TXBUFSIZE;
	*txtail = c;
	
	UCSRB |= (1 << UDRIE);		/* enable data register empty IRQ */
	sei();
}
#endif /* UART_XON_XOFF */
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
	while (!(UCSRA & (1<<UDRE))) /* warten bis Senden moeglich */
		; 
	#ifdef UART_XON_XOFF
	while (txon==nogo) 			/* warte bis XON empfangen */
		;
	#endif
	UDR = c;                  /* schreibt das Zeichen x auf die Schnittstelle */
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

void uart_hexdump(void* buf, int len)
{
	unsigned char table[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	
	while(len--){
		uart_putc(table[((*((char*)buf))>>4)&0xf]);
		uart_putc(table[(*((char*)buf))&0xf]);
		uart_putc(' ');
		buf=(uint8_t*)buf+1;
	}
}


#ifdef UART_INTERRUPT
char uart_getc(void)
{
	char val;

	while(rxhead==rxtail) ;

	val = *rxtail;
 	if (++rxtail == (rxbuf + UART_RXBUFSIZE)) rxtail = rxbuf;

	return val;
}
#else  // WITHOUT INTERRUPT
char uart_getc(void)
{
	char t;
	while (!(UCSRA & (1<<RXC)));	// warten bis Zeichen verfuegbar
	t=UDR;
	#ifdef UART_XON_XOFF
	if (t==XON)  txon=go;
	if (t==XOFF) txon=nogo;
	#endif
	return t;			// Zeichen aus UDR zurueckgeben
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
		#ifdef UART_XON_XOFF
			if (*c==XON)  txon=go;
			if (*c==XOFF) txon=nogo;
		#endif
		return 1;
	}

	return 0;
}
#endif // UART_INTERRUPT
