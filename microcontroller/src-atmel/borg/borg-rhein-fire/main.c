
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
#define SPEED "30"
#define SPEEDQUICK "20"

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
			scrolltext("<"SPEED"/#Rhein Fire Fans on Tour");
			wait(250);
			scrolltext("<"SPEED"/#Amsterdam, April 15th");
			wait(250);
			break;
		case 3:
			rhein_fire_logo(5000);
			break;
		case 4:
			scrolltext("<"SPEED"/#D\x81sseldorf Rhein Fire");
			scrolltext("d/#@");
			scrolltext("<"SPEED"/#Amsterdam Admirals");
			scrolltext("<"SPEED"/#Samstag 15. April 15:00");
			scrolltext("<"SPEED"/#Amsterdam ArenA");
			break;
		case 5:
			rhein_fire(100);
			break;
		case 6:
			scrolltext("<"SPEEDQUICK"/#Fire Jacket Borg");
			scrolltext("<"SPEEDQUICK"/#idea  and  realisation  by");
			scrolltext("<"SPEEDQUICK"/#Das Labor - Bochum");
			scrolltext("<"SPEEDQUICK"/#source code available at");
			scrolltext("<"SPEEDQUICK"/#www.das-labor.org");
			break;
		case 7:
			schachbrett(20);
			break;
		case 8:
			scrolltext("<"SPEED"/#Rhein Fire Fans on Tour");
			scrolltext("<"SPEED"/#Amsterdam ArenA");
			scrolltext("<"SPEED"/#saturday, april 15th 15:00");
			scrolltext("<"SPEED"/#D\x81sseldorf Rhein Fire");
			scrolltext("d/#@");
			scrolltext("<"SPEED"/#Amsterdam Admirals");
			break;
		case 9:
			running();
			scrolltext("<"SPEED"/#!!! TOUCHDOWN !!!");
			break;
		case 10:
			scrolltext("<"SPEED"/#Road to Worldbowl XIV");
			scrolltext("<"SPEED"/#18.3. Fire vs. Galaxy");
			scrolltext("d/#10:6");
			wait(250);
			//rhein_fire(10);
			scrolltext("<"SPEED"/#25.3. Fire @ Thunder");
			scrolltext(">/#22:0");
			wait(250);
			//rhein_fire(10);
			scrolltext("<"SPEED"/#2.4. Fire @ Centurions");
			scrolltext(">/#20:10");
			wait(250);
			//rhein_fire(10);
			scrolltext("<"SPEED"/#8.4. Fire vs. Sea Devils");
			scrolltext(">/#31:21");
			wait(250);
			//rhein_fire(10);
			scrolltext("<"SPEED"/#15.4. Fire @ Admirals");
			wait(250);
			//rhein_fire(10);
			scrolltext("<"SPEED"/#22.4. Fire vs. Admirals");
			wait(250);
			//rhein_fire(10);
			scrolltext("<"SPEED"/#29.4. Fire vs. Thunder");
			wait(250);
			//rhein_fire(10);
			scrolltext("<"SPEED"/#6.5. Fire @ Galaxy");
			wait(250);
			//rhein_fire(10);
			scrolltext("<"SPEED"/#14.5. Fire @ Sea Devils");
			wait(250);
			//rhein_fire(10);
			scrolltext("<"SPEED"/#20.5. Fire vs. Centurions");
			wait(250);
			//rhein_fire(10);
			scrolltext("<"SPEED"/#27.5. World Bowl XIV");
			wait(250);
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
