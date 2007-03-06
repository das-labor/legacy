#include <setjmp.h>
#include <avr/interrupt.h>

#include "config.h"
#include "scrolltext.h"
#include "programm.h"
#include "borg_hw.h"
#include "pixel.h"
//#include "menu.h"
#include "buttons.h"
#include "blinkenlights.h"
#include "pong.h"

volatile unsigned char oldMode, oldOldmode, mode;

jmp_buf newmode_jmpbuf;

int main (void){
	clear_screen(0);
	borg_hw_init();

	sei();
	
	mode = setjmp(newmode_jmpbuf);
	oldOldmode = oldMode;
	for(;;){
		oldMode = mode;
		switch(mode++) {
		case 1:
			scrolltext("</#XXX");
//			scrolltext(SCROLLTEXT_DEFAULT);
			break;
		case 2:
			testbuttons();
//			spirale(5);
			break;
		case 3:
			play_pong();
//			joern1();
			break;
		case 4:
			test_blinkenlights();
//			snake();
			break;
		case 5:
			schachbrett(20);
			break;
		case 6:
			feuer();
			break;
		case 7:
			matrix();
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
//			menu();
			mode = oldOldmode;
			break;
		default:
			break;
		}
	}
}
