#include <avr/interrupt.h>
#include <avr/io.h>

#undef WATCHDOG_ENABLED
#ifdef WATCHDOG_ENABLED
#  include <avr/wdt.h>
#endif

#include "../config.h"
#include "../makros.h"
#include "borg_hw.h"

// define row port
#ifndef ROWPORT
#	define ROWPORT PORTA
#endif
#define ROWDDR DDR(ROWPORT)

// define column port
#ifndef COLPORT
#	define COLPORT PORTC
#endif
#define COLDDR DDR(COLPORT)


// buffer which holds the currently shown frame
unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];


SIGNAL(SIG_OUTPUT_COMPARE0)
{
	static unsigned char plane = 0;
	static unsigned char row = 0;

#ifdef WATCHDOG_ENABLED
	// reset watchdog
	wdt_reset();
#endif

	ROWPORT = 0;
	row++;
	if ((COLPORT <<= 1) == 0) {
		row = 0;
		COLPORT = 1;

		if (++plane == NUMPLANE)
		{
			plane = 0;
		}
	}

	for (unsigned char x = 0; x < 10; x++) {
		asm volatile ("nop");
	}

	ROWPORT = pixmap[plane][row][0];
}

void timer0_off(){
	cli();

	COLPORT = 0;
	ROWPORT = 0;

#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644__)
	TCCR0A = 0x00;
	TCCR0B = 0x00;
	TIMSK0 = 0;
#else
	TCCR0 = 0x00;
	TIMSK = 0;
#endif

	sei();
}

static void timer0_on(){
/* 	TCCR0: FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00
		CS02 CS01 CS00
		 0    0    0       stop
		 0    0    1       clk
		 0    1    0       clk/8
		 0    1    1       clk/64
		 1    0    0       clk/256
		 1    0    1       clk/1024

*/

#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644__)
	TCCR0A = 0x02; // CTC Mode
	TCCR0B = 0x03; // clk/64
	TCNT0  = 0x00; // reset timer
	OCR0   = 0x0A; // compare with this value
	TIMSK0 = 0x02; // compare match Interrupt on
#else
	TCCR0  = 0x0B; // CTC Mode, clk/64
	TCNT0  = 0x00; // reset timer
	OCR0   = 0x0A; // compare with this value
	TIMSK  = 0x02; // compare match Interrupt on
#endif
}

void borg_hw_init(){
	// switch all pins of both the row and the column port to output mode
	ROWDDR = 0xFF;
	COLDDR = 0xFF;

	// switch off all rows and columns for now
	COLPORT = 0;
	ROWPORT = 0;

	timer0_on();

#ifdef WATCHDOG_ENABLED
	// activate watchdog timer
	wdt_reset();
	wdt_enable(0x00); // 17ms watchdog
#endif
}
