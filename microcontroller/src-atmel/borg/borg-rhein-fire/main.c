
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

// If Testmode 1 then only first Sub is executed
#define TESTMODE 0

volatile unsigned char oldMode, oldOldmode, mode;

jmp_buf newmode_jmpbuf;

int main (void){
	unsigned char ani_cnt=1; 
	clear_screen(0);
	borg_hw_init();
	joy_init();	
	sei();

	mode = setjmp(newmode_jmpbuf);
	oldOldmode = oldMode;
	waitForFire = 1;	
	//setjmp(newmode_jmpbuf);

/*
	taster	JOYISUP
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
		// In Testmode only Subprogram 1
		if (TESTMODE == 1)
			mode = 1;
		// Switch Subprogram
		switch(mode) {
		case 1:
			clear_screen(0);
			// test();
			// running();
			// scrolltext("</#!!! TOUCHDOWN !!!");
			// scrolltext("</#Hier k\x80nnte Ihre Werbung stehen");
			break;
		case 2:
			scrolltext("</#Rhein Fire");
			scrolltext("d/#vs");
			scrolltext("</#Sea Devils");
			break;
		case 3:
			rhein_fire_logo(5000);
			break;
		case 4:
			scrolltext("</#Sea Devils");
			scrolltext("u/#@");
			scrolltext("</#Rhein Fire");
			break;
		case 5:
			rhein_fire(100);
			break;
		case 6:
			scrolltext("</#Das Labor - Bochum");
			scrolltext("</#www.das-labor.org");
			break;
		case 7:
			schachbrett(20);
			break;
		case 8:
			scrolltext("</#D\x81sseldorf");
			scrolltext("</#Samstag 8. April 19:00 Uhr");
			break;
		case 9:
			running();
			scrolltext("</#!!! TOUCHDOWN !!!");
			break;
		case 10:
			scrolltext("</#Road to Worldbowl XIV");
			scrolltext("</#18.3. Fire vs. Galaxy");
			scrolltext("d/#10:6");
			//rhein_fire(10);
			scrolltext("</#25.3. Fire @ Thunder");
			scrolltext(">/#22:0");
			//rhein_fire(10);
			scrolltext("</#2.4. Fire @ Centurions");
			scrolltext(">/#20:10");
			//rhein_fire(10);
			scrolltext("</#8.4. Fire vs. Sea Devils");
			//rhein_fire(10);
			scrolltext("</#15.4. Fire @ Admirals");
			//rhein_fire(10);
			scrolltext("</#22.4. Fire vs. Admirals");
			//rhein_fire(10);
			scrolltext("</#29.4. Fire vs. Thunder");
			//rhein_fire(10);
			scrolltext("</#6.5. Fire @ Galaxy");
			//rhein_fire(10);
			scrolltext("</#14.5. Fire @ Sea Devils");
			//rhein_fire(10);
			scrolltext("</#20.5. Fire vs. Centurions");
			//rhein_fire(10);
			scrolltext("</#27.5. World Bowl XIV");
			break;
		case 11:
			spirale(5);
			joern1();
			break;
		case 12:
			switch (ani_cnt) {
				case 1:
					feuer();
					break;
				case 2:
					matrix();
					break;
				case 3:
					rhein_fire(1000);
					ani_cnt=0;
					break;
			}
			ani_cnt++;
			break;
		case 13:
			mode=1;
			break;
		case 0xFF:
			//off();
			break;
		case 43:
			mode = oldOldmode;
			break;
		default:
			break;
		}
		// JOYISDOWN ist der Mode-Halten Schalter
		// JOYISUP ist der Taster, der die Modes weiterschaltet		
		if((!JOYISDOWN) || JOYISUP){
			while(JOYISUP) VERPEIL HIER;
			waitForFire = 1;
			mode++;
		}
	}
}
