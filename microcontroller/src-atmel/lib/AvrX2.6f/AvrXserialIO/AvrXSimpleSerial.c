/*
	AvrXSimpleSerial.c

	Sample code for simple unbuffered (except for hardware)
	serial I/O for most (all?) Avr single and dual U(S)ART
	capable processors with >= 8k FLASH.

	There is a natural three charactor buffer for Rx data: the hardware is
	double buffered and the input shift register.  The Tx is double
	buffered in hardware.  It is easy to make Rx triple buffered by reading
	in UDR in the interrupt handler and using that stored value instead of UDR
	in the get_c and get_char routines.

	NB: The value in the shift register will be overwritten by additional
	incoming data.

	Author: Larry Barello (larry@barello.net)

	Revision History:
	09-13-2005	- Initial version

*/

//------------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include "avrx.h"
#include "AvrXSerialIo.h"

#if USART_CHANNELS & CHANNEL_0

Mutex Rx0Ready, Tx0Ready;

void InitSerial0(uint16_t ubrr)
{
// Two USARTs, not shared
#if defined(__AVR_ATmega128__) || defined(__AVR_ATmega64__) | defined(__AVR_ATcan128__)
	UBRR0L = ubrr;
	UBRR0H = (uint8_t)(ubrr>>8);
	UCSR0A = (BAUDX == 8)?(1<<U2X):0;
	UCSR0B = ((1<<TXEN) | (1<<RXEN) | (1<<RXCIE) | (1<<UDRIE));
	UCSR0C = ((1<<UCSZ1) | (1<<UCSZ0));

// Dual USART, old shared UCSRC & UBRRH
#elif defined(__AVR_ATmega162__) | defined(__AVR_ATmega161__)
	UBRR0L = ubrr;
	UBRR0H = (uint8_t)(ubrr>>8);
	UCSR0A = (BAUDX == 8)?(1<<U2X):0;
	UCSR0B = ((1<<TXEN) | (1<<RXEN) | (1<<RXCIE) | (1<<UDRIE));
#	ifdef (__AVR_ATmega162__)
	UCSR0C = (1<<URSEL0) | (1<<UCSZ1) | (1<<UCSZ0);
#	endif

// One UART (note the missing C register)
#elif defined(__AVR_ATmega163__)
	UBRR   = ubrr;
	UBRRHI = (uint8_t)(ubrr>>8);
	UCSRA  = (BAUDX == 8)?(1<<U2X):0;
	UCSRB  = ((1<<TXEN) | (1<<RXEN) | (1<<RXCIE) | (1<<UDRIE));

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
	UCSRB = ((1<<TXEN) | (1<<RXEN) | (1<<RXCIE) | (1<<UDRIE));
	UCSRC = ((1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0));

// One USART, (C register not shared)
#elif defined(__AVR_ATmega169__) | defined(__AVR_ATmega165__) | defined(__AVR_IOtn2313__)
	UBRRL = ubrr;
	UBRRH = (uint8_t)(ubrr>>8);
	UCSRA = (BAUDX == 8)?(1<<U2X):0;
	UCSRB = ((1<<TXEN) | (1<<RXEN) | (1<<RXCIE) | (1<<UDRIE));
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
}

int put_c0(char c)	// Non blocking output
{
	if (AvrXTestSemaphore(&Tx0Ready) == SEM_DONE)
	{
		UDR = c;
		UCSRB |= (1<<UDRIE);
		return 0;
	}
	else
		return FIFO_ERR;
}

int put_char0( char c)	// Blocking output
{
	AvrXWaitSemaphore(&Tx0Ready);
	UDR = c;
	UCSRB |= (1<<UDRIE);
	return 0;
}

int get_c0(void)	// Non blocking, return status outside of char range
{
	if (AvrXTestSemaphore(&Rx0Ready) == SEM_DONE)
	{
		uint8_t c = UDR;
		UCSRB |= (1<<RXCIE);	// Enable getting the next charactor
		return c;
	}
	else
		return FIFO_ERR;
}

int get_char0(void)	// Blocks waiting for something
{
	AvrXWaitSemaphore(&Rx0Ready);
	uint8_t c = UDR;
	UCSRB |= (1<<RXCIE);	// Enable getting the next charactor
	return c;
}
/*
This handler will buffer up to three characters in hardware. (double buffer +
input shift register).  The semaphore is used to signal the task that one or more
characters are ready.  The task routines re-enables the interrupt to fetch the next
character.  By using SetSemaphore & TestSemaphore the processing overhead is very
light.  The only time it gets heavy is when the receiving task blocks.
*/

