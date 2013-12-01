#include "../config.h"
#include "../makros.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "borg_hw.h"

/*
 // those macros get defined via menuconfig, now

 // 16 columns total directly controlled, therefore 2 ports
 #define COLPORT1  PORTC
 #define COLDDR1   DDRC

 #define COLPORT2  PORTA
 #define COLDDR2   DDRA

 // the other port controls the shift registers
 #define ROWPORT   PORTD
 #define ROWDDR    DDRD

 // both clock and reset are connected to each shift register
 // reset pin is negated
 #define PIN_MCLR  PD4
 #define PIN_CLK   PD6

 // these are the individual data input pins for the shift registers
 #define PIN_DATA  PD7
*/

#define COLDDR1  DDR(COLPORT1)
#define COLDDR2  DDR(COLPORT2)
#define ROWDDR   DDR(ROWPORT)

#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644__)
/* more ifdef magic :-( */
#define OCR0 OCR0A
#define TIMER0_COMP_vect TIMER0_COMPA_vect
#endif

// buffer which holds the currently shown frame
unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

// switch to next row
static void nextrow(uint8_t row) {
	//reset states of preceding row
	COLPORT1 = 0;
	COLPORT2 = 0;

	// short delay loop, to ensure proper deactivation of the drivers
	unsigned char i;
	for (i = 0; i < 10; i++) {
		asm volatile("nop");
	}

	if (row == 0) {
		// row 0: initialize first shift register
#ifndef INVERT_ROWS
		ROWPORT |= (1 << PIN_DATA);
		ROWPORT |= (1 << PIN_CLK);
		ROWPORT &= ~(1 << PIN_CLK);
		ROWPORT &= ~(1 << PIN_DATA);
#else
		ROWPORT&= ~(1<<PIN_DATA);
		ROWPORT|= (1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_CLK);
		ROWPORT|= (1<<PIN_DATA);

#endif
	} else {
		// remaining rows: just shift forward
		ROWPORT |= (1 << PIN_CLK);
		ROWPORT &= ~(1 << PIN_CLK);
	}

	// another delay loop, to ensure that the drivers are ready
	for (i = 0; i < 20; i++) {
		asm volatile("nop");
	}
}

// show a row
static void rowshow(unsigned char row, unsigned char plane) {
	// depending on the currently drawn plane, display the row for a specific
	// amount of time
	static unsigned char const ocr_table[] = {3, 4, 22};
	OCR0 = ocr_table[plane];

	// output data of the current row to the column drivers
	uint8_t tmp, tmp1;
#ifndef INTERLACED_ROWS
	tmp = pixmap[plane][row][0];
	tmp1 = pixmap[plane][row][1];
#else
	row = (row>>1) + ((row & 0x01)?8:0 );
	tmp = pixmap[plane][row][0];
	tmp1 = pixmap[plane][row][1];
#endif
#ifdef REVERSE_COLS
	tmp = (tmp >> 4) | (tmp << 4);
	tmp = ((tmp & 0xcc) >> 2) | ((tmp & 0x33)<< 2); //0xcc = 11001100, 0x33 = 00110011
	tmp = ((tmp & 0xaa) >> 1) | ((tmp & 0x55)<< 1); //0xaa = 10101010, 0x55 = 1010101
	COLPORT2 = tmp;
	tmp = tmp1;
	tmp = (tmp >> 4) | (tmp << 4);
	tmp = ((tmp & 0xcc) >> 2) | ((tmp & 0x33) << 2); //0xcc = 11001100, 0x33 = 00110011
	tmp = ((tmp & 0xaa) >> 1) | ((tmp & 0x55) << 1); //0xaa = 10101010, 0x55 = 1010101
	COLPORT1 = tmp;
#else
#ifdef INTERLACED_COLS
	static uint8_t interlace_table[16] = {
		0x00, 0x01, 0x04, 0x05, 0x10, 0x11, 0x14, 0x15, 0x40, 0x41, 0x44, 0x45,
		0x50, 0x51, 0x54, 0x55
	};
	COLPORT1 = interlace_table[tmp&0x0f] | (interlace_table[tmp1&0x0f]<<1);
	tmp>>=4; tmp1>>=4;
	COLPORT2 = interlace_table[tmp] | (interlace_table[tmp1]<<1);
#else
	COLPORT1 = tmp;
	COLPORT2 = tmp1;
#endif
#endif

}

// depending on the plane this interrupt triggers at 50 kHz, 31.25 kHz or
// 12.5 kHz
ISR(TIMER0_COMP_vect) {
	static unsigned char plane = 0;
	static unsigned char row = 0;

	// reset watchdog
	wdt_reset();

	// increment both row and plane
	if (++plane == NUMPLANE) {
		plane = 0;
		if (++row == NUM_ROWS) {
			row = 0;
		}
		nextrow(row);
	}

	// output current row according to current plane
	rowshow(row, plane);
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
	 0    0    0       stop
	 0    0    1       clk
	 0    1    0       clk/8
	 0    1    1       clk/64
	 1    0    0       clk/256
	 1    0    1       clk/1024
	 */

#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644__)
	TCCR0A = 0x02; // CTC Mode
	TCCR0B = 0x04; // clk/256
	TCNT0  = 0;    // reset timer
	OCR0   = 20;   // compare with this value
	TIMSK0 = 0x02; // compare match Interrupt on
#else
	TCCR0  = 0x0C; // CTC Mode, clk/256
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
	ROWDDR = (1 << PIN_CLK) | (1 << PIN_DATA);

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
