#include "../config.h"
#include "../makros.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "borg_hw.h"

/*
 // those macros get defined via menuconfig, now
*/

#define TLC_PIN_SIN		0
#define TLC_PIN_SCLK	1
#define TLC_PIN_SOUT	5
#define TLC_PIN_XLAT	2
#define TLC_PIN_BLANK	3
#define TLC_PIN_VPRG	7
#define TLC_PIN_DCPRG	4
#define TLC_PIN_XERR	6

#define COLDDR  DDR(TLCPORT)
#define ROWDDR  DDR(ROWPORT)

#define SCLK() 	{									\
					TLCPORT |= _BV(TLC_PIN_SCLK);	\
					TLCPORT &= ~_BV(TLC_PIN_SCLK);	\
				}

#define XLAT() 	{									\
					TLCPORT |= _BV(TLC_PIN_XLAT);	\
					TLCPORT &= ~_BV(TLC_PIN_XLAT);	\
				}

/* more ifdef magic :-( */
#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644__)
	#define OCR0 OCR0A
	#define TIMER0_COMP_vect TIMER0_COMPA_vect
#endif

// buffer which holds the currently shown frame
unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

// switch to next row
static void nextrow(uint8_t row) {
	unsigned char i;
	static unsigned char row = 0;

	//increment & wrap row counter
	if (++row == NUM_ROWS) row = 0;

	// short delay loop, to ensure proper deactivation of the drivers
	for (i = 0; i < 10; i++) asm volatile("nop");

	// row 0: initialize first shift register
	if (row == 0)
	{
		#ifndef INVERT_ROWS
			ROWPORT &= ~(1 << PIN_MCLR);
			ROWPORT |= (1 << PIN_MCLR);
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
	}
	// remaining rows: just shift forward
	else
	{
		ROWPORT |= (1 << PIN_CLK);
		ROWPORT &= ~(1 << PIN_CLK);
	}

	// another delay loop, to ensure that the drivers are ready
	for (i = 0; i < 20; i++) asm volatile("nop");
}

// show a row
/*
static void rowshow(unsigned char row, unsigned char plane) {
	// depending on the currently drawn plane, display the row for a specific
	// amount of time
	static unsigned char const ocr_table[] = {3, 4, 22};
	OCR0 = ocr_table[plane];

	// output data of the current row to the column drivers
	uint8_t tmp, tmp1;
	row = (row>>1) + ((row & 0x01)?8:0 );
	tmp = pixmap[plane][row][0];
	tmp1 = pixmap[plane][row][1];

#ifdef REVERSE_COLS
	tmp = (tmp >> 4) | (tmp << 4);
	tmp = ((tmp & 0xcc) >> 2) | ((tmp & 0x33)<< 2); //0xcc = 11001100, 0x33 = 00110011
	tmp = ((tmp & 0xaa) >> 1) | ((tmp & 0x55)<< 1); //0xaa = 10101010, 0x55 = 1010101
	TLCPORT2 = tmp;
	tmp = tmp1;
	tmp = (tmp >> 4) | (tmp << 4);
	tmp = ((tmp & 0xcc) >> 2) | ((tmp & 0x33) << 2); //0xcc = 11001100, 0x33 = 00110011
	tmp = ((tmp & 0xaa) >> 1) | ((tmp & 0x55) << 1); //0xaa = 10101010, 0x55 = 1010101
	TLCPORT1 = tmp;
#else
	TLCPORT1 = tmp;
	TLCPORT2 = tmp1;
#endif
}
*/


//this int runs with multiples of half the frequency of timer 1,
//meaning that new row data gets laoded every 2n pwm periods of the tlc
ISR(TIMER0_COMP_vect ) {
	uint8_t i, x;

	// reset watchdog
	wdt_reset();

	//sync to timer 1
	while(!(TIFR1 & _BV(OCF1A)));

	//blank outputs of low-side (column) driver
	TLCPORT |= _BV(TLC_PIN_BLANK);

	//disable timer 1
	TCCR1B &= ~_BV(CS10);

	// switch rows
	nextrow(row);

	// output current row according to current plane
	//rowshow(row, plane);

	//debug: load 100% duty cycle into column driver
	for(i = 0; i < 16; i++) //16 x 12bit
	{
		//shift in 12 bit words
		for(x = 0; x < 12; x++)
		{
			TLCPORT &= ~_BV(TLC_PIN_SIN);
			TLCPORT |= ((uint8_t)((uint16_t)0xFFF >> x) & 1) << TLC_PIN_SIN;
			SCLK();
		}
	}

	//latch data, disable blanking, enable and reset timers
	XLAT();
	TCNT1    =   0;
	TIFR1    =  _BV(OCF1A)
	TLCPORT &= ~_BV(TLC_PIN_BLANK);
	TCNT0 	 =   0; //be a little faster than timer 1
	TCCR1B  |=  _BV(CS10); // clk/1
}


