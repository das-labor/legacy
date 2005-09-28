
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include "borg_hw.h"

#define ROWPORT  PORTB
#define ROWDDR DDRB

#define COLPORT PORTD
#define COLDDR DDRD

#define PIN_DATA     PC4
#define PIN_CLK      PC6 /*active low*/
#define PIN_LINE_EN  PC5 /*active low*/


unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];


inline void rowshow(unsigned char row, unsigned char plane){
	COLPORT |= (1<<PIN_LINE_EN);//blank display
		
	ROWPORT = (ROWPORT & 0xF8) | row;
	
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
				COLPORT |= (1<<PIN_DATA);
			}else{
				COLPORT &= ~(1<<PIN_DATA);
			}
			d<<=1;
			COLPORT &= ~(1<<PIN_CLK);
			COLPORT |= (1<<PIN_CLK);
		}
	}
	
	COLPORT &= ~(1<<PIN_LINE_EN);//unblank display
	
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
	wdt_reset();
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

void timer0_off(){
	cli();

	COLPORT |= (1<<PIN_LINE_EN);//blank display
	
	TCCR0 = 0x00;
	sei();
}

void watchdog_enable()
{
	wdt_reset();
	wdt_enable(0x00);  // 17ms

}

void borg_hw_init(){
	COLPORT |= (1<<PIN_CLK) | (1<<PIN_LINE_EN);
	COLDDR |= (1<<PIN_CLK) | (1<<PIN_LINE_EN) | (1<<PIN_DATA);
	
	ROWDDR |= 0x07;
	
	watchdog_enable();
	timer0_on();
}

