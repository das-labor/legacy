
#include <inttypes.h>
#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "config.h"
#include "scrolltext.h"

#include "programm.h"

#define ROWPORT  PORTA
#define ROWDDR   DDRA
#define COLPORT  PORTC
#define COLDDR   DDRC

charmap pix_L = {4,{0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x0F}};
charmap pix_A = {5,{0x00,0x0E,0x11,0x11,0x11,0x1F,0x11,0x11}};
charmap pix_B = {4,{0x00,0x0E,0x09,0x09,0x0E,0x09,0x09,0x0E}};
charmap pix_O = {5,{0x00,0x0E,0x11,0x11,0x11,0x11,0x11,0x0E}};
charmap pix_R = {5,{0x00,0x1C,0x12,0x12,0x1C,0x14,0x12,0x11}};
charmap pix_G = {5,{0x00,0x0E,0x11,0x10,0x17,0x11,0x11,0x0E}};



unsigned char pixmap[] = {0x00,
			  0x00,
			  0x00,
			  0x00,
			  0x00,
			  0x00,
			  0x00,
			  0x00};

SIGNAL(SIG_OUTPUT_COMPARE0)
{
	static unsigned char count = 0;
	count ++;
	if ((COLPORT <<= 1) == 0){
		count = 0;
		COLPORT =1;
	}
	ROWPORT = pixmap[count];
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
	OCR0 = 0xFF;	// Compare with this value
	TIMSK = 0x02;	// Compare match Interrupt on
}

void timer0_off(){
	TIMSK = 0;
}

void wait(int ms){
/* 	TCCR2: FOC2 WGM20 COM21 COM20 WGM21 CS22 CS21 CS20
		CS22 CS21 CS20
		 0    0    0	       stop
		 0    0    1       clk
		 0    1    0       clk/8
		 0    1    1       clk/32
		 1    0    0       clk/64
		 1    0    1       clk/128
		 1    1    0       clk/256
		 1    1    1       clk/1024	
*/
	TCCR2 = 0x0D;	//CTC Mode, clk/8
	OCR2 = 125;	//1000Hz
	for(;ms>0;ms--){
		while(!(TIFR&0x80));	//wait for compare matzch flag
		TIFR=0x80;		//reset flag
	}
}

void init_Ports(){
	ROWDDR = 0xFF;
	COLDDR = 0xFF;
	ROWPORT = 0;
	COLPORT = 0;
}

void labor_borg(){
	unsigned int delay = 40;

	shift_in(&pix_L, delay);
	shift_in(&pix_A, delay);
	shift_in(&pix_B, delay);
	shift_in(&pix_O, delay);
	shift_in(&pix_R, delay);

	shift_out(5,delay);

	shift_in(&pix_B, delay);
	shift_in(&pix_O, delay);
	shift_in(&pix_R, delay);
	shift_in(&pix_G, delay);
	
	shift_out(16,delay);
}



int
main (void){
	init_Ports();
	timer0_on();
	sei();
	for(;;){

		joern1();
		snake();
		joern2();
		schachbrett(20);
		spirale(20);
		labor_borg();


//		testline();
//		test1();
//		draw_l();

//		clear_screen;
//     		wait(500);	
	}
}
