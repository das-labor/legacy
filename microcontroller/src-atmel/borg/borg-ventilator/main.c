
#include <setjmp.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "config.h"
#include "scrolltext.h"
#include "programm.h"
#include "matrix.h"
#include "gameoflife.h"
#include "borg_hw.h"
#include "pixel.h"
#include "borg_can.h"
#include "joystick.h"
#include "prng.h"
#include "persistentCounter.h"
#include "menu.h"

char scrolltext_text[SCROLLTEXT_STRING_SIZE] = SCROLLTEXT_DEFAULT;

volatile unsigned char oldMode, oldOldmode, mode;

jmp_buf newmode_jmpbuf;

int main (void){
	clear_screen(0);
	srandom32(percnt_get());
	percnt_inc();
	borg_hw_init();
//	bcan_init();
//	joy_init();	
	sei();
	
	mode = setjmp(newmode_jmpbuf);
	oldOldmode = oldMode;
	waitForFire = 0;	
	for(;;){
		oldMode = mode;
		switch(mode++) {
		case 1:
			scrolltext(scrolltext_text);
			break;
		case 2:
			spirale(10);
			mode = 2;
			break;
		case 3:
			joern1();
			break;
		case 4:
			snake();
			break;
		case 5:
			schachbrett(20);
			break;
		case 6:
			feuer();
			break;
		case 7:
			matrix();
			break;
		case 8:
			random_bright(200);
			break;
		case 9:	
			//gameoflife();
			mode = 1;
			break;
		case 31:
			test_level1();
			break;
		case 32:
			test_level2();
			break;
		case 33:
			test_level3();
			break;
		case 34:
			fadein();
			break;
		case 35:
			test1();
			break;
		case 0xFF:
			off();
			break;
		case 43:
		//	snakeJoystick();
		//	menu();
			mode = oldOldmode;
		default:
			break;
		}
	}
}
