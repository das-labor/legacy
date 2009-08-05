#include <stdio.h>
#include "config.h"
#include "util.h"
#include "api.h"

color black = {  0,   0,   0};
color white = {255, 255, 255};
color red   = {255,   0,   0};
color green = {  0, 255,   0};
color blue  = {  0,   0, 255};

uint8_t imag[MAX_Y][MAX_X][COLOR_BYTES];

//Color order is GBR
//uint32_t (*pixmap_p)[NUM_HW_ROWS][NUM_HW_COLS] = (void *)0xf0040000;
//#define pixmap (*pixmap_p)

extern unsigned char pixmap[MAX_Y][MAX_X][COLOR_BYTES];

char uart_getchar()
{
        //return getc();
	return 0;
}

void uart_putchar(char c)
{
        putchar(c);
}

void uart_putstr(char *str)
{
        puts(str);
}

void initPwm(){
	int i, tmp;
	//init of the pwm table
	/*for (i = 0; i < 256; i++) {
		tmp = i/2;
		if (tmp < 14) 
			tmp = 14;
		PWMTABLE[i] = tmp;
	}*/
	//for (i = 0; i < 1024; i++) {
	//	((unsigned char*)pixmap)[i] = 0x10;
	//}
	//magic turn-on command
	//*((uint32_t *) 0xf0041d54) = 0x23;
}


void setPixel(pixel p, color c){
	if (p.x < MAX_X && p.y < MAX_Y) {
		uint8_t * im = (void *)&imag[p.y][p.x];
		*im++ = c.g;
		*im++ = c.b;
		*im = c.r;
	}
}


void swap(){
	int x,y;
	uint8_t * im; uint8_t * pix;
	for(y=0;y<MAX_Y;y++){
		pix = (void*)&pixmap[y];
		im = (void*)&imag[y];
		for(x=0;x<(COLOR_BYTES*MAX_X); x++){
			*pix++ = *im++;
		}
	}
}
