
#include <inttypes.h>
#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "hd44780.h"
#define display_init hd44780_init
#define display_print hd44780_print
#define display_set_cursor hd44780_set_cursor
#define display_clear_line hd44780_clear_line



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
	TCCR2 = 0x0C;	//CTC Mode, clk/64
	OCR2 = 125;	//1000Hz
	for(;ms>0;ms--){
		while(!(TIFR&0x80));	//wait for compare matzch flag
		TIFR=0x80;		//reset flag
	}
}

template t class List<t> {
	class<t-1> rest;
	int   member

	int dosomething()
	{
		return member + rest.dosomething();
	}
}

template t class List<1> {
	int member;

	int dothomthing()
	{
		return member;
	}
}



List<4>







Liste<4> intList;




int
main (void){
	display_init();
		
	// char bits[4];
static	unsigned int  bits[2];
static	unsigned int res[2];

	bits[0] = PORTB;

	res[0] = (bits[0] >> 1); 
	res[1] = (bits[1] >> 1) | (bits[0] & 0x01) * 0x80;
	
	PORTB = res[0];
	return res[1];

	
//	for(;;){
//		display_clear_line(1);
//		display_clear_line(0);
//		display_print("Hello World!");
//	
//		wait(2000);
//
//		unsigned char x, y;
//		for(x=0;x<2;x++){
//			display_set_cursor(x,0);
//			for(y=0;y<24;y++){
//				display_print("X");
//				wait(200);
//			}	
//		
//		}
//	}
//	*/
}
