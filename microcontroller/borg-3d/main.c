#define MAIN_C

#include <avr/io.h>
//#include <stdlib.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "display.h"
#include "uart.h"
#include "pixel.h"
#include "anim3d.h"

#define DISPLAY_PORT PORTA
#define DISPLAY_ENABLE_PORT DDRA

#define ROWPORT  PORTC
#define ROWDDR   DDRC
#define COLPORT  PORTD
#define COLDDR   DDRD

#define MAXPLANE 3

unsigned char pixmap[NUMPLANE][6];

SIGNAL(SIG_OUTPUT_COMPARE0)
{
	static unsigned char plane = 0;
	static unsigned char count = 0;
	
	count ++;
	if (count >= MAXPLANE) {
		count = 0;
	}
	ROWPORT = pixmap[plane][count];
	COLPORT = ((pixmap[plane][count+3] & 7) << 5) | (1 << (count + 2));
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
	TCNT0 = 0;		// reset timer
	OCR0  = 0x10;	// Compare with this value
	TIMSK = 0x02;	// Compare match Interrupt on
}

void timer0_off(){
	TIMSK = 0;
}

void init_Ports(){
	ROWDDR = 0xFF;
	COLDDR = 0xFF;
	ROWPORT = 0;
	COLPORT = 0;
}




int main(void) {
	
	display_init();
	
	display_cursor(0, 0);
	display_printChar('#');
	
	init_Ports();
	timer0_on();
	sei();
	//uart_init();
  	
	while (1) {
		snake();
		schachBlinking2();
		cool(90, 1);
		schachBlinking();
		test3();
		cool(60, 3);
		test1(); 
		cool(30, 9);
		test3();		
	}
}
