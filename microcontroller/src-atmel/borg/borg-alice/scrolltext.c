#define SCROLLTEXT_C
#include "scrolltext.h"
#include "pixel.h"
#include "font_alice.h"
#include <avr/pgmspace.h>
extern void wait(int);
void shift_in(charmap * cm, unsigned int delay){
	unsigned char j;
	char i;	
	for(i=cm->len-1;i>-1;i--){
		shift_pixmap_l();
		for(j=0;j<5;j++){
			if(cm->pix[j]>>i & 0x01){
				setpixel((pixel){15,j},3);
			}
			else{
				setpixel((pixel){15,j},0);
			}
		}
		wait(delay);
	}
	shift_pixmap_l();
	wait(delay);
}

void alice_putchar(char c){
		if (c>=0x41 && c<=0x5A){
			shift_in(&alicefont[c-0x41],60);
		}
		else{	
			switch(c){
				case '\n':
					shift_in(&alicefont[26],60);
					shift_in(&alicefont[26],60);
					break;
				case ' ':
					shift_in(&alicefont[26],60);
					break;
				case '!':
					shift_in(&alicefont[27],60);
					break;
				case '\'':
					shift_in(&alicefont[28],60);
					break;
				case '"':
					shift_in(&alicefont[29],60);
					break;
				case '(':
					shift_in(&alicefont[30],60);
					break;
				case ')':
					shift_in(&alicefont[31],60);
					break;
				case '*':
					shift_in(&alicefont[32],60);
					break;
				case ',':
					shift_in(&alicefont[33],60);
					break;
				case '-':
					shift_in(&alicefont[34],60);
					break;
				case '.':
					shift_in(&alicefont[35],60);
					break;
				case ':':
					shift_in(&alicefont[36],60);
					break;
				case ';':
					shift_in(&alicefont[37],60);
					break;
				case '?':
					shift_in(&alicefont[39],60);
					break;
				case '`':
					shift_in(&alicefont[29],60);
					break;
			}
		}
		


}
void alice_putstr(char * str){
	unsigned char c;
	while((c = pgm_read_byte(str)) != 0x00){
		alice_putchar(c);
		str++;	
	}
}
//void shift_out(unsigned char cols, unsigned int delay){
//	unsigned char i, j;
//	unsigned char plane, col;
//	for(i=0;i<cols;i++){
//		for(j=0;j<16;j++){
//			col =(pixmap[0][j]>>1);
//			for(plane=0; plane < NUMPLANE; plane++)
//				pixmap[plane][j] = col;
//		}
//		wait(delay);
	//}
//}

