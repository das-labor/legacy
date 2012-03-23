#include "../config.h"
#include "../makros.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include "borg_hw.h"

#define ROWPORT  PORTC
#define ROWDDR DDRC

#define COLPORT PORTA
#define COLDDR DDRA

#define MYPORT PORTC
#define MYPORT2 PORTA

#define SHOW 0
#define DATA 1

#define PIN_ENABLE 0
#define PIN_DATA 1

unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

inline void rowshow(unsigned char row, unsigned char plane) {

	unsigned char p, n, m, outb, clkmsk;

	clkmsk = 0x80 >> row;

	// if (row==4)
	{
		MYPORT &= ~(1 << PIN_ENABLE);
		// MYPORT |= (1 << PIN_ENABLE); //hide
		// MYPORT |= (1 << PIN_ENABLE); //hide
		for (n = 7; n < 8; n--) {
			outb = pixmap[plane][row][n];
			p = 0x80;
			for (m = 0; m < 8; m++) {
				if (outb & p)
					// MYPORT |= (1 << PIN_DATA);
					MYPORT &= ~(1 << PIN_DATA);
				else
					// MYPORT &= ~(1 << PIN_DATA); //off
					MYPORT |= (1 << PIN_DATA);

				MYPORT2 &= ~clkmsk;
				MYPORT2 |= clkmsk;

				p >>= 1;

			}
		}
	}
	MYPORT |= (1 << PIN_ENABLE);
	//MYPORT &= ~(1 << PIN_ENABLE) ;
	// for(n=0;n<250;n++) asm ("nop");
}

ISR(TIMER0_COMP_vect) {
	static unsigned char plane = 0;
	unsigned char row = 0;

	switch (plane) {
	case 0:
		OCR0 = 60;
		break;
	case 1:
		OCR0 = 120;
		break;
	case 2:
		OCR0 = 160;
		break;
//	case 3:
//		OCR0 = 24;
//		break;
//	case 4:
//		OCR0 = 48;
//		break;
	}

	for (row = 0; row < NUM_ROWS; row++) {
		rowshow(row, plane);
	}

	if (++plane == NUMPLANE)
		plane = 0;
	wdt_reset();
}

void timer0_on() {
	/* 	TCCR0: FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00
	 CS02 CS01 CS00
	 0    0    0	   stop
	 0    0    1       clk
	 0    1    0       clk/8
	 0    1    1       clk/64
	 1    0    0       clk/256
	 1    0    1       clk/1024
	 */
	TCCR0 = 0x08 | 0x04; // CTC Mode, clk/256 MH: slower 5 instead of 4
	TCNT0 = 0;           // reset timer
	OCR0 = 0x30;         // compare with this value 0x30
	TIMSK = 0x02;        // compare match Interrupt on
}

void timer0_off() {
	cli();

	MYPORT &= ~(1 << PIN_ENABLE);
	// MYPORT |= (1 << PIN_ENABLE); // hide
	// COLPORT |= (1<<PIN_LINE_EN); // blank display
	// PORTD |= 0x01;               // blank display
	TCCR0 = 0x00;
	sei();
}

void watchdog_enable() {
	wdt_reset();
	wdt_enable(0x00); // 17ms
}

void borg_hw_init() {
	//COLPORT |= (1<<PIN_CLK) | (1<<PIN_LINE_EN);
	//COLDDR |= (1<<PIN_CLK) | (1<<PIN_LINE_EN) | (1<<PIN_DATA);
	//ROWDDR |= 0x07;

	DDRA = 0xFF;
	DDRC = 0xFF;

	watchdog_enable();
	timer0_on();
}

