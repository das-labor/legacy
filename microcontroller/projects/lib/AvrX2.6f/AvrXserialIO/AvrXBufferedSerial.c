/*
	AvrXBufferedSerial.c

	Sample code for fully buffered interrupt driven serial I/O for the
	AVR processor.  Uses the AvrXFifo facility.

	Author: Larry Barello (larry@barello.net)

	Revision History:
	09-13-2005	- Initial version

*/

//------------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#define _AVRXSERIALIO_C_
#include "AvrXSerialIo.h"

#if USART_CHANNELS & CHANNEL_0

AVRX_DECL_FIFO(Rx0Buf, RX0_BUFSZ);
AVRX_DECL_FIFO(Tx0Buf, TX0_BUFSZ);

void InitSerial0(uint16_t ubrr)
{
// Two USARTs, not shared
#if defined(__AVR_ATmega128__) || defined(__AVR_ATmega64__) | defined(__AVR_ATcan128__)
	UBRR0L = ubrr;
	UBRR0H = (uint8_t)(ubrr>>8);
	UCSR0A = (BAUDX == 8)?(1<<U2X):0;
	UCSR0B = ((1<<TXEN) | (1<<RXEN) | (1<<RXCIE));
	UCSR0C = ((1<<UCSZ1) | (1<<UCSZ0));

// Dual USART, old shared UCSRC & UBRRH
#elif defined(__AVR_ATmega162__) | defined(__AVR_ATmega161__)
	UBRR0L = ubrr;
	UBRR0H = (uint8_t)(ubrr>>8);
	UCSR0A = (BAUDX == 8)?(1<<U2X):0;
	UCSR0B = ((1<<TXEN) | (1<<RXEN) | (1<<RXCIE));
#	ifdef (__AVR_ATmega162__)
	UCSR0C = (1<<URSEL0) | (1<<UCSZ1) | (1<<UCSZ0));
#	endif

// One UART (note the missing C register)
#elif defined(__AVR_ATmega163__)
	UBRR   = ubrr;
	UBRRHI = (uint8_t)(ubrr>>8);
	UCSRA  = (BAUDX == 8)?(1<<U2X):0;
	UCSRB  = ((1<<TXEN) | (1<<RXEN) | (1<<RXCIE));

// One UART, (Classic)
#elif defined(__AVR_AT90S4414__) | defined(__AVR_AT90S8515__)| defined(__AVR_AT90S8535__)
	UBRR	= ubrr;
	UCR		= ((1<<TXEN) | (1<<RXEN) | (1<<RXCIE) | (1<<UDRIE));
#	define UCSRB UCR

// One USART, (C register shared)
#elif defined(__AVR_ATmega8__)   | defined(__AVR_ATmega16__)  | defined(__AVR_ATmega32__)  | \
	  defined(__AVR_ATmega323__) | defined(__AVR_ATmega8515__)| defined(__AVR_ATmega8535__)
	UBRRL = ubrr;
	UBRRH = (uint8_t)(ubrr>>8);
	UCSRA = (BAUDX == 8)?(1<<U2X):0;
	UCSRB = ((1<<TXEN) | (1<<RXEN) | (1<<RXCIE));
	UCSRC = ((1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0));

// One USART, (C register not shared)
#elif defined(__AVR_ATmega169__) | defined(__AVR_ATmega165__) | defined(__AVR_IOtn2313__)
	UBRRL = ubrr;
	UBRRH = (uint8_t)(ubrr>>8);
	UCSRA = (BAUDX == 8)?(1<<U2X):0;
	UCSRB = ((1<<TXEN) | (1<<RXEN) | (1<<RXCIE));
	UCSRC = ((1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0));
#else
#   ERROR Dont know about that CPU!
#endif
#ifndef UCSRB
#   define UCSRB UCSR0B
#endif
#ifndef UDR
#	define UDR UDR0
#endif
	AVRX_INIT_FIFO(Rx0Buf);
	AVRX_INIT_FIFO(Tx0Buf);
}

int put_c0(char c)	// Non blocking output
{
	int retc;
	retc = AvrXPutFifo(Tx0Buf, c);
	UCSRB |= (1<<UDRIE);
	return retc;
}

int put_char0( char c)	// Blocking output
{
	AvrXWaitPutFifo(Tx0Buf, c);
	UCSRB |= (1<<UDRIE);
	return 0;
}


int get_c0(void)	// Non blocking, return status outside of char range
{
	int retc = AvrXPullFifo(Rx0Buf);
	return retc;
}

int get_char0(void)	// Blocks waiting for something
{
	return AvrXWaitPullFifo(Rx0Buf);
}
// We don't care if the buffer is full.  Just signal we got one.
// The task may attempt one extra time to get data out of an empyt
// buffer (getc(), but so what.  Eventually it will block waiting
// for another character to be received.