#if defined(SIG_UART_RECV) && !defined(SIG_UART0_RECV)
#  define SIG_UART0_RECV SIG_UART_RECV
#  define SIG_UART0_DATA SIG_UART_DATA
#endif

AVRX_SIGINT(SIG_UART0_RECV)
{
    IntProlog();
    UCSRB &= ~(1<<RXCIE);	// Disable Rx interrupt
    sei();					// Allow other interrupt activity to occur
	AvrXSetSemaphore(&Rx0Ready);
	Epilog();
}

AVRX_SIGINT(SIG_UART0_DATA)
{
    IntProlog();
    UCSRB &= ~(1<<UDRIE);			// Disable UDRE interrupt
    sei();							// Allow other stuff to happen
	AvrXSetSemaphore(&Tx0Ready);
	Epilog();
}
#endif	// USART_CHANNELS & CHANNEL_0
//------------------------------------------------------------------------
#if USART_CHANNELS & CHANNEL_1

Mutex Rx1Ready, Tx1Ready;

void InitSerial1(uint16_t ubrr)
{
// Two USARTs, not shared
#if defined(__AVR_ATmega128__) || defined(__AVR_ATmega64__) | defined(__AVR_ATcan128__)
	UBRR1L = ubrr;
	UBRR1H = (uint8_t)(ubrr>>8);
	UCSR1A = (BAUDX == 8)?(1<<U2X):0;
	UCSR1B = ((1<<TXEN) | (1<<RXEN) | (1<<RXCIE) | (1<<UDRIE));
	UCSR1C = ((1<<UCSZ1) | (1<<UCSZ0));

// Dual USART, old shared UCSRC & UBRRH
#elif defined(__AVR_ATmega162__) | defined(__AVR_ATmega161__)
	UBRR1L = ubrr;
	UBRR1H = (uint8_t)(ubrr>>8);
	UCSR1A = (BAUDX == 8)?(1<<U2X):0;
	UCSR1B = ((1<<TXEN) | (1<<RXEN) | (1<<RXCIE) | (1<<UDRIE));
#	ifdef (__AVR_ATmega162__)	// No C register on the 161
	UCSR1C = (1<<URSEL0) | (1<<UCSZ1) | (1<<UCSZ0);
#	endif
#else
#   ERROR Unknown CPU or CPU does not support two USARTs
#endif
}

int put_c1(char c)	// Non blocking output
{
	if (AvrXTestSemaphore(&Tx1Ready) == SEM_DONE)
	{
		UDR1 = c;
		UCSR1B |= (1<<UDRIE);
		return 0;
	}
	else
		return FIFO_ERR;
}

int put_char1( char c)	// Blocking output
{
	AvrXWaitSemaphore(&Tx1Ready);
	UDR1 = c;
	UCSR1B |= (1<<UDRIE);
	return 0;
}

int get_c1(void)	// Non blocking, return status outside of char range
{
	if (AvrXTestSemaphore(&Rx1Ready) == SEM_DONE)
	{
		uint8_t c = UDR1;
		UCSR1B |= (1<<RXCIE);	// Enable getting the next charactor
		return c;
	}
	else
		return FIFO_ERR;
}

int get_char1(void)	// Blocks waiting for something
{
	AvrXWaitSemaphore(&Rx1Ready);
	uint8_t c = UDR1;
	UCSR1B |= (1<<RXCIE);	// Enable getting the next charactor
	return c;
}
/*
This handler will buffer up to three characters in hardware. (double buffer +
input shift register).  The semaphore is used to signal the task that one or more
characters are ready.  The task routines re-enables the interrupt to fetch the next
character.  By using SetSemaphore & TestSemaphore the processing overhead is very
light.  The only time it gets heavy is when the receiving task blocks.
*/
#if !defined(SIG_UART1_RECV) && defined(SIG_USART1_RECV)
#	define SIG_UART1_RECV SIG_USART1_RECV)
#	define SIG_UART1_DATA SIG_USART1_DATA)
#endif
AVRX_SIGINT(SIG_UART1_RECV)
{
    IntProlog();
    UCSR1B &= ~(1<<RXCIE);	// Disable Rx interrupt
    sei();					// Allow other interrupt activity to occur
	AvrXSetSemaphore(&Rx1Ready);
	Epilog();
}

AVRX_SIGINT(SIG_UART1_DATA)
{
    IntProlog();
    UCSR1B &= ~(1<<UDRIE);			// Disable UDRE interrupt
    sei();							// Allow other stuff to happen
	AvrXSetSemaphore(&Tx1Ready);
	Epilog();
}
#endif	// USART_CHANNELS & CHANNEL_1
