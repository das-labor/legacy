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

//#define COLDDR1  DDR(COLPORT1)
//#define COLDDR2  DDR(COLPORT2)
//#define ROWDDR   DDR(ROWPORT)
//#define DATAPORT PORTC
#define DATADDR  DDR(DATAPORT)

//#define ADDRPORT PORTA
#define ADDRDDR  DDR(ADDRPORT)

//#define CTRLPORT PORTD
#define CTRLDDR  DDR(CTRLPORT)

#define RDIMDDR DDR(RDIMPORT)

//#define BIT_CS0 2
//#define BIT_CS1 3
//#define BIT_CS2 4
//#define BIT_CS3 5

//#define BIT_RW 6

// buffer which holds the currently shown frame
unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];


inline void pd1165_write(uint8_t addr, uint8_t data) {
	ADDRPORT = (ADDRPORT & 0xf0) | addr;

	DATAPORT = data;
	/*
	switch (display) {
	case 0:
		CTRLPORT &= ~((1 << BIT_CS0) | (1 << BIT_RW));
		CTRLPORT |= ((1 << BIT_CS0));
		break;
	case 1:
		CTRLPORT &= ~((1 << BIT_CS1) | (1 << BIT_RW));
		CTRLPORT |= ((1 << BIT_CS1));
		break;
	case 2:
		CTRLPORT &= ~((1 << BIT_CS2) | (1 << BIT_RW));
		CTRLPORT |= ((1 << BIT_CS2));
		break;
	case 3:
		CTRLPORT &= ~((1 << BIT_CS3) | (1 << BIT_RW));
		CTRLPORT |= ((1 << BIT_CS3));
		break;
	}
	*/
}


/*
// display a row
inline void rowshow(unsigned char row, unsigned char plane) {
	int addr = row;
	// depending on the currently drawn plane, display the row for a specific
	// amount of time
	switch (plane) {
	case 0:
		OCR0 = 3;
		break;
	case 1:
		OCR0 = 4;
		break;
	case 2:
		OCR0 = 22;
	}

	uint8_t tmp, tmp1;
	// output data of current row to the column drivers

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
	//COLPORT2 = tmp;
	tmp = tmp1;
	tmp = (tmp >> 4) | (tmp << 4);
	tmp = ((tmp & 0xcc) >> 2) | ((tmp & 0x33) << 2); //0xcc = 11001100, 0x33 = 00110011
	tmp = ((tmp & 0xaa) >> 1) | ((tmp & 0x55) << 1); //0xaa = 10101010, 0x55 = 1010101
	//COLPORT1 = tmp;
#else
#ifdef INTERLACED_COLS
	static uint8_t interlace_table[16] = {
		0x00, 0x01, 0x04, 0x05, 0x10, 0x11, 0x14, 0x15,
		0x40, 0x41, 0x44, 0x45, 0x50, 0x51, 0x54, 0x55
	};
	//COLPORT1 = interlace_table[tmp&0x0f]  | (interlace_table[tmp1&0x0f]<<1);
	tmp>>=4; tmp1>>=4;
	//COLPORT2 = interlace_table[tmp]  | (interlace_table[tmp1]<<1);
#else
	//COLPORT1 = tmp;
	//COLPORT2 = tmp1;

	pd1165_write(row, tmp);

#endif
#endif
}
*/


