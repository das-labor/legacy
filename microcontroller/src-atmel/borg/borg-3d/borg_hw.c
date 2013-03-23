#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include "util.h"

#include "borg_hw.h"


#define CTRLPORT  PORTA
#define CTRLDDR   DDRA

#define DATAPORT  PORTC
#define DATADDR   DDRC
#define DATAPIN   PINC

#define PIN_OE_DATA    	 PA0
#define PIN_SHIFT_DATA   PA1
#define PIN_CLEAR_DATA 	 PA2
#define PIN_CLK_DATA 	 PA3
#define PIN_SHIFT_PLANE  PA4
#define PIN_CLK_PLANE    PA5
#define PIN_CLEAR_PLANE  PA6


//the actual Pixmap
unsigned char pixmap[NUM_LEVELS][NUM_PLANES][PLANEBYTES];

uint8_t PlanesPNP;	//decides if The Transistors for the Planes are turned one
					// by logic high (PlanesPNP=0), or low (PlanesPNP = 1)

inline void rowshow(unsigned char row, unsigned char plane) {
	unsigned char i;	
	CTRLPORT |= (1<<PIN_OE_DATA);

	if (row == 7){
		switch (plane){
			case 0:
				OCR0 = 8;
				break;
			case 1:
				OCR0 = 8;
				break;
			case 2:
				OCR0 = 20;
		}
		if(PlanesPNP){
			CTRLPORT &=  ~(1<<PIN_SHIFT_PLANE);		
			CTRLPORT |=  (1<<PIN_CLK_PLANE);		
			CTRLPORT &= ~(1<<PIN_CLK_PLANE);
			CTRLPORT |=  (1<<PIN_SHIFT_PLANE);
		}else{	
			CTRLPORT &= ~(1<<PIN_CLEAR_PLANE);
			CTRLPORT |=  (1<<PIN_CLEAR_PLANE);
			CTRLPORT |=  (1<<PIN_SHIFT_PLANE);		
			CTRLPORT |=  (1<<PIN_CLK_PLANE);		
			CTRLPORT &= ~(1<<PIN_CLK_PLANE);
			CTRLPORT &= ~(1<<PIN_SHIFT_PLANE);
		}			
	} else {
		CTRLPORT |=  (1<<PIN_CLK_PLANE);		
		CTRLPORT &= ~(1<<PIN_CLK_PLANE);
	}
	// clear
	// clear data[0] shift_data clk !clk !shift_data data[1] clk !clk ...

	CTRLPORT &= ~(1<<PIN_CLEAR_DATA);
	CTRLPORT |=  (1<<PIN_CLEAR_DATA);
	DATAPORT = pixmap[plane][row][0];
	CTRLPORT |=  (1<<PIN_SHIFT_DATA);		
	CTRLPORT |=  (1<<PIN_CLK_DATA);		
	CTRLPORT &= ~(1<<PIN_CLK_DATA);
	CTRLPORT &= ~(1<<PIN_SHIFT_DATA);

	for (i = 1; i < NUM_PLANES; i++) {
		DATAPORT =  pixmap[plane][row][i];
		CTRLPORT |=  (1<<PIN_CLK_DATA);
		CTRLPORT &= ~(1<<PIN_CLK_DATA);
	}
	CTRLPORT &= ~(1<<PIN_OE_DATA);
}


ISR(TIMER0_COMP_vect) {
	static unsigned char plane = 0;
	static unsigned char row = 7;
	
	wdt_reset();

	rowshow(row, plane);
	
	if (row-- == 0) {
		row = 7;
		if (++plane==NUM_LEVELS) 
			plane = 0;
	}
}


void timer0_on() {
/* 	TCCR0: FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00
		CS02 CS01 CS00
		 0    0    0	       stop
		 0    0    1       clk
		 0    1    0       clk/8
		 0    1    1       clk/64
		 1    0    0       clk/256
		 1    0    1       clk/1024
	
*/
	TCCR0 = 0x0B;	// CTC Mode, clk/64
	TCNT0 = 0;	// reset timer
	OCR0  = 0x20;	// Compare with this value
	TIMSK = 0x02;	// Compare match Interrupt on
}

void timer0_off(){
	cli();

	CTRLPORT = 0;
	DATAPORT = 0;

	TCCR0 = 0x00;
	sei();
}

void borg_hw_init() {
	CTRLPORT = (1<<PIN_OE_DATA);
	CTRLDDR = 0xFF;
	
	DATAPORT = 0xff;//Pullups on
	wait(1);
	if (DATAPIN != 0xff){
		PlanesPNP = 1;//Pulldown on dataline means Plane control inverted
	}
	DATADDR = 0xFF;
	
	if(PlanesPNP){
		uint8_t x;
		CTRLPORT |=  (1<<PIN_CLEAR_PLANE);//leave reset
		CTRLPORT |=  (1<<PIN_SHIFT_PLANE);//shift in ones
		//set shiftreg outputs to 1 (So PNPs are off)
		for(x=0;x<8;x++){
			CTRLPORT |=  (1<<PIN_CLK_PLANE);		
			CTRLPORT &= ~(1<<PIN_CLK_PLANE);
		}
	}
	
	timer0_on ();

	wdt_reset();
	wdt_enable(0x00);  // 17ms Watchdog
}
