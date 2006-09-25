
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "borg_hw.h"

#define COLPORT  PORTB
#define COLDDR   DDRB

#define ROWPORT PORTC
#define ROWDDR   DDRC

unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

inline void rowshow(unsigned char row, unsigned char plane){
	static uint8_t rowmask = 0xFF;
	
	//alle Zeilen aus
	ROWPORT |= 0x1F;
	//alle Spalten aus
	COLPORT = 0;
	
	if (row == 0){
		rowmask = 0xFE;
	}
	
	switch (plane){
		case 0:
			TCNT0 = 0x100-2;
			break;
		case 1:
			TCNT0 = 0x100-8;
			break;
		case 2:
			TCNT0 = 0x100-20;
	}
	
	ROWPORT &= rowmask;
	
	rowmask <<= 1;
	rowmask |= 0x01;
	
	COLPORT = pixmap[plane][row][0];
}


SIGNAL(SIG_OVERFLOW0)
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
	TCCR0 = 0x03;	// clk/64
	TCNT0 = 0xFF-20;// reset timer
	TIMSK |= (1<<TOIE0);
}


void borg_hw_init(){
	COLDDR = 0xFF;
	ROWDDR = 0x1F;
	COLPORT = 0xFF;
	ROWPORT &= ~0x1F;
	timer0_on();
}
