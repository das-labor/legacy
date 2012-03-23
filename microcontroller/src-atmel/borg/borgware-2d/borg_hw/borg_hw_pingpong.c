#include "../config.h"
#include "../makros.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "borg_hw.h"

#define PIN_DATA PB4
#define PIN_CLK	 PB3
#define PIN_STR  PB2


// buffer which holds the currently shown frame
unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];


// switch to next row
static void nextrow(uint8_t row) {
	// clear states of the preceding row
	PORTC &= 0xF0;
	PORTD &= 0x0F;
	PORTB &= 0xFC;

	// short delay loop, to ensure proper deactivation of the drivers
	unsigned char i;
	for (i = 0; i < 10; i++) {
		asm volatile("nop");
	}

	if (row == 0) {
		// row 0: initialize first shift register
		PORTB &= ~(1 << PIN_DATA); // row is active low
		PORTB |= (1 << PIN_CLK);
		PORTB &= ~(1 << PIN_CLK);
		PORTB |= (1 << PIN_DATA);
	} else {
		// remaining rows: just shift forward
		PORTB |= (1 << PIN_CLK);
		PORTB &= ~(1 << PIN_CLK);
	}

	// another delay loop, to ensure that the drivers are ready
	for (i = 0; i < 20; i++) {
		asm volatile("nop");
	}
}


// display a row
static void rowshow(unsigned char row, unsigned char plane) {
	// depending on the currently drawn plane, display the row for a specific
	// amount of time
	static unsigned char const tcnt0_table[] = {244, 236, 206};
	TCNT0 = tcnt0_table[plane];

	// output data of the current row to the column drivers
	uint8_t tmp, tmp1;
	tmp = pixmap[plane][row][0];
	tmp1 = pixmap[plane][row][1];

	PORTC = (PORTC & 0xF0) | (tmp & 0x0F);
	PORTD = (PORTD & 0x0F) | (tmp & 0xF0);
	PORTB = (PORTB & 0xFC) | (tmp1 & 0x03);
}


// depending on the plane this interrupt gets triggered at 50 kHz, 31.25 kHz or
// 12.5 kHz
ISR(TIMER0_OVF0_vect) {
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
	/*
	 COLPORT1 = 0;
	 COLPORT2 = 0;
	 ROWPORT = 0;
	 */
	TCCR0 = 0x00;
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
	TCCR0 = 0x03;          // clk/64
	TCNT0 = 0xFF - 20;     // reset timer
	TIMSK |= (1 << TOIE0); // compare match Interrupt on
}


void borg_hw_init() {
	// set required pins to output mode
	DDRC = 0x0F; // PC0-3 - Row 0-3
	DDRD = 0xF4; // PD4-7 - Row 4-7  PD2 - ground for joy
	DDRB = 0x1F; // PB0-1 - Row 8-9  PB2-3 - STR, CLK, d

	// turn off all columns for now, clk off, d and str on
	// PC4-5, PD013 pullup on
	PORTC = 0x30;
	PORTD = 0x0B;
	PORTB = 0x14;

	timer0_on();

	// activate watchdog timer
	wdt_reset();
	wdt_enable(0x00); // 17ms watchdog
}
