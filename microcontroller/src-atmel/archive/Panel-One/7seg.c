#include <avr/pgmspace.h>
#include "pixel.h"




unsigned char numbers[] PROGMEM = {
	0x3f,//0
	0x06,//1
	0x5B,//2
	0x4f,//3
	0x66,//4
	0x6d,//5
	0x7d,//6
	0x07,//7
	0x7f,//8
	0x6f,//9	
};

unsigned char letters[] PROGMEM = {
	0x77,//A
	0x7c,//b
	0x39,//C
	0x5e,//d
	0x79,//E
	0x71,//F
	0x6f,//g
	0x74,//h
	0x30,//I
	0x1e,//J
	0x80,
	0x38,//L	
};

unsigned char field[] PROGMEM ={
	9,4,8,3,7,2,10,6,1,5,0
};

#define PGMB(a) pgm_read_byte(&(a))

void seg_print(unsigned char* txt){
	unsigned char c, fp = 6, dat, row;
	while((c=*txt++)){
			if (c >= 'a'){
				dat	= PGMB(letters[c-'a']);
			}else{
				dat = PGMB(numbers[c-'0']);	
			}
			row = PGMB(field[fp]);
			pixmap[0][row][0] = dat;
			pixmap[1][row][0] = dat;
			pixmap[2][row][0] = dat;
			fp--;
	}
}
