
#include <avr/io.h>
#include <avr/interrupt.h>

#define UCSRB UCSR1B
#define UCSRC UCSR1C
#define UDR UDR1
#define UBRRH UBRR1H
#define UBRRL UBRR1L
#define TXEN TXEN1
#define RXEN RXEN1
#define UCSRA UCSR1A
#define UDRIE UDRIE1
#define UCSZ0 UCSZ10
#define RXCIE RXCIE1
#define TXCIE TXCIE1
#define RXC RXC1
#define UDRE UDRE1
#define UART_UDRE_VECTOR USART1_UDRE_vect


#define UART1_TXBUFSIZE 256
#define UART1_BAUD_RATE 57600
#define UART_BAUD_CALC(UART_BAUD_RATE,F_OSC) ((F_OSC+(UART_BAUD_RATE)*8L)/((UART_BAUD_RATE)*16L)-1)


volatile static char txbuf[UART1_TXBUFSIZE];
volatile static char *volatile txhead, *volatile txtail;

ISR(UART_UDRE_VECTOR) {
	if ( txhead == txtail ) {
	
		UCSRB &= ~(1 << UDRIE);		/* disable data register empty IRQ */
	} else {
		UDR = *txtail;			/* schreibt das Zeichen x auf die Schnittstelle */
		if (++txtail == (txbuf + UART1_TXBUFSIZE)) txtail = txbuf;
	}
}


void uart1_init() {
	UCSRA = 0;
	UCSRB = (1<<TXEN); //UART TX einschalten
	UCSRC = (3<<UCSZ0);		//Asynchron 8N1


	UBRRH=(uint8_t)(UART_BAUD_CALC(UART1_BAUD_RATE,F_CPU)>>8);
	UBRRL=(uint8_t)(UART_BAUD_CALC(UART1_BAUD_RATE,F_CPU));

	// init buffers
	txhead = txtail = txbuf;
}

void uart1_putc(char c) {
	volatile int diff;

	/* buffer full? */
	do {
		diff = txhead - txtail;
		if ( diff < 0 ) diff += UART1_TXBUFSIZE;
	} while ( diff >= UART1_TXBUFSIZE -1 );

	cli();
	*txhead = c;
 	if (++txhead == (txbuf + UART1_TXBUFSIZE)) txhead = txbuf;

	UCSRB |= (1 << UDRIE);		/* enable data register empty IRQ */
	sei();
}
