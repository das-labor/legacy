#include <avr/interrupt.h>
#include <avr/signal.h>

#include "util.h"

#define RED 0x1
#define GREEN 0x2
#define BLUE 0x4

uint8_t bright[8];

static uint8_t cycle=0;

SIGNAL(SIG_OUTPUT_COMPARE0)
{


	uint8_t newval = 0;
	uint8_t mask   = 1;
	uint8_t i;
	for(i=0;i<8;i++){
		if (cycle<birght[i])
			newval |=	mask;  
		mask >>= 1;
	}


	cycle = (cycle + 1) & 63;

	PORTA=newval;
}

void timer0_off(){
	cli();

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


int main (void){
 	DDRC = 0xFF; //output
 	DDRA = 0xFF; //output
	timer0_on();
	sei();
	bright[0]=0;
	bright[1]=0;
	bright[2]=0;
	while(1){
		wait(10);
	}
	
}
