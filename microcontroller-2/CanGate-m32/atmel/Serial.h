#ifndef _SERIAL_H_
#define _SERIAL_H_

/////////////////////////////////////////////////////////////////////////////
/* Serial.h

	SerialIO for AvrX

	Heavily based on:
  		Sample code for fully buffered interrupt driven serial I/O for the
		AVR processor.  Uses the AvrXFifo facility.

		Author: Larry Barello (larry@barello.net)
*/

#include "AvrXFifo.h"
#include "config.h"
// UTILITY
enum
{
	FALSE,
	TRUE
};

typedef unsigned char BOOL;


// Control & configuration section.  Define USART_CHANNELS with 1, 2 or both
// to enable sections of code.

#define CHANNEL_0 (1<<0)
#define CHANNEL_1 (1<<1)

#if !defined(USART_CHANNELS)
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__) || defined(__AVR_ATcan128__) || \
	defined(__AVR_ATmega161__) || defined(__AVR_ATmega162__)
#	define USART_CHANNELS (CHANNEL_0 | CHANNEL_1)
#else
#	define USART_CHANNELS CHANNEL_0
#endif
#endif // !defined(USART_CHANNELS)

#ifndef U2X
# define U2X U2X0
#endif

// Baud Rate divisor.  8x gives less error tolerance, but works
// well with cheap 12mhz ceramic resonators to give standard baud
// rates.  16x gives more margin but only gives 4800, 9600, 19200
// and 38400 with standard 8 & 16mhz crystals or the 8 mhz internal
// oscillator.

#define BAUDX	8		// Baud rate divisor.

#if defined(__AVR_AT90S8515__) || defined(__AVR_AT90S8535__) || defined(__AVR_AT90S4414__)
#  undef BAUDX
#  define BAUDX 16		// 16x is only divisor on classic chips (at90s8515, etc).
#endif

#define BAUD(A) ((CPUCLK/BAUDX/(A))-1)

// Forward declarations
#ifndef _AVRXSERIALIO_C_	// Don't comingle this macro
	AVRX_EXT_FIFO(Rx0Buf);	// with the declaration macro.
	AVRX_EXT_FIFO(Tx0Buf);	// This block only active in
	AVRX_EXT_FIFO(Rx1Buf);	// non-serialio driver code.
	AVRX_EXT_FIFO(Tx1Buf);
#endif


void SerialInit(uint16_t ubrr);

void SerialGet(char *c);
int  SerialGetNB(char *c);
void SerialPut(char c);
int  SerialPutNB(char c);
void SerialPutStr(char *str);

#endif //_SERIAL_H_
