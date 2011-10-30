#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/wdt.h>
#include "mood-hw.h"

uint8_t bright_a[64][4];
uint8_t bright_b[64][4];

SIGNAL(SIG_OUTPUT_COMPARE0)
{
	static uint8_t row=0;
	static uint8_t cycle=0;
	uint8_t rowmask;
	
	wdt_reset();
	
	PORTA = 0x00;
	
	if (++row > 3){
		row = 0;
		if (++cycle>63)
			cycle=0;
	}

	// calc row-mask
	rowmask = 1 << (row + 4);
	
	PORTC = rowmask;
	
	
	// set values
	PORTA = bright_a[cycle][row];
}

void timer0_off(){
	PORTA = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
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
	TCCR0 = 0x0A;	// CTC Mode, clk/8
	TCNT0 = 0;	// reset timer
	OCR0  = 128;	// Compare with this value
	TIMSK = 0x02;	// Compare match Interrupt on
}

void mood_init()
{
 	DDRC = 0xFF; // debug output
 	DDRA = 0xFF; // output
	wdt_reset();
	wdt_enable(0x00);
	timer0_on();
}


