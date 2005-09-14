#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include "borg_hw.h"


unsigned char pixmap[NUM_LEVELS][NUM_PLANES][PLANEBYTES];

inline void rowshow(unsigned char row, unsigned char plane){
	unsigned char i;	
	CTRLPORT |= (1<<PIN_OE_DATA);

	if (row == 0){
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
		CTRLPORT &= ~(1<<PIN_CLEAR_PLANE);
		CTRLPORT |=  (1<<PIN_CLEAR_PLANE);
		CTRLPORT |=  (1<<PIN_SHIFT_PLANE);		
		CTRLPORT |=  (1<<PIN_CLK_PLANE);		
		CTRLPORT &= ~(1<<PIN_CLK_PLANE);
		CTRLPORT &= ~(1<<PIN_SHIFT_PLANE);	
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
		DATAPORT = pixmap[plane][row][i];	
		CTRLPORT |=  (1<<PIN_CLK_DATA);		
		CTRLPORT &= ~(1<<PIN_CLK_DATA);
	}
	CTRLPORT &= ~(1<<PIN_OE_DATA);
}


SIGNAL(SIG_OUTPUT_COMPARE0) {
	static unsigned char plane = 0;
	static unsigned char row = 0;
	
	wdt_reset();

	rowshow(row, plane);
	
	if (++row == NUM_PLANES) {
		row = 0;
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

void borg_hw_init() {
	CTRLPORT = (1<<PIN_OE_DATA);
	DATAPORT = 0x00;
	CTRLDDR = 0xFF;
	DATADDR = 0xFF;
	timer0_on ();

	wdt_reset();
	wdt_enable(0x00);  // 17ms Watchdog
}

