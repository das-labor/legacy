
#include <avr/pgmspace.h>

#define SCROLLTEXT_C

#include "config.h"
#include "scrolltext.h"
#include "borg_hw.h"
#include "pixel.h"

extern void wait(int);


charmap pix_A PROGMEM = {5,{0x00,0x0E,0x11,0x11,0x11,0x1F,0x11,0x11}};
charmap pix_B PROGMEM = {4,{0x00,0x0E,0x09,0x09,0x0E,0x09,0x09,0x0E}};
charmap pix_C PROGMEM = {5,{0x00,0x0E,0x11,0x10,0x10,0x10,0x11,0x0E}};
charmap pix_D PROGMEM = {4,{0x00,0x0E,0x09,0x09,0x09,0x09,0x09,0x0E}};
charmap pix_E PROGMEM = {4,{0x00,0x0F,0x08,0x08,0x0F,0x08,0x08,0x0F}};
//F
charmap pix_G PROGMEM = {5,{0x00,0x0E,0x11,0x10,0x17,0x11,0x11,0x0E}};
//H
charmap pix_I PROGMEM = {1,{0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01}};
//J
charmap pix_K PROGMEM = {5,{0x00,0x11,0x12,0x14,0x18,0x14,0x12,0x11}};
charmap pix_L PROGMEM = {4,{0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x0F}};
//M
charmap pix_N PROGMEM = {5,{0x00,0x19,0x19,0x15,0x15,0x15,0x13,0x13}};
charmap pix_O PROGMEM = {5,{0x00,0x0E,0x11,0x11,0x11,0x11,0x11,0x0E}};
//P
//Q
charmap pix_R PROGMEM = {5,{0x00,0x1C,0x12,0x12,0x1C,0x14,0x12,0x11}};
charmap pix_S PROGMEM = {4,{0x00,0x07,0x08,0x08,0x06,0x01,0x01,0x0E}};
charmap pix_T PROGMEM = {5,{0x00,0x1F,0x04,0x04,0x04,0x04,0x04,0x04}};
//U
//V
charmap pix_W PROGMEM = {7,{0x00,0x41,0x41,0x49,0x49,0x55,0x22,0x22}};
//X
//Y
charmap pix_Z PROGMEM = {5,{0x00,0x1F,0x01,0x02,0x04,0x08,0x10,0x1F}};

charmap pix__ PROGMEM = {4,{0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00}};

void shift_in(const charmap * PROGMEM cm, unsigned int delay){
	unsigned char i, y, ror;
	unsigned char len = pgm_read_byte(&cm->len);
	ror = shl_table[len-1];
	for(i=0;i<len;i++){
		shift_pixmap_l();
		for(y=0;y<7;y++){
			setpixel((pixel){NUM_COLS-1, y}, (pgm_read_byte(&cm->pix[y+1]) & ror)?3:0 );
		}
		ror>>=1;
		wait(delay);
	}
	
	for(i=0;i<2;i++){
		shift_pixmap_l();
		wait(delay);
	}
}


void shift_out(unsigned char cols, unsigned int delay){
	unsigned char i;
	for(i=0;i<cols;i++){
		shift_pixmap_l();
		wait(delay);
	}
}

void labor_borg(){
	clear_screen(0);
	unsigned int delay = SCROLLTXT_DELAY;

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

	shift_out(4,delay);

	shift_in(&pix__, delay);
	
	shift_out(4,delay);
	
	shift_in(&pix_W, delay);
	shift_in(&pix_I, delay);
	shift_in(&pix_D, delay);
	shift_in(&pix_E, delay);
	shift_in(&pix_R, delay);
	shift_in(&pix_S, delay);
	shift_in(&pix_T, delay);
	shift_in(&pix_A, delay);
	shift_in(&pix_N, delay);
	shift_in(&pix_D, delay);
	
	shift_out(5,delay);
	
	shift_in(&pix_I, delay);
	shift_in(&pix_S, delay);
	shift_in(&pix_T, delay);
	
	shift_out(5,delay);
	
	shift_in(&pix_Z, delay);
	shift_in(&pix_W, delay);
	shift_in(&pix_E, delay);
	shift_in(&pix_C, delay);
	shift_in(&pix_K, delay);
	shift_in(&pix_L, delay);
	shift_in(&pix_O, delay);
	shift_in(&pix_S, delay);
			
	shift_out(NUM_COLS+8,delay);
}

