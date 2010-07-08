
#include "pixel.h"
#include "borg_hw.h"
#include "config.h"

uint8_t shl_table[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

void clear_screen(uint8_t value){
	uint8_t *pix,v=0;
	unsigned int i;
	pix=&pixmap[0][0];
	if(value) v = 0xff;
	for(i=0;i<NUM_ROWS*LINEBYTES;i++)
		pix[i]=v;
}

void setpixel(uint8_t x, uint8_t value ){
	uint8_t y;
	y = x/NUM_COLS;
	x %= NUM_COLS;
	uint8_t pos = (y%NUM_ROWS)*LINEBYTES + (x/8);
	uint8_t mask = shl_table[x%8];
	if(value){
		pixmap[0][pos]|=mask;
	}else{
		mask ^=0xff;
		pixmap[0][pos]&=mask;
	}
}