ISR(TIMER1_COMPA_vect)
{
	TLCPORT |= _BV(TLC_PIN_BLANK);
	TLCPORT &= ~_BV(TLC_PIN_BLANK);
}


void timer0_off() {
	cli();

	TLCPORT = 0;
	ROWPORT = 0;

	#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644__)
		TCCR0A = 0x00;
		TCCR0B = 0x00;
	#else
		TCCR0 = 0x00;
	#endif

	//disable timer 1 also
	TCCR1B &= ~_BV(CS10);

	sei();
}


// initialize timers which trigger the interrupts
static void timer0_on() {
	/* TCCR0: FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00
	 * CS02 CS01 CS00
	 * 0    0    0       stop
	 * 0    0    1       clk
	 * 0    1    0       clk/8
	 * 0    1    1       clk/64
	 * 1    0    0       clk/256
	 * 1    0    1       clk/1024
	 */

	#if defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644__)
		TCCR0A = 0x02; // CTC Mode
		TCCR0B = 0x04; // clk/256
		TCNT0  = 0;    // reset timer
		OCR0   = 0xFF; // compare with this value
		TIMSK0 = 0x02; // compare match Interrupt on
	#else
		TCCR0  = 0x0C; // CTC Mode, clk/256
		TCNT0  = 0;    // reset timer
		OCR0   = 0xFF; // compare with this value
		TIMSK  = 0x02; // compare match Interrupt on
	#endif

	//setup timer1 also (for blanking pulse)
	TCCR1A = 0x00;
	TCCR0B = _BV(WGM12); // CTC Mode
	OCR1A  = 8192; // top value
	TIMSK1 = _BV(OCIE1A); // compare match interrupt on
}


void tlc5940_init()
{
	uint8_t i;

	//init control port pins
	COLDDR = 0x00;
	COLDDR = _BV(TLC_PIN_SCLK) | _BV(TLC_PIN_SIN)
			| _BV(TLC_PIN_XLAT) | _BV(TLC_PIN_BLANK)
			| _BV(TLC_PIN_VPRG) | _BV(TLC_PIN_DCPRG);

	//switch off all outputs and pull-ups
	//xerr is open-drain, enable PUP
	//set blank flag (disable led-outputs)
	//dcprog is high -> use values from dc register
	//vprg is high -> dot correction mode mode
	TLCPORT = _BV(TLC_PIN_XERR) | _BV(TLC_PIN_BLANK) | _BV(TLC_PIN_DCPRG) | _BV(TLC_PIN_VPRG);

	//set max power (6bit * 16 LEDs)
	TLCPORT |= _BV(TLC_PIN_SIN);
	for(i = 0; i < 96; i++) SCLK();
	XLAT();

	//switch to grayscale mode
	TLCPORT &= ~_BV(TLC_PIN_VPRG);

	//flush input shift register (12bit * 16 LEDs)
	TLCPORT &= ~_BV(TLC_PIN_SIN);
	for(i = 0; i < 192; i++) SCLK();
	XLAT();

	//one extra clk pulse is needed after switching from dc mode
	SCLK();
}


void borg_hw_init() {
	//init column driver
	tlc5940_init();

	// switch pins of the row port to output mode
	ROWDDR = (1 << PIN_MCLR) | (1 << PIN_CLK) | (1 << PIN_DATA);

	// reset shift registers for the rows
	ROWPORT = 0;

	//activate the activator
	timer0_on();

	// activate watchdog timer
	wdt_reset();
	wdt_enable(0x00); // 17ms watchdog
}
