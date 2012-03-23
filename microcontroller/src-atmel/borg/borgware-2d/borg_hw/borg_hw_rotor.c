#include "../config.h"
#include "../makros.h"
#include "../ioport.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "borg_hw.h"


// buffer which holds the currently shown frame
unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];


// display a row
inline void rowshow(unsigned char row) {
	// output data of the current row to the column drivers
	COLPORT1 = pixmap[0][row][0];
	COLPORT2 = pixmap[0][row][1];

	OUTPUT_ON(LATCH_R);
	OUTPUT_OFF(LATCH_R);

	COLPORT1 = pixmap[1][row][0];
	COLPORT2 = pixmap[1][row][1];

	OUTPUT_ON(LATCH_G);
	OUTPUT_OFF(LATCH_G);

	COLPORT1 = pixmap[2][row][0];
	COLPORT2 = pixmap[2][row][1];

	OUTPUT_ON(LATCH_B);
	OUTPUT_OFF(LATCH_B);
}


uint32_t mod = 1280000ul;
uint32_t akku;

unsigned char row = 0;


ISR(TIMER0_COMP_vect)
{
	// reset watchdog
	wdt_reset();

	akku += mod;

	OCR1A = akku / 256;

	rowshow(row);

	if (++row == NUM_ROWS) {
		row = NUM_ROWS - 1;
	}
}


ISR(INT0_vect) {
	if (akku > (64ul * 256ul * 64ul)) {
		akku -= OCR1A - TCNT1;

		mod = akku / 64;
		akku = 0;
		row = 0;
		OCR1A = 0;
		TCNT1 = 0xffff;
	}
}


void timer0_off() {
	cli();
	TCCR1B = 0x00;
	sei();
}


// initialize timer which triggers the interrupt
void timer1_on() {
	TCCR1B = 1; // clk/1
	TIMSK |= (1 << OCIE1A);
}


void borg_hw_init() {
	DDR(COLPORT1) = 0xff;
	DDR(COLPORT2) = 0xff;

	SET_DDR(LATCH_R);
	SET_DDR(LATCH_G);
	SET_DDR(LATCH_B);

	timer1_on();

	// activate watchdog timer
	wdt_reset();
	wdt_enable(0x00); // 17ms watchdog
}
