
#include <inttypes.h>
#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "config.h"
#include "scrolltext.h"

#define ROWPORT  PORTA
#define ROWDDR   DDRA
#define COLPORT  PORTC
#define COLDDR   DDRC

charmap pix_L = {4,{0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x0F}};
charmap pix_A = {5,{0x00,0x0E,0x11,0x11,0x11,0x1F,0x11,0x11}};
charmap pix_B = {4,{0x00,0x0E,0x09,0x09,0x0E,0x09,0x09,0x0E}};
charmap pix_O = {5,{0x00,0x0E,0x11,0x11,0x11,0x11,0x11,0x0E}};
charmap pix_R = {5,{0x00,0x1C,0x12,0x12,0x1C,0x14,0x12,0x11}};




unsigned char pixmap[] = {0x00,
			  0x00,
			  0x00,
			  0x00,
			  0x00,
			  0x00,
			  0x00,
			  0x00};
unsigned char shl_table[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

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



void joern1(){
unsigned char i;
		unsigned char j;
		for(i = 0; i< 255;i++){
			for(j = 0 ;j < 8; j++){
				pixmap[i]=shl_table[j];	
				pixmap[(i+1)%8]=shl_table[(j+1)%8];
				pixmap[(i+2)%8]=shl_table[(j+2)%8];
				pixmap[(i+3)%8]=shl_table[(j+3)%8];
				pixmap[(i+4)%8]=shl_table[(j+4)%8];
				pixmap[(i+5)%8]=shl_table[(j+5)%8];
				pixmap[(i+6)%8]=shl_table[(j+6)%8];
				pixmap[(i+7)%8]=shl_table[(j+7)%8];
			}
				wait(50);
		}
		pixmap[0] = 0x01;
		pixmap[1] = 0x00;
		pixmap[2] = 0x00;
		pixmap[3] = 0x00;
		pixmap[4] = 0x00;
		pixmap[5] = 0x00;
		pixmap[6] = 0x00;
		pixmap[7] = 0x00;
		wait (500);
		pixmap[7] = 0xf0;
                pixmap[0] = 0x0f;
                pixmap[1] = 0xf0;
                pixmap[2] = 0x0f;
                pixmap[3] = 0xf0;
                pixmap[4] = 0x0f;
                pixmap[5] = 0xf0;
                pixmap[6] = 0x0f;
		wait(500);

}

void clear_screen(unsigned char value){
	unsigned char x;
	for(x=0;x<8;x++){
		pixmap[x]=value;
	}
}


void setpixel(unsigned char x, unsigned char y){
	pixmap[y%8] |= shl_table[x%8];
}

void clearpixel(unsigned char x, unsigned char y){
	pixmap[y%8] &= ~shl_table[x%8];
}

unsigned char get_pixel(unsigned char x, unsigned char y){
	if (pixmap[y%8] & shl_table[x%8])return 1;
	return 0;
}


struct{
	unsigned char x;
	unsigned char y;
	enum{clear, set} mode;
	enum{right,left,up,down} direction;
}CURSOR;

void set_cursor(unsigned char x, unsigned char y){
	CURSOR.x = x;
	CURSOR.y = y;
	switch (CURSOR.mode){
		case clear:
			clearpixel(x,y);
			break;
		case set:
			setpixel(x,y);
			break;
	}
}

void walk(unsigned char steps, unsigned int delay){
	unsigned char x;
	if(steps > 0){
		for(x=0;x<steps;x++){
			switch (CURSOR.direction){
				case right:
					set_cursor(CURSOR.x+1, CURSOR.y);
					break;
				case left:
					set_cursor(CURSOR.x-1, CURSOR.y);
					break;
				case down:
					set_cursor(CURSOR.x, CURSOR.y+1);
					break;
				case up:
					set_cursor(CURSOR.x, CURSOR.y-1);
					break;
	
			}
			wait(delay);
		}
	}
	else {
		set_cursor (CURSOR.x,CURSOR.y);
	}
}

unsigned char get_next_pixel(){
	switch (CURSOR.direction){
		case right:
			return get_pixel(CURSOR.x+1,CURSOR.y);
			break;
		case left:
			return get_pixel(CURSOR.x-1,CURSOR.y);
			break;	
		case up:
			return get_pixel(CURSOR.x,CURSOR.y-1);
			break;
		case down:
			return get_pixel(CURSOR.x,CURSOR.y+1);
			break;
	}
}



int
main (void){
	init_Ports();
	timer0_on();
	sei();
	for(;;){

	unsigned int delay = 40;

	shift_in(&pix_L, delay);
	shift_in(&pix_A, delay);
	shift_in(&pix_B, delay);
	shift_in(&pix_O, delay);
	shift_in(&pix_R, delay);

	shift_out(8,delay);

//		joern2();
//		schachbrett(20);
//		tixiv1();
//		testline();
//		test1();
//		draw_l();

//		clear_screen;
//     		wait(500);	
	}
 }
