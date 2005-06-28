
#include "scrolltext.h"
#include "pixel.h"

extern void wait(int);

void shift_in(charmap * cm, unsigned int delay){
	unsigned char plane, col;
	unsigned char i, j;	
	for(i=0;i<cm->len;i++){
		for(j=0;j<8;j++){
			col = (pixmap[0][j]>>1)|(  ( 0x01 & (cm->pix[j]>>(cm->len-i-1)) )  ?  0x80:0   );
			for(plane=0; plane < NUMPLANE; plane++)
				pixmap[plane][j] = col;
		}
		wait(delay);
	}
	
	for(i=0;i<2;i++){
		for(j=0;j<8;j++){
			col = (pixmap[0][j]>>1);
			for(plane=0; plane < NUMPLANE; plane++)
				pixmap[plane][j] = col;
		}
		wait(delay);
	}
}


void shift_out(unsigned char cols, unsigned int delay){
	unsigned char i, j;
	unsigned char plane, col;
	for(i=0;i<cols;i++){
		for(j=0;j<8;j++){
			col =(pixmap[0][j]>>1);
			for(plane=0; plane < NUMPLANE; plane++)
				pixmap[plane][j] = col;
		}
		wait(delay);
	}
}
