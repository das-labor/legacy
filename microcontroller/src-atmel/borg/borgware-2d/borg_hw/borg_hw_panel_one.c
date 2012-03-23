#include"../autoconf.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "borg_hw.h"


/* pinout of the ribbon cable connected to the panel
 * (the numbering is actually upside down)
 * 
 * 1-3		GND
 * 4		+5V for logic
 * 5-8		+12V
 * 9-10		GND
 * 11		CP3
 * 12		CP2
 * 13		CP1
 * 14		/show
 * 15		CP4
 * 16		/EO3
 * 17-18	GND
 * 19-26	D0-D7
 *
 * and now the right way round:
 * 1 		D7
 * 2 		D6
 * 3 		D5
 * 4 		D4
 * 5 		D3
 * 6 		D2
 * 7 		D1
 * 8 		D0
 * 9 		GND
 * 10		GND
 * 11		/EO3
 * 12		CP4
 * 13		/show
 * 14		CP1
 * 15		CP2
 * 16		CP3
 * 17		GND
 * 18		GND
 * 19		+12V
 * 20		+12V
 * 21		+12V
 * 22		+12V
 * 23		+5V
 * 24		GND
 * 25		GND
 * 26		GND
 *
 * Four 40374 latches are used. No. 1, 2 and 4 drive from the data bus to the
 * panel, no. 3 drives from the button outputs to the data bus. The EOs of
 * 1, 2 and 4 are hardwired to GND.
 *
 * The LEDs are aligned to a 12*16 matrix. The values for the LED columns are
 * passed to the latches via CP1 und CP2 (16 columns total). The index of the
 * row is passed during the deletion of "/show".
 *
 * The buttons are aligned to an 8*8 matrix. The rows get separately set to
 * "high" via latch 4. The columns can then be read via latch 3.
 */


// data port for the panel
#define COLPORT  PORTC
#define COLDDR   DDRC
#define COLPIN	PINC

#define CTRLPORT PORTD
#define CTRLDDR   DDRD

// pins on CTRLPORT
#define PIN_EO3  PD7
#define PIN_CP4  PD2
#define PIN_SHOW PD3
#define PIN_CP1  PD4
#define PIN_CP2  PD5
#define PIN_CP3  PD6


// buffer which holds the currently shown frame
unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

volatile uint8_t keys[8];


inline void busywait() {
	//unsigned char i;
	//for(i=0; i < 20; i++){
	//	asm volatile("nop");
	//}
}


// display a row
inline void rowshow(unsigned char row, unsigned char plane){
	CTRLPORT |= (1 << PIN_SHOW); //blank

	COLPORT = pixmap[plane][row][0];
	busywait();
	CTRLPORT |= (1 << PIN_CP1);
	busywait();
	CTRLPORT &= ~(1 << PIN_CP1);
	busywait();

	COLPORT = pixmap[plane][row][1];
	busywait();
	CTRLPORT |= (1 << PIN_CP2);
	busywait();
	CTRLPORT &= ~(1 << PIN_CP2);
	busywait();

	COLPORT = row;
	busywait();
	CTRLPORT &= ~(1 << PIN_SHOW);
}


inline void checkkeys(uint8_t row){
	static uint8_t mask;
	if (row == 0) {
		mask = 1;
	} else {
		//read keyboard cols into latch
		COLDDR = 0;
		CTRLPORT &= ~(1 << PIN_EO3);
		CTRLPORT |= (1 << PIN_CP3);
		busywait();
		CTRLPORT &= ~(1 << PIN_CP3);
		busywait();
		keys[row - 1] = COLPIN;
		CTRLPORT |= (1 << PIN_EO3);
		busywait();
		COLDDR = 0xFF;
	}

	COLPORT = mask;
	mask <<= 1;
	busywait();
	CTRLPORT |= (1 << PIN_CP4);
	busywait();
	CTRLPORT &= ~(1 << PIN_CP4);
}


// depending on the plane this interrupt gets triggered at 50 kHz, 31.25 kHz or
// 12.5 kHz
ISR(TIMER0_COMP_vect)
{
	static unsigned char plane = 0;
	static unsigned char row = 0;

	// reset watchdog
	wdt_reset();
	
	// output current row according to current plane
	rowshow(row, plane);

	if( (plane == 2) && (row<9) ) checkkeys(row);
	
	// increment both row and plane
	if(++row == NUM_ROWS){
		row = 0;
		if(++plane==NUMPLANE) plane=0;
		switch(plane){
			case 0: 
					OCR0 = 5;
					break;
			case 1:
					OCR0 = 12;
					break;
			case 2:
					OCR0 = 20;
					break;
		}
	}
}


void timer0_off(){
	cli();

	TCCR0 = 0x00;
	sei();
}


// initialize timer which triggers the interrupt
void timer0_on(){
	/* 	TCCR0: FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00
	  CS02 CS01 CS00
	   0    0    0       stop
	   0    0    1       clk
	   0    1    0       clk/8
	   0    1    1       clk/64
	   1    0    0       clk/256
	   1    0    1       clk/1024
	*/
	TCCR0 = 0x0C; // CTC Mode, clk/64
	TCNT0 = 0;    // reset timer
	OCR0  = 20;   // compare with this value
	TIMSK = 0x02; // compare match Interrupt on
}


void borg_hw_init(){
	//Pins am Zeilenport auf Ausgang
	CTRLPORT |= (1 << PIN_EO3) | (1 << PIN_SHOW);
	CTRLDDR |= (1 << PIN_EO3) | (1 << PIN_CP4) | (1 << PIN_SHOW)
			| (1 << PIN_CP1) | (1 << PIN_CP2) | (1 << PIN_CP3);

	// switch off all columns for now
	// switch column ports to output mode
	COLDDR = 0xFF;
	COLPORT = 0x00;

	timer0_on();

	// activate watchdog timer
	wdt_reset();
	wdt_enable(0x00); // 17ms watchdog
}
