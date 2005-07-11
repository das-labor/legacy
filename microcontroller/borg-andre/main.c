
#include <inttypes.h>
#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "config.h"
#include "scrolltext.h"
#include "pixel.h"
#include "programm.h"
#include "util.h"

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


unsigned int pixmap[NUMPLANE][16];

SIGNAL(SIG_OUTPUT_COMPARE0)
{
	static unsigned char plane = 0;
	static unsigned char count = 0;
	
	COLPORT1 = 0;
	COLPORT2 = 0;
	
	if (count == 0){
		ROWPORT&= ~(1<<PIN_RST);
		ROWPORT|= (1<<PIN_RST);
		ROWPORT|= (1<<PIN_SHFT1);
		ROWPORT|= (1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_CLK);
		ROWPORT&= ~(1<<PIN_SHFT1);
		if ( ++plane == NUMPLANE )
			plane = 0;
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
	}else if(count == 8){
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
	
	COLPORT1 = pixmap[plane][count];
	COLPORT2 = (pixmap[plane][count])>>8;
	if(++count == NUM_ROWS)count = 0;
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

void timer0_off(){
	TIMSK = 0;
}

void init_Ports(){
	COLDDR1 = 0xFF;
	COLDDR2 = 0xFF;
	ROWDDR = 0xFF;
	COLPORT1 = 0;
	COLPORT2 = 0;
	ROWPORT = 0;
}

int main (void){
	clear_screen(0);
	init_Ports();
	timer0_on();
	sei();
	
	for(;;){
		matrix1();
		fadein();
		joern1();
		//test1();
		snake();
		schachbrett(20);
		spirale(20);
		labor_borg();
		feuer();
		

//		testline();
//		test1();
//		draw_l();

//		clear_screen;
//     		wait(500);	
	}
}
