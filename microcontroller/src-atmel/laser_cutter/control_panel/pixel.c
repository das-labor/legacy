#define PIXEL_C
#include "pixel.h"

#include "borg_hw.h"

//#include "util.h"
#include "config.h"

unsigned char shl_table[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

void clear_screen(unsigned char value){
	unsigned char p,*pix,v=0xff;
	unsigned int i;
	for(p=0;p<NUMPLANE;p++){
		pix=&pixmap[p][0][0];
		if(p==value)
			v=0;
		for(i=0;i<NUM_ROWS*LINEBYTES;i++)
			pix[i]=v;
	}
}

void setpixel(unsigned char x, unsigned char value ){
	unsigned char y;
	y = x/NUM_COLS;
	x %= NUM_COLS;
	if(value>NUMPLANE)
		value=NUMPLANE;
	unsigned char pos = (y%NUM_ROWS)*LINEBYTES + (x/8);
	unsigned char mask = shl_table[x%8];
	unsigned char plane;
	for(plane=0;plane<value;plane++)
		pixmap[plane][0][pos]|=mask;
	mask ^=0xff;
	for(;plane<NUMPLANE;plane++)
		pixmap[plane][0][pos]&=mask;
}

