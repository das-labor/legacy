

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include "borg_hw.h"

#define ROWPORT  PORTD
#define ROWDDR DDRD

#define COLPORT PORTB
#define COLDDR DDRB

#define PIN_DATA     PB3
#define PIN_CLK      PB5 /*active low*/


unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];


inline void rowshow(unsigned char row, unsigned char plane){
	

	unsigned char b, d, x;
	ROWPORT = 0xFF;
	for(b=0;b<6;b++){
		d = pixmap[plane][row][b];
		for(x=0;x<8;x++){
			if(d & 0x01){
				COLPORT &= ~(1<<PIN_DATA);
			}else{
				COLPORT |= (1<<PIN_DATA);
			}
			d>>=1;
			COLPORT &= ~(1<<PIN_CLK);
			COLPORT |= (1<<PIN_CLK);
		}
	}
		
				d = pixmap[plane][row][6];
				PORTC  =  (PORTC & 0xE7 ) | ((~d << 3) & 0x18 );

		ROWPORT &=  ~(0x80>>(row));
	
	switch (plane){
		case 0:
			TCNT0 = 0x100-12;
			break;
		case 1:
			TCNT0 = 0x100-20;
			break;
		case 2:
			TCNT0 = 0x100-50;
	}
		
}


ISR(SIG_OVERFLOW0)
{
	static unsigned char plane = 0;
	static unsigned char row = 0;
	
	rowshow(row, plane);
	
//	PORTB ^= (1<<PB2);   // Toggel Board Led on Interupt
	
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
	COLPORT |= (1<<PIN_CLK);
	COLDDR |= (1<<PIN_CLK) | (1<<PIN_DATA);
	
	ROWPORT = 0x00;
	ROWDDR |= 0xFE; // 0=rxd, 1-7=rows (7)

	COLDDR |= (1<<PB2); // Set Status Led Pin Output

	DDRC = 0x18;
	

	timer0_on();
}
