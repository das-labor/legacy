
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "borg_hw.h"

#define COLPORT1  PORTA
#define COLDDR1   DDRA

#define COLPORT2  PORTC
#define COLDDR2   DDRC

#define ROWPORT PORTD
#define ROWDDR   DDRD
#define PIN_RST  PD0
#define PIN_CLK  PD1
#define PIN_SHFT1 PD2
#define PIN_SHFT2 PD3

unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];


#define BORG_ANDRE

#ifdef BORG_ANDRE
inline void rowshow(unsigned char row, unsigned char plane){
	COLPORT1 = 0;
	COLPORT2 = 0;
	
	if (row == 0){
		ROWPORT&= ~(1<<PIN_RST);
		ROWPORT|= (1<<PIN_RST);
		ROWPORT|= (1<<PIN_SHFT1);
		ROWPORT|= (1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_SHFT1);
		switch (plane){
			case 0:
				OCR0 = 2;
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
	
	COLPORT1 = pixmap[plane][row][0];
	COLPORT2 = pixmap[plane][row][1];
}
#elif BORG_LS

#error jeah

#endif




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
	TCCR0 = 0x0B;	// CTC Mode, clk/8
	TCNT0 = 0;	// reset timer
	OCR0  = 0x20;	// Compare with this value
	TIMSK = 0x02;	// Compare match Interrupt on
}

void borg_hw_init(){
	COLDDR1 = 0xFF;
	COLDDR2 = 0xFF;
	ROWDDR = 0xFF;
	COLPORT1 = 0;
	COLPORT2 = 0;
	ROWPORT = 0;
	timer0_on();
}

