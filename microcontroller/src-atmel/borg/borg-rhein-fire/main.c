
#include <setjmp.h>
#include <avr/interrupt.h>

#include "config.h"
#include "scrolltext.h"
#include "programm.h"
#include "borg_hw.h"
#include "pixel.h"
#include "joystick.h"
#include "util.h"
#include "rhein.h"


volatile unsigned char oldMode, oldOldmode, mode;

jmp_buf newmode_jmpbuf;

int main (void){
	clear_screen(0);
	borg_hw_init();
	joy_init();	
	sei();

	mode = setjmp(newmode_jmpbuf);
	//setjmp(newmode_jmpbuf);
	oldOldmode = oldMode;
	waitForFire = 1;	

	


       // borg_invaders();


	/*
		taster JOYISUP

		dip1	JOYISDOWN
		dip2	JOYISLEFT
		dip3	JOYISRIGHT
	*/
/*
	while(1) {
		clear_screen(0);
		if(JOYISUP) setpixel((pixel){0,0}, 3);
		if(JOYISDOWN) setpixel((pixel){1,0}, 3);
		if(JOYISLEFT) setpixel((pixel){2,0}, 3);
		if(JOYISRIGHT) setpixel((pixel){3,0}, 3);
		wait(10);
		
	}
*/

	for(;;){
		if(JOYISRIGHT){
	 		test();
	 	 	wait(120);
		}

		oldMode = mode;
		switch(mode) {
		case 1:
			clear_screen(0);
			//running();		
			break;
		case 2:
			//spirale(5);
			scrolltext("</#FIRE vs. GALAXY");
			
			break;
		case 3:
			//joern1();
			//rhein_fire();
			rhein_matrix();
			break;
		case 4:
			scrolltext("</>2/#RHEIN FIRE");
			rhein_fire_logo(2000);
			scrolltext("b>10/<#RHEIN#</#FIRE");
			break;
		case 5:
			schachbrett(20);
			break;
		case 6:
			rhein_fire();
			break;
		case 7:
			matrix();
			break;
		case 8:	
			scrolltext("</#www.das-labor.org");
			break;
		case 9:
			feuer();
			break;

		case 14:
			mode=1;
			break;
		case 0xFF:
			off();
			break;
		case 43:
			//scrolltext("</#...");
			mode = oldOldmode;
			break;
		default:
			break;
		}
		if((!JOYISDOWN) || JOYISUP){
			while(JOYISUP) VERPEIL HIER;
			waitForFire = 1;
			mode++;
		}

	}
		
}