#if defined(SIG_UART_RECV) && !defined(SIG_UART0_RECV)
#  define SIG_UART0_RECV SIG_UART_RECV		// This covers old single UART chips
#  define SIG_UART0_DATA SIG_UART_DATA
#endif

AVRX_SIGINT(SIG_UART0_RECV)
{
    IntProlog();
    UCSRB &= ~(1<<RXCIE);	// Disable Rx interrupt
    sei();					// Allow other interrupt activity to occur
	AvrXPutFifo(Rx0Buf, UDR);// This resets the Rx Interrupt
	UCSRB |= (1<<RXCIE);	// Re-enable.
	Epilog();
}

AVRX_SIGINT(SIG_UART0_DATA)
{
    IntProlog();
    UCSRB &= ~(1<<UDRIE);			// Disable UDRE interrupt
    sei();							// Allow other stuff to happen
	int c = AvrXPullFifo(Tx0Buf);	// Return -1 if empty (enables interrupts)
	if (c >= 0)						// Tricky tight code: only checking sign
	{
		UDR = c;
		UCSRB |= (1<<UDRIE);
	}
	Epilog();
}
#endif	// USART_CHANNELS & CHANNEL_0
//------------------------------------------------------------------------
#if USART_CHANNELS & CHANNEL_1

AVRX_DECL_FIFO(Rx1Buf, RX1_BUFSZ);
AVRX_DECL_FIFO(Tx1Buf, TX1_BUFSZ);

void InitSerial1(uint16_t ubrr)
{
// Two USARTs, not shared
#if defined(__AVR_ATmega128__) || defined(__AVR_ATmega64__) | defined(__AVR_ATcan128__)
	UBRR1L = ubrr;
	UBRR1H = (uint8_t)(ubrr>>8);
	UCSR1A = (BAUDX == 8)?(1<<U2X):0;
	UCSR1B = ((1<<TXEN) | (1<<RXEN) | (1<<RXCIE));
	UCSR1C = ((1<<UCSZ1) | (1<<UCSZ0));

// Dual USART, old shared UCSRC & UBRRH
#elif defined(__AVR_ATmega162__) | defined(__AVR_ATmega161__)
	UBRR1L = ubrr;
	UBRR1H = (uint8_t)(ubrr>>8);
	UCSR1A = (BAUDX == 8)?(1<<U2X):0;
	UCSR1B = ((1<<TXEN) | (1<<RXEN) | (1<<RXCIE));
#	ifdef (__AVR_ATmega162__)	// No C register on the 161
	UCSR1C = (1<<URSEL0) | (1<<UCSZ1) | (1<<UCSZ0);
#	endif
#else
#   ERROR Unknown CPU or CPU does not support two USARTs
#endif
	AVRX_INIT_FIFO(Rx1Buf);
	AVRX_INIT_FIFO(Tx1Buf);
}

int put_c1(char c)	// Non blocking output
{
	int retc;
	retc = AvrXPutFifo(Tx1Buf, c);
	UCSR1B |= (1<<UDRIE);
	return retc;
}

int put_char1( char c)	// Blocking output
{
	AvrXWaitPutFifo(Tx1Buf, c);
	UCSR1B |= (1<<UDRIE);
	return 0;
}

int get_c1(void)	// Non blocking, return status outside of char range
{
	int retc = AvrXPullFifo(Rx1Buf);
	return retc;
}

int get_char1(void)	// Blocks waiting for something
{
	return AvrXWaitPullFifo(Rx1Buf);
}
// We don't care if the buffer is full.  Just signal we got one.
// The task may attempt one extra time to get data out of an empyt
// buffer (getc(), but so what.  Eventually it will block waiting
// for another character to be received.

AVRX_SIGINT(SIG_UART1_RECV)
{
    IntProlog();
    UCSR1B &= ~(1<<RXCIE);	// Disable Rx interrupt
    sei();					// Allow other interrupt activity to occur
	AvrXPutFifo(Rx1Buf, UDR1);// This resets the Rx Interrupt
	UCSR1B |= (1<<RXCIE);	// Re-enable.
	Epilog();
}

AVRX_SIGINT(SIG_UART1_DATA)
{
    IntProlog();
    UCSR1B &= ~(1<<UDRIE);			// Disable UDRE interrupt
    sei();							// Allow other stuff to happen
	int c = AvrXPullFifo(Tx1Buf);	// Return -1 if empty (enables interrupts)
	if (c >= 0)						// Tricky tight code: only checking sign
	{
		UDR1 = c;
		UCSR1B |= (1<<UDRIE);
	}
	Epilog();
}
#endif	// USART_CHANNELS & CHANNEL_1
