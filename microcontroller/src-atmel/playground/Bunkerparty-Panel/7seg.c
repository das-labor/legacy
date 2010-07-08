#include <avr/pgmspace.h>
#include <stdlib.h>
#include "borg_hw.h"
#include "7seg.h"

uint8_t numbers[] PROGMEM = {
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

uint8_t letters[] PROGMEM = {
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
	0x80,//K - special 0
	0x38,//L
	0x81,//m - special 1
	0x54,//n
	0x5c,//o
	0x73,//P
	0x67,//q
	0x50,//r
	0x6d,//S
	0x78,//t
	0x1c,//u
	0x3c,//V
	0x82,//w - special 2
	0x76,//X
	0x66,//Y
	0x5b,//Z	
};

uint8_t letters_special[][2] PROGMEM ={
	{0x06,0x64},//K
	//{0x54,0x44},//m
	{0x33,0x27},//M
	{0x3c,0x1e},//W
};

uint8_t field[] PROGMEM ={
	9,4,8,3,7,2,10,6,1,5,0
};

#define PGMB(a) pgm_read_byte(&(a))

uint8_t fp = 10;

static void writec(uint8_t dat){
	uint8_t row;
	//dont write it, if the display is full.
	if(fp == 0xff)
		return;
	row = PGMB(field[fp]);
	pixmap[row][0] = dat;
	fp--;
}

void seg_putc(uint8_t c){
	uint8_t dat, row, uc;
	if(c == '\r'){
		uint8_t f;
		fp = 10;
		for(f=0;f<=10;f++){
			row = PGMB(field[f]);
			pixmap[row][0] = 0;	
		}
		return;		
	}
	if(c == '\n'){
		fp = 10;
		return;
	}
	uc = c & 0xdf;//convert potential letter to uppercase
	if (uc >= 'A' && uc<= 'Z'){
		dat = PGMB(letters[uc-'A']);
	}else if(c >= '0' && c<= '9'){
		dat = PGMB(numbers[c-'0']);	
	}else if(c == ' '){
		dat = 0;
	}else{
		return;
	}
	//check for special letter
	if(dat & 0x80){//special
		dat &= 0x7f;
		writec(PGMB(letters_special[dat][0]));
		writec(PGMB(letters_special[dat][1]));
	}else{
		writec(dat);
	}
}

void seg_putstr(char * txt){
	uint8_t c;
	while((c=*txt++)){
		seg_putc(c);			
	}
}

void seg_putstr_P(char * txt){
	uint8_t c;
	while((c=pgm_read_byte(txt++))){
		seg_putc(c);
	}
}

void seg_puthex(uint8_t c){
	char buf[3];
	itoa(c, buf, 16);
	if(buf[1] == 0)
		seg_putc('0');
	seg_putstr(buf);
}

