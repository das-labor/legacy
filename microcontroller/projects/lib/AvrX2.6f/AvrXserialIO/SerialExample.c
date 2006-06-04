/*
    Serial interface Demo for AvrXFifo's

    Also illustrates the use of Frame based variables
    at the top level tasking (switched from NAKED to
    NORETURN function attribute)

	When linked with simple serialio there is no buffering
	of charactors so only two charactors can be received
	during the delay (Hardware buffering within the USART).

	When linked with the buffered IO up to 31 (or whatever
	the buffer size - 1 is) charactors can be received while
	delaying.
*/
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "avrx.h"

// Uncomment this to override "AvrXSerialIo.h and just use one channel
// #define USART_CHANNELS (1<1)	// 0 - USART0, 1 = USART1

#include "AvrXSerialIo.h"

enum
{
	FALSE,
	TRUE
};

typedef unsigned char BOOL;

// Peripheral initialization

#define TCNT0_INIT (0xFF-CPUCLK/256/TICKRATE)

/*
 Timer 0 Overflow Interrupt Handler

 Prototypical Interrupt handler:
 . Switch to kernel context
 . handle interrupt
 . switch back to interrupted context.
 */
AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Switch to kernel stack/context
    TCNT0 += TCNT0_INIT;		// Add to pre-load to account for any missed clocks
    AvrXTimerHandler();         // Call Time queue manager
    Epilog();                   // Return to tasks
}

// Super simple string printers...

// PutString from RAM
void myputs(int (*putch)(char), const uint8_t * psz)
{
	while (*psz != 0)
		(*putch)(*psz++);
}

// PutString from FLASH
void myputs_P(int (*putch)(char), const uint8_t * psz)
{
	while (__LPM(psz) != 0)
		(*putch)(__LPM(psz++));
}

#if (USART_CHANNELS & CHANNEL_0)

// This task uses GCC Libc stdio facility and needs an additional 60-80 bytes of stack
// for processing the strings.  Longer strings probably need more stack.

AVRX_GCC_TASKDEF(task0, 76, 1)
{
	TimerControlBlock timer;

//	InitSerial0(BAUD(115200));	// Note: this only works with 12mhz or baud rate crystal
	InitSerial0(BAUD(38400));
    fdevopen(put_char0, get_c0, 0);		// Set up standard I/O
	while(1)
	{
		int c;
		BOOL bGotACharacter = FALSE;

		printf_P(PSTR("Type something within 5 seconds\r"));
		AvrXDelay(&timer, 5*1000);
#if 0		// Alternate code to determine when something was typed in.
		if ((c = AvrXStatFifo(Rx0Buf)))
		{
			bGotACharacter = TRUE;
			printf_P(PSTR("You typed in %d char: \""), c);
			while ((c = getchar()) >= 0)
				putchar(c);
		}
#else
		while ((c = getchar()) != EOF)
		{
			if (!bGotACharacter)
			{
				printf_P(PSTR("You typed in \""));
				bGotACharacter = TRUE;
			}
			putchar(c);
		}
#endif
		if (!bGotACharacter)
			printf_P(PSTR("Try again... "));
		else
			putchar('"'),putchar('\r');
	}
}
#endif // USART_CHANNELS & CHANNEL_0

#if USART_CHANNELS & CHANNEL_1

// This task uses simplified string handlers and only needs a little extra
// stack for the timer & normal call-return stuff.  Note: Buffered I/O routines
// need more stack (this is sufficient for buffered, but not stdio!).

AVRX_GCC_TASKDEF(task1, 40, 1)
{
	TimerControlBlock timer;

//	InitSerial1(BAUD(115200));	// Note: this only works with 12mhz or baud rate crystal
	InitSerial1(BAUD(38400));
	while(1)
	{
		int c;
		BOOL bGotACharacter = FALSE;

		myputs_P(put_char1, PSTR("Type something within 5 seconds\r"));
		AvrXDelay(&timer, 4*1000);
		while ((c = get_c1()) != EOF)
		{
			if (!bGotACharacter)
			{
				myputs_P(put_char1, PSTR("You typed in \""));
				bGotACharacter = TRUE;
			}
			put_char1(c);
		}

		if (!bGotACharacter)
			myputs_P(put_char1, PSTR("Try again... "));
		else
			myputs(put_char1, "\"\r");		// Example of non-FLASH string..
	}
}
#endif // USART_CHANNELS & CHANNEL_1

int main(void)
{
    AvrXSetKernelStack(0);

	MCUCR = _BV(SE);
	TCNT0 = TCNT0_INIT;
#if defined (__AVR_ATmega103__) || defined (__ATmega103__)
	TCCR0 =  ((1<<CS02) | (1<<CS01));
#elif defined (__AVR_ATmega128__) || defined (__ATmega128__) || defined (__AVR_ATmega64__) || defined (__ATmega64__)
	TCCR0 =  ((1<<CS2) | (1<<CS1));
#else	// Most other chips...  Note: some are TCCR0 and some are TCCR0B...
	TCCR0 =  (1<<CS02);
#endif
	TIMSK = _BV(TOIE0);

#if USART_CHANNELS & CHANNEL_1
	AvrXRunTask(TCB(task1));
#endif
#if USART_CHANNELS & CHANNEL_0
	AvrXRunTask(TCB(task0));
#endif
	Epilog();
	while(1);
}
