#include "../config.h"
#include "../makros.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
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

//lookup table
uint16_t colormap[256] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,29,31,34,36,39,42,45,48,51,55,58,62,65,69,73,77,81,85,90,94,99,103,108,113,118,123,129,134,140,145,151,157,163,169,176,182,188,195,202,209,216,223,230,238,245,253,261,269,277,285,293,302,310,319,328,337,346,355,365,374,384,394,404,414,424,434,445,455,466,477,488,499,510,522,533,545,557,569,581,593,606,618,631,644,657,670,683,696,710,723,737,751,765,779,794,808,823,838,853,868,883,898,914,930,945,961,978,994,1010,1027,1043,1060,1077,1094,1112,1129,1147,1164,1182,1200,1218,1237,1255,1274,1293,1311,1331,1350,1369,1389,1408,1428,1448,1468,1488,1509,1529,1550,1571,1592,1613,1635,1656,1678,1700,1721,1744,1766,1788,1811,1834,1856,1879,1903,1926,1949,1973,1997,2021,2045,2069,2094,2118,2143,2168,2193,2218,2244,2269,2295,2321,2347,2373,2399,2426,2452,2479,2506,2533,2560,2588,2615,2643,2671,2699,2727,2756,2784,2813,2842,2871,2900,2929,2959,2989,3018,3048,3079,3109,3139,3170,3201,3232,3263,3294,3326,3357,3389,3421,3453,3485,3518,3550,3583,3616,3649,3682,3716,3749,3783,3817,3851,3886,3920,3955,3989,4024,4059,4094};

// buffer which holds the currently shown frame
unsigned char framebuf[NUM_ROWS][NUM_COLS];

//row counter
static unsigned char row = 0;

//switch to next row
static void nextrow() {
	unsigned char i;

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
}


//this int runs with multiples of half the frequency of timer 1,
//meaning that new row data gets laoded every 2n pwm periods of the tlc
ISR(TIMER0_COMP_vect ) {
	uint8_t y, x, v;

	// reset watchdog
	wdt_reset();
	
	//increment & wrap row counter
	if (++row == NUM_ROWS) row = 0;

	//shift pixmap as 12 bit words into device
	for(y = 0; y < 16; y++) //16 x 12bit
	{	
		//fetch msb
		v = (colormap[framebuf[row][y]] >> 4) & 0xF0;
		
		//12
		TLCPORT &= ~_BV(TLC_PIN_SIN);
		TLCPORT |= (v & 0x80) >> (7-TLC_PIN_SIN);
		SCLK();
		v <<= 1;
		
		//11
		TLCPORT &= ~_BV(TLC_PIN_SIN);
		TLCPORT |= (v & 0x80) >> (7-TLC_PIN_SIN);
		SCLK();
		v <<= 1;
		
		//10
		TLCPORT &= ~_BV(TLC_PIN_SIN);
		TLCPORT |= (v & 0x80) >> (7-TLC_PIN_SIN);
		SCLK();
		v <<= 1;
		
		//9
		TLCPORT &= ~_BV(TLC_PIN_SIN);
		TLCPORT |= (v & 0x80) >> (7-TLC_PIN_SIN);
		SCLK();
		v <<= 1;
		
		//fetch lsb
		v = (colormap[framebuf[row][y]]) & 0xFF;
		
		//8
		TLCPORT &= ~_BV(TLC_PIN_SIN);
		TLCPORT |= (v & 0x80) >> (7-TLC_PIN_SIN);
		SCLK();
		v <<= 1;
		
		//7
		TLCPORT &= ~_BV(TLC_PIN_SIN);
		TLCPORT |= (v & 0x80) >> (7-TLC_PIN_SIN);
		SCLK();
		v <<= 1;
		
		//6
		TLCPORT &= ~_BV(TLC_PIN_SIN);
		TLCPORT |= (v & 0x80) >> (7-TLC_PIN_SIN);
		SCLK();
		v <<= 1;
		
		//5
		TLCPORT &= ~_BV(TLC_PIN_SIN);
		TLCPORT |= (v & 0x80) >> (7-TLC_PIN_SIN);
		SCLK();
		v <<= 1;
		
		//4
		TLCPORT &= ~_BV(TLC_PIN_SIN);
		TLCPORT |= (v & 0x80) >> (7-TLC_PIN_SIN);
		SCLK();
		v <<= 1;
		
		//3
		TLCPORT &= ~_BV(TLC_PIN_SIN);
		TLCPORT |= (v & 0x80) >> (7-TLC_PIN_SIN);
		SCLK();
		v <<= 1;
		
		//2
		TLCPORT &= ~_BV(TLC_PIN_SIN);
		TLCPORT |= (v & 0x80) >> (7-TLC_PIN_SIN);
		SCLK();
		v <<= 1;
		
		//1
		TLCPORT &= ~_BV(TLC_PIN_SIN);
		TLCPORT |= (v & 0x80) >> (7-TLC_PIN_SIN);
		SCLK();
		v <<= 1;
	}
	
	//blank outputs of low-side (column) driver
	TLCPORT |= _BV(TLC_PIN_BLANK);

	//switch rows
	nextrow();

	//latch data, disable blanking, enable and reset timers
	XLAT();
	TLCPORT &= ~_BV(TLC_PIN_BLANK);
	
	//reset timer
	TCNT0 = 0;
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
		OCR0   = 16;   // compare with this value
		TIMSK0 = 0x02; // compare match Interrupt on
	#else
		TCCR0  = 0x0C; // CTC Mode, clk/256
		TCNT0  = 0;    // reset timer
		OCR0   = 16;   // compare with this value
		TIMSK  = 0x02; // compare match Interrupt on
	#endif
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

void test_tlc()
{
	uint8_t i, x;

	while(1)
	{	
		//debug: load 100% duty cycle into column driver
		for(i = 0; i < 16; i++) //16 x 12bit
		{
			//shift in 12 bit words
			for(x = 0; x < 12; x++)
			{
				TLCPORT &= ~_BV(TLC_PIN_SIN);
				TLCPORT |= 1<< TLC_PIN_SIN;
				SCLK();
			}
		}

		//blank outputs of low-side (column) driver
		TLCPORT |= _BV(TLC_PIN_BLANK);
		
		//rowswitch
		nextrow();
		
		//latch data, disable blanking
		XLAT();
		TLCPORT &= ~_BV(TLC_PIN_BLANK);
		
		//delay 4096 gsclocks (256)
		_delay_us(254);
	}
}


void borg_hw_init() {
	//init column driver
	tlc5940_init();

	// switch pins of the row port to output mode
	ROWDDR = (1 << PIN_MCLR) | (1 << PIN_CLK) | (1 << PIN_DATA);

	// reset shift registers for the rows
	ROWPORT = 0;
	
	//test tlc chip
	//test_tlc();

	//activate the activator
	timer0_on();

	// activate watchdog timer
	wdt_reset();
	wdt_enable(0x00); // 17ms watchdog
}
