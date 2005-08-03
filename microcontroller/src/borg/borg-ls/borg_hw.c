
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "borg_hw.h"

#define LSPORT  PORTC
#define LSDDR   DDRC


#define PIN_DATA     PC3
#define PIN_CLK      PC4 /*active low*/
#define PIN_LINE_EN  PC5 /*active low*/


unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];


inline void rowshow(unsigned char row, unsigned char plane){
	LSPORT |= (1<<PIN_LINE_EN);//blank display
		
	LSPORT = (LSPORT & 0xF8) | row;
	
	if (row == 0){
		switch (plane){
			case 0:
				OCR0 = 8;
				break;
			case 1:
				OCR0 = 13;
				break;
			case 2:
				OCR0 = 20;
		}
	}
	
	unsigned char b, d, x;
	for(b=LINEBYTES-1;b!=0xFF;b--){
		d = pixmap[plane][row][b];
		for(x=0;x<8;x++){
			if(d & 0x80){
				LSPORT |= (1<<PIN_DATA);
			}else{
				LSPORT &= ~(1<<PIN_DATA);
			}
			d<<=1;
			LSPORT &= ~(1<<PIN_CLK);
			LSPORT |= (1<<PIN_CLK);
		}
	}
	
	LSPORT &= ~(1<<PIN_LINE_EN);//unblank display
	
}

SIGNAL(SIG_OUTPUT_COMPARE0)
{
	static unsigned char plane = 0;
	static unsigned char row = 0;
	
	rowshow(row, plane);
	
	if(++row == NUM_ROWS){
		row = 0;
		if(++plane==NUMPLANE) plane=0;
	}
}



void timer0_on(){
/* 	TCCR0: FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00
		CS02 CS01 CS00
		 0    0    0	       stop
		 0    0    1       clk
		 0    1    0       clk/8
		 0    1    1       clk/64
		 1    0    0       clk/256
		 1    0    1       clk/1024
	
*/
	TCCR0 = 0x08 |0x04;	// CTC Mode, clk/256
	TCNT0 = 0;	// reset timer
	OCR0  = 0x20;	// Compare with this value
	TIMSK = 0x02;	// Compare match Interrupt on
}

void borg_hw_init(){
	LSPORT |= (1<<PIN_CLK) | (1<<PIN_LINE_EN);
	LSDDR |= 0x3F;
	timer0_on();
}

