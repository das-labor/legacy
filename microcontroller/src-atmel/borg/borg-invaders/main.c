
#include <setjmp.h>
#include <avr/interrupt.h>

#include "config.h"
#include "scrolltext.h"
#include "programm.h"
#include "borg_hw.h"
#include "pixel.h"
#include "borg_can.h"
#include "joystick.h"

volatile unsigned char oldMode, oldOldmode, mode;

jmp_buf newmode_jmpbuf;

int main (void){
	uint8_t r,p,l;
	clear_screen(0);
	borg_hw_init();
	bcan_init();
	joy_init();	
	sei();

	


	mode = setjmp(newmode_jmpbuf);
	oldOldmode = oldMode;
	waitForFire = 1;	


        borg_invaders();

	for(;;){
		oldMode = mode;
		switch(mode++) {
		case 1:
			scrolltext(scrolltext_text);
			break;
		case 2:
			spirale(5);
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
		default:
			break;
		}
	}

}
