
#include "scrolltext.h"

extern unsigned char pixmap[];
extern void wait(int);

void shift_in(charmap * cm, unsigned int delay){
	unsigned char i, j;	
	for(i=0;i<cm->len;i++){
		for(j=0;j<8;j++){
			pixmap[j]=(pixmap[j]>>1)|(  ( 0x01 & (cm->pix[j]>>(cm->len-i-1)) )  ?  0x80:0   );
		}
		wait(delay);
	}
	
	for(i=0;i<2;i++){
		for(j=0;j<8;j++){
			pixmap[j]=(pixmap[j]>>1);
		}
		wait(delay);
	}
}


void shift_out(unsigned char cols, unsigned int delay){
	unsigned char i, j;
	for(i=0;i<cols;i++){
		for(j=0;j<8;j++){
			pixmap[j]=(pixmap[j]>>1);
		}
		wait(delay);
	}
}
