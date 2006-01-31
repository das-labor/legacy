#ifndef _AvrXSerialIo_h_
#define _AvrXSerialIo_h_
#include "AvrXFifo.h"
/*
	AvrSerialIo.h

	Sample code for fully buffered interrupt driven serial I/O for the
	AVR processor.  Uses the AvrXFifo facility.

	Author: Larry Barello (larry@barello.net)

	Revision History:

*/
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

// Buffer size can be any thing from 2 to 250
#define TX0_BUFSZ 32
#define RX0_BUFSZ 32
#define TX1_BUFSZ 32
#define RX1_BUFSZ 32

// Forward declarations
#ifndef _AVRXSERIALIO_C_	// Don't comingle this macro
	AVRX_EXT_FIFO(Rx0Buf);	// with the declaration macro.
	AVRX_EXT_FIFO(Tx0Buf);	// This block only active in
	AVRX_EXT_FIFO(Rx1Buf);	// non-serialio driver code.
	AVRX_EXT_FIFO(Tx1Buf);
#endif

int get_c0(void);		// Non blocking, returns -1 when empty
int get_char0(void);	// Blocking, always returns character
int put_c0(char c);		// Non blocking, returns -1 when full
int put_char0(char c);	// Blocking, always returns 0.

int get_c1(void);		// Non blocking, returns -1 when empty
int get_char1(void);	// Blocking, always returns character
int put_c1(char c);		// Non blocking, returns -1 when full
int put_char1(char c);	// Blocking, always returns 0.

void InitSerial0(uint16_t ubrr);
void InitSerial1(uint16_t ubrr);

// If only one channel defined, then use simpler "get_c()" notation
// This probably doesn't work...

#if ((USART_CHANNELS & CHANNEL_0) == CHANNEL_0)
#	define get_c get_c0
#	define get_char get_char0
#	define put_c put_c0
#	define put_char put_char0
#endif

#if ((USART_CHANNELS & CHANNEL_1) == CHANNEL_1)
#	define get_c get_c1
#	define get_char get_char1
#	define put_c put_c1
#	define put_char put_char1
#endif

#endif //_AvrSerialIo_h_
