
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
	clear_screen(0);
	borg_hw_init();
	bcan_init();
	joy_init();	
	sei();

	mode = setjmp(newmode_jmpbuf);
	oldOldmode = oldMode;
	waitForFire = 1;	

	/*
        while(1) {
          if (JOYISFIRE) 
		setpixel((pixel){0,1}, 3);
	  else
		setpixel((pixel){0,1}, 0);

          if (JOYISLEFT) 
		setpixel((pixel){0,2}, 3);
	  else
		setpixel((pixel){0,2}, 0);

          if (JOYISRIGHT) 
		setpixel((pixel){0,3}, 3);
	  else
		setpixel((pixel){0,3}, 0);

          if (JOYISUP) 
		setpixel((pixel){0,4}, 3);
	  else
		setpixel((pixel){0,4}, 0);

          if (JOYISDOWN) 
		setpixel((pixel){0,5}, 3);
	  else
		setpixel((pixel){0,5}, 0);
	}
	*/


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