// depending on the plane this interrupt gets triggered at 50 kHz, 31.25 kHz or
// 12.5 kHz
ISR(TIMER0_COMP_vect) {
	static unsigned char plane = 0;
	unsigned char row = 0;

	// reset watchdog
	wdt_reset();

	// determine button status of the joystick
	readButtons();

	for (row = 0; row < 8; row++) {
		pd1165_write(row, pixmap[plane][row][0]);
		CTRLPORT &= ~((1 << BIT_CS3) | (1 << BIT_RW));
		CTRLPORT |= ((1 << BIT_CS3));

		pd1165_write(row, pixmap[plane][row][1]);
		CTRLPORT &= ~((1 << BIT_CS2) | (1 << BIT_RW));
		CTRLPORT |= ((1 << BIT_CS2));

		//pd1165_write(0, row, pixmap[plane][row][0]);
		//pd1165_write(1, row, pixmap[plane][row][1]);
	}

	for (row = 8; row < NUM_ROWS; row++) {
		pd1165_write(row - 8, pixmap[plane][row][0]);
		CTRLPORT &= ~((1 << BIT_CS0) | (1 << BIT_RW));
		CTRLPORT |= ((1 << BIT_CS0));

		pd1165_write(row - 8, pixmap[plane][row][1]);
		CTRLPORT &= ~((1 << BIT_CS1) | (1 << BIT_RW));
		CTRLPORT |= ((1 << BIT_CS1));
	}

	// depending on the currently drawn plane, display the row for a specific
	// amount of time
	switch (plane) {
	case 0:
		OCR0 = 3;
		break;
	case 1:
		OCR0 = 4;
		break;
	case 2:
		OCR0 = 22;
		break;
	}

	//increment both row and plane
	if (++plane == NUMPLANE) {
		plane = 0;
	}
}


void timer0_off() {
	cli();

	TCCR0 = 0x00;
	sei();
}


// initialize timer which triggers the interrupt
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
	TCCR0 = 0x0D; // CTC Mode, clk/64
	TCNT0 = 0;    // reset timer
	OCR0  = 20;   // compare with this value
	TIMSK = 0x02; // compare match Interrupt on
}


void timer2_on() {
	/* 	TCCR2: FOC2 WGM20 COM21 COM20 WGM21 CS22 CS21 CS20
	 CS02 CS01 CS00
	 0    0    0	   stop
	 0    0    1       clk
	 0    1    0       clk/8
	 0    1    1       clk/32
	 1    0    0       clk/64
	 1    0    1       clk/128
	 1    1    0       clk/256
	 1    1    1       clk/1024

	 Table 51. Compare Output Mode, non-PWM Mode
	 COM21  COM20  Description
	 0      0      normal port operation, OC2 disconnected.
	 0      1      toggle OC2 on compare match
	 1      0      clear OC2 on compare match
	 1      1      set OC2 on compare match
	 */

	TCCR2 = (1 << WGM21) | (1 << COM20) | 1; //CTC, OC2 toggle, clk/1
	OCR2 = 92; // 80kHz clock on OC2
}


void borg_hw_init() {
	CTRLDDR = (1<<BIT_CS0)|(1<<BIT_CS1)|(1<<BIT_CS2)|(1<<BIT_CS3)|(1<<BIT_RW);
	CTRLPORT = (1<<BIT_CS0)|(1<<BIT_CS1)|(1<<BIT_CS2)|(1<<BIT_CS3)|(1<<BIT_RW);
	DATADDR = 0xff;
	ADDRDDR |= 0x0f;
	CTRLPORT = (1<<BIT_CS0)|(1<<BIT_CS1)|(1<<BIT_CS2)|(1<<BIT_CS3)|(1<<BIT_RW);

	pd1165_write(8, 0x10|7);

	CTRLPORT &= ~((1<<BIT_CS0)|(1<<BIT_CS1)|(1<<BIT_CS2)|(1<<BIT_CS3)|(1<<BIT_RW));
	CTRLPORT |= ((1<<BIT_CS0)|(1<<BIT_CS1)|(1<<BIT_CS2)|(1<<BIT_CS3));

	timer0_on();
	timer2_on();

	DDRD |= 1 << PD7; // OC2 pin to output

	RDIMPORT |= (1 << BIT_RDIM);
	RDIMDDR |= (1 << BIT_RDIM);

	// activate watchdog timer
	wdt_reset();
	wdt_enable(0x00); // 17ms watchdog
}
