#include <setjmp.h>
#include <avr/interrupt.h>
#include "config.h"
#include "scrolltext.h"
#include "programm.h"
#include "borg_hw.h"
#include "borg_can.h"
#include "pixel.h"
#include "uart.h"
#include "borg_can.h"
#include "util.h"

jmp_buf newmode_jmpbuf;

int main (void){
	unsigned char mode;

	borg_hw_init();
	uart_init();
	sei();
	bcan_init();
	clear_screen(0);
	mode = setjmp(newmode_jmpbuf);
	
	joy_init();
	joystick_test();
	
	for(;;) {
		switch(mode++) {
		case 0xff:
			clear_screen(0);
			while (1) wait(5);
			break;
		case 1:
        	movingArrows();
        	growingCubeFilled();
			break;
		case 2:
			scrolltext(scrolltext_text);
        		growingCubeFilled();
	        	break;
		case 3:
			joern1();
			growingCubeFilled();
			break;
		case 4:
			rotatePixmap(1);
			//gameOfLife(1, 400);
			//growingCubeFilled();
			break;
		case 5:
			laborZylinder();
			//gameOfLife(0, 400);
			//growingCubeFilled();
			//growingCubeFilled();
			break;
		case 6:
			gameOfLife(2, 150);
			growingCubeFilled();
			growingCubeFilled();
			growingCubeFilled();
			growingCubeFilled();
			break;
		case 7:
			spirale();
			//spirale2();
			break;
		case 8:
			matrix();
			//snake3dJoystick();
			break;
		case 9:
			test3();
			coolAnim();
			growingCubeFilled();
			growingCubeFilled();
			coolAnim();
			test2();
			test2();
			test2();
			test2();
			break;
		case 10:
			snake3d();
			break;
		case 11:
			feuer();
			break;
		case 12:
			rotatePixmap(0);
			break;
		case 13:
			rotatePixmap(2);
			break;
		case 14:
			testRotate();
		case 20:
			planeAnimation2(80);
			break;
		case 42:
			mode = 1;
			break;
		}
	}
}
