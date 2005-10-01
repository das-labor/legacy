
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "borg_hw.h"

#define COLPORT1  PORTA
#define COLDDR1   DDRA

#define COLPORT2  PORTC
#define COLDDR2   DDRC

#define ROWPORT PORTD
#define ROWDDR   DDRD
#define PIN_RST  PD4
#define PIN_CLK  PD5
#define PIN_SHFT1 PD6
#define PIN_SHFT2 PD7

unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];


inline void rowshow(unsigned char row, unsigned char plane){
	COLPORT1 = 0;
	COLPORT2 = 0;
	unsigned char i;
	for(i=0;i<20;i++){
		asm volatile("nop");
	}
	if (row == 0){
		ROWPORT&= ~(1<<PIN_RST);
		ROWPORT|= (1<<PIN_RST);
		ROWPORT|= (1<<PIN_SHFT1);
		ROWPORT|= (1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_SHFT1);
		switch (plane){
			case 0:
				OCR0 = 5;
				break;
			case 1:
				OCR0 = 8;
				break;
			case 2:
				OCR0 = 20;
		}
	}else if(row == 8){
		ROWPORT&= ~(1<<PIN_RST);
		ROWPORT|= (1<<PIN_RST);
		ROWPORT|= (1<<PIN_SHFT2);
		ROWPORT|= (1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_SHFT2);
	}else{
		ROWPORT|= (1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_CLK);
	}
	
	for(i=0;i<20;i++){
		asm volatile("nop");
	}
	COLPORT1 = pixmap[plane][row][0];
	COLPORT2 = pixmap[plane][row][1];
}



SIGNAL(SIG_OUTPUT_COMPARE0)
{
	static unsigned char plane = 0;
	static unsigned char row = 0;
	
	wdt_reset();
	rowshow(row, plane);
	
	if(++row == NUM_ROWS){
		row = 0;
		if(++plane==NUMPLANE) plane=0;
	}
}




void timer0_off(){
	cli();

	COLPORT1 = 0;
	COLPORT2 = 0;
	ROWPORT = 0;

	TCCR0 = 0x00;
	sei();
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
	TCCR0 = 0x0B;	// CTC Mode, clk/64
	TCNT0 = 0;	// reset timer
	OCR0  = 0x20;	// Compare with this value
	TIMSK = 0x02;	// Compare match Interrupt on
}

void borg_hw_init(){
	COLDDR1 = 0xFF;
	COLDDR2 = 0xFF;
	ROWDDR = (1<<PIN_RST) | (1<<PIN_CLK) | (1<< PIN_SHFT1) | (1<<PIN_SHFT2);
	COLPORT1 = 0;
	COLPORT2 = 0;
	ROWPORT = 0;
	timer0_on();		

	wdt_reset();
	wdt_enable(0x00);	// 17ms Watchdog
}

