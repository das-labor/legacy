#define PIXEL_C

#include "config.h"

#include "pixel.h"
#include "borg_hw/borg_hw.h"

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

void setpixel(pixel p, unsigned char value ){
	p.x %= NUM_COLS;
	if(value>NUMPLANE)
		value=NUMPLANE;
	unsigned char pos = (p.y%NUM_ROWS)*LINEBYTES + (p.x/8);
	unsigned char mask = shl_table[p.x%8];
	unsigned char plane;
	for(plane=0;plane<value;plane++)
		pixmap[plane][0][pos]|=mask;
	mask ^=0xff;
	for(;plane<NUMPLANE;plane++)
		pixmap[plane][0][pos]&=mask;
}


//shifts pixmap left. It is really shifted right, but because col0 is left in the Display it's left.
void shift_pixmap_l(){
	unsigned char plane, row, byte;
	
	for(plane=0; plane<NUMPLANE; plane++){
		for(row=0;row<NUM_ROWS; row++){
			for(byte=0;byte<(LINEBYTES-1);byte++){
				pixmap[plane][row][byte] >>= 1;
				pixmap[plane][row][byte] |= (pixmap[plane][row][byte+1] & 0x01) * 0x80;;
			}
				pixmap[plane][row][LINEBYTES-1] >>= 1;
		}
	}
}


unsigned char get_pixel(pixel p){

	if( (p.x >= NUM_COLS) || (p.y >= NUM_ROWS) ){
		return 0xff;
	}else{
		return 0!= (pixmap[0][p.y][p.x/8] & shl_table[p.x%8]);
	}
}
