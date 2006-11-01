
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "borg_hw.h"

unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];
#define COLDDR DDRC
#define COLPORT PORTC
#define ROWPORT PORTB
#define COLDATA PC1
#define COLCLK PC0
#define ROWDDR DDRB

inline void rowshow(unsigned char row, unsigned char plane){
	static uint8_t rowmask;
	uint8_t i;
	uint8_t tmp;
	ROWPORT = 0;
	
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


	
	if (row == 0){
		rowmask=2;
	}else{
		rowmask <<= 1;
	}
	tmp = pixmap[plane][row][1];
	for (i = 0;i<8;i++){
		if (tmp & 0x80){
			COLPORT |= (1<<COLDATA);
		}else{
			COLPORT &= ~(1<<COLDATA);
		}	
		COLPORT |= (1<<COLCLK);
	       	COLPORT &= ~(1<<COLCLK);	
        	tmp <<= 1;
	}
	tmp = pixmap[plane][row][0];
	for (i = 0;i<8;i++){
		if (tmp & 0x80){
			COLPORT |= (1<<COLDATA);
		}else{
			COLPORT &= ~(1<<COLDATA);
		}	
		COLPORT |= (1<<COLCLK);
	       	COLPORT &= ~(1<<COLCLK);	
        	tmp <<= 1;
	}

	
	ROWPORT = rowmask;	
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
	COLDDR |= 0x03;
	ROWDDR = 0xFF;
	ROWPORT = 0;
	timer0_on();
}

