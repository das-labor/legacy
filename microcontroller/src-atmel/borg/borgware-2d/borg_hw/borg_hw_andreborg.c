#include "../config.h"
#include "../makros.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "borg_hw.h"

/*
 // those macros get defined via menuconfig, now

 // 16 columns total directly controlled, therefore 2 ports
 #define COLPORT1  PORTA
 #define COLDDR1   DDRA

 #define COLPORT2  PORTC
 #define COLDDR2   DDRC

 // the other port controls the shift registers
 #define ROWPORT   PORTD
 #define ROWDDR    DDRD

 // both clock and reset are connected to each shift register
 // reset pin is negated
 #define PIN_MCLR  PD4
 #define PIN_CLK   PD5

 // these are the individual data input pins for the shift registers
 #define PIN_DATA1 PD6
 #define PIN_DATA2 PD7
*/

#define COLDDR1  DDR(COLPORT1)
#define COLDDR2  DDR(COLPORT2)
#define ROWDDR   DDR(ROWPORT)

#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644__)
/* more ifdef magic :-( */
#define OCR0 OCR0A
#define SIG_OUTPUT_COMPARE0 SIG_OUTPUT_COMPARE0A
#endif

// buffer which holds the currently shown frame
unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

// display a row
static void rowshow(unsigned char row, unsigned char plane) {
	//reset states of preceding row
	COLPORT1 = 0;
	COLPORT2 = 0;

	// short delay loop, to ensure proper deactivation of the drivers
	unsigned char i;
	for (i = 0; i < 20; i++) {
		asm volatile("nop");
	}

	if (row == 0) {
		// row 0: initialize first shift register
		ROWPORT &= ~(1 << PIN_MCLR);
		ROWPORT |= (1 << PIN_MCLR);
		ROWPORT |= (1 << PIN_DATA1);
		ROWPORT |= (1 << PIN_CLK);
		ROWPORT &= ~(1 << PIN_CLK);
		ROWPORT &= ~(1 << PIN_DATA1);

		// depending on the currently drawn plane, display the row for a
		// specific amount of time
		static unsigned char const ocr0_table[] = {5, 8, 20};
		OCR0 = ocr0_table[plane];
	} else if (row == 8) {
		// row 8: initialize second shift register
		ROWPORT &= ~(1 << PIN_MCLR);
		ROWPORT |= (1 << PIN_MCLR);
		ROWPORT |= (1 << PIN_DATA2);
		ROWPORT |= (1 << PIN_CLK);
		ROWPORT &= ~(1 << PIN_CLK);
		ROWPORT &= ~(1 << PIN_DATA2);
	} else {
		// remaining rows: just shift forward
		ROWPORT |= (1 << PIN_CLK);
		ROWPORT &= ~(1 << PIN_CLK);
	}

	// another delay loop, to ensure that the drivers are ready
	for (i = 0; i < 20; i++) {
		asm volatile("nop");
	}

	// output data of the current row to the column drivers
	COLPORT1 = pixmap[plane][row][0];
	COLPORT2 = pixmap[plane][row][1];
}

// depending on the plane this interrupt gets triggered at 50 kHz, 31.25 kHz or
// 12.5 kHz
SIGNAL(SIG_OUTPUT_COMPARE0) {
	static unsigned char plane = 0;
	static unsigned char row = 0;

	// reset watchdog
	wdt_reset();

	// output current row according to current plane
	rowshow(row, plane);

	// increment both row and plane
	if (++row == NUM_ROWS) {
		row = 0;
		if (++plane == NUMPLANE) {
			plane = 0;
		}
	}
}

void timer0_off() {
	cli();

	COLPORT1 = 0;
	COLPORT2 = 0;
	ROWPORT = 0;

#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644__)
	TCCR0A = 0x00;
	TCCR0B = 0x00;
#else
	TCCR0 = 0x00;
#endif
	sei();
}

// initialize timer which triggers the interrupt
static void timer0_on() {
	/* 	TCCR0: FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00
	 CS02 CS01 CS00
	 0    0    0	   stop
	 0    0    1       clk
	 0    1    0       clk/8
	 0    1    1       clk/64
	 1    0    0       clk/256
	 1    0    1       clk/1024
	 */

#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644__)
	TCCR0A = 0x02; // CTC Mode
	TCCR0B = 0x03; // clk/64
	TCNT0  = 0;    // reset timer
	OCR0   = 20;   // compare with this value
	TIMSK0 = 0x02; // compare match Interrupt on
#else
	TCCR0  = 0x0B; // CTC Mode, clk/64
	TCNT0  = 0;    // reset timer
	OCR0   = 20;   // compare with this value
	TIMSK  = 0x02; // compare match Interrupt on
#endif
}

void borg_hw_init() {
	// switch column ports to output mode
	COLDDR1 = 0xFF;
	COLDDR2 = 0xFF;

	// switch pins of the row port to output mode
	ROWDDR = (1<<PIN_MCLR) | (1<<PIN_CLK) | (1<<PIN_DATA1) | (1<<PIN_DATA2);

	// switch off all columns for now
	COLPORT1 = 0;
	COLPORT2 = 0;

	// reset shift registers for the rows
	ROWPORT = 0;

	timer0_on();

	// activate watchdog timer
	wdt_reset();
	wdt_enable(0x00); // 17ms watchdog
}
