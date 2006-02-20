#include "invaders.h"

void drawIntroScreen(int ms)
{
	clear_screen(0);
	int np; 
	for(np = 0; np < NUMPLANE; ++np) {
		pixmap[np][4][1] = 0x04; pixmap[np][4][0] = 0x10;
		pixmap[np][5][1] = 0x02; pixmap[np][5][0] = 0x20;
		pixmap[np][6][1] = 0x07; pixmap[np][6][0] = 0xF0;
		pixmap[np][7][1] = 0x0D; pixmap[np][7][0] = 0xD8;
		pixmap[np][8][1] = 0x1F; pixmap[np][8][0] = 0xFC;
		pixmap[np][9][1] = 0x17; pixmap[np][9][0] = 0xF4;
		pixmap[np][10][1] = 0x14; pixmap[np][10][0] = 0x14;
		pixmap[np][11][1] = 0x03; pixmap[np][11][0] = 0x60;
	
	}
	wait(ms);
}

void drawCleared()
{
	clear_screen(0);
	wait(100);
	clear_screen(1);
	wait(100);
	clear_screen(0);
	wait(100);
	clear_screen(1);
	wait(100);
	clear_screen(0);
	wait(100);
	scrolltext("<80#CLEARED!");
	drawIntroScreen(750);
	//wait (750);
}
