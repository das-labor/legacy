#include <setjmp.h>
#include <avr/interrupt.h>
#include "config.h"
#include "scrolltext.h"
#include "borg_hw.h"
#include "borg_can.h"
#include "pixel.h"
#include "uart.h"
#include "borg_can.h"
#include "util.h"
#include "joystick.h"
#include "programs.h"

jmp_buf newmode_jmpbuf;

volatile unsigned char oldMode, oldOldmode, mode;


int main (void) {
	borg_hw_init();
	uart_init();
	joy_init();
	sei();
#ifdef BORG_CAN
	bcan_init();
#endif
	clear_screen(0);

	mode = setjmp(newmode_jmpbuf);

	oldOldmode = oldMode;
	waitForFire = 1;
	for (;;) {
		oldMode = mode;
		switch(mode++) {

		case 0xff:
			clear_screen(0);
			while (1) wait(5);
			break;
		case 1:
			waves();
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
			growingCubeFilled();
			break;
		case 5:
			laborZylinder();
			growingCubeFilled();
			growingCubeFilled();
			break;
		case 6:
			gameOfLife(0, 150);
			growingCubeFilled();
			growingCubeFilled();
			growingCubeFilled();
			growingCubeFilled();
			break;
		case 7:
			spirale();
			break;
		case 8:
			matrix();
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
			break;
		case 15:
			movingArrows();
			break;
		case 16:
			flury();
			growingCubeFilled();
			break;
		case 20:
			planeAnimation2(80);
			break;
		case 21:
			rotatedScaledCube(); 
			break;
		case 42:
			mode = 1;
			break;
		case 43:
			pong();
			mode = oldOldmode;
			break;
		case 50:
			test1();
			break;
		case 51:
			setplane_test();
			break;
		case 52:
			joystick_test();
			break;	
		}
	}
}

