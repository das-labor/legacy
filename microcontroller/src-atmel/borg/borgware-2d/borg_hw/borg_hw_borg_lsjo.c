#include "../config.h"
#include "../makros.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include "borg_hw.h"

/*
 // those macros get defined via menuconfig, now

 #define ROWPORT PORTB
 #define ROWDDR DDRB

 #define COLPORT PORTD
 #define COLDDR DDRD

 #define PIN_DATA     PC4
 #define PIN_CLK      PC6 //active low
 #define PIN_LINE_EN  PC5 //active low

 EN = 3        PD3
 RT 4          PD4
 GN 5          PD5
 Latch 6       PD6
 Clk 7         PD7
 Row a-d 10-13 PB2-5

 Clk 13        PB5
 GN  11        PB3


 Row d-a A3-A0 PC3-0

 */

#define D_EN_H()     PORTD |= (1<<PD3)
#define D_EN_L()     PORTD &= ~(1<<PD3)

#define D_RT_H()     PORTD |=  (1<<PD4)
#define D_RT_L()     PORTD &= ~(1<<PD4)

#define D_GN_H()     PORTB |=  (1<<PB3)
#define D_GN_L()     PORTB &= ~(1<<PB3)

#define D_LATCH_H()  PORTD |=  (1<<PD6)
#define D_LATCH_L()  PORTD &= ~(1<<PD6)

#define D_CLK_H()    PORTB |=  (1<<PB5)
#define D_CLK_L()    PORTB &= ~(1<<PB5)

#define ROW_PORT     PORTC

#define COLDDR DDR(COLPORT)
#define ROWDDR DDR(ROWPORT)

unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

inline void rowshow(unsigned char row, unsigned char plane) {
	D_EN_L(); //blank display

//	ROWPORT = (ROWPORT & 0xF8) | row;
	ROW_PORT = (ROW_PORT & 0xf0) | row;

	unsigned char d, x;
	signed char b;
	for (b = LINEBYTES - 1; b >= 0; b--) {
		d = pixmap[plane][row][b];
		while (!(SPSR & (1 << SPIF)))
			;
		SPDR = ~d;
		/*

		for (x = 0; x < 8; x++) {
			if (d & 0x80) {
				D_GN_L();
			} else {
				D_GN_H();
			}
			d <<= 1;
			D_CLK_L();
			D_CLK_H();
		}
		*/
	}
	while (!(SPSR & (1 << SPIF)));

	D_LATCH_H();
	asm volatile ("nop");
	D_LATCH_L();

	D_EN_H(); // unblank display
}

ISR( TIMER0_COMPA_vect) {
	static unsigned char plane = 0;
	static unsigned char row = 0;

	if (row == 0) {
		switch (plane) {
		case 0:
			OCR0A = 7;
			break;
		case 1:
			OCR0A = 20;
			break;
		case 2:
			OCR0A = 40;
		}
	}

	rowshow(row, plane);

	if (++row == NUM_ROWS) {
		row = 0;
		if (++plane == NUMPLANE)
			plane = 0;
	}
	wdt_reset();
}

void timer0_on() {
	/* 	TCCR0: FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00
	 CS02 CS01 CS00
	 0    0    0       stop
	 0    0    1       clk
	 0    1    0       clk/8
	 0    1    1       clk/64
	 1    0    0       clk/256
	 1    0    1       clk/1024

	 */
	// CTC Mode, clk/256
	TCCR0A = 0x02;
	TCCR0B = 0x04;
	TCNT0  = 0;    // reset timer
	OCR0A  = 0x30; // compare with this value
	TIMSK0 = 0x02; // compare match Interrupt on
}

void timer0_off() {
	cli();

	D_EN_L(); //blank display

	TCCR0B = 0x00;
	sei();
}

void watchdog_enable() {
	wdt_reset();
	wdt_enable(0x00); // 17ms

}

void borg_hw_init() {
//	COLPORT |= (1 << PIN_CLK) | (1 << PIN_LINE_EN);
//	COLDDR |= (1 << PIN_CLK) | (1 << PIN_LINE_EN) | (1 << PIN_DATA);

//	ROWDDR |= 0x07;

	DDRD |= 0xf8;
	DDRB = 0x2f;
	DDRC |= 0x0f;

	D_RT_H();
	D_GN_H();

	SPCR = 0x50;
	SPSR |= (1 << SPI2X);
	SPDR = 0;

	watchdog_enable();
	timer0_on();
}

