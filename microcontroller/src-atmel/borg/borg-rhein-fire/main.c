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
#define SPEED "25"
#define SPEEDQUICK "20"

volatile unsigned char oldMode, oldOldmode, mode;

jmp_buf newmode_jmpbuf;

int main (void){
	unsigned char ani_cnt=1; 
	clear_screen(0);
	borg_hw_init();
	joy_init();	
	sei();

	int pattern;
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
		pattern = 0;
		if (JOYISDOWN)  pattern += 100;
		if (JOYISLEFT)  pattern += 10;
		if (JOYISRIGHT) pattern += 1;
		// 100
		switch(pattern) {
		case 001:
			running();
			scrolltext("<"SPEED"/#!!! TOUCHDOWN !!!");
			break;
		case 010:
                        scrolltext("</#Wer nicht h\x81pft der ist ein  \x83LNER");
                        wait(250);
			scrolltext(">/#hey");
                        wait(150);
			scrolltext("</#hey");
			break;
		case 100:
			rhein_fire(10000);
			break;
		case 110:
			scrolltext("<"SPEED"/#World Bowl Campion 2006:");
			scrolltext("<"SPEED"/#Frankfurt  Gay - Leck - Sie");
			wait(500);
			break;
		case 111:
			scrolltext("<"SPEED"/#World Bowl Campion 2006:");
			scrolltext("<"SPEED"/#Amsterdam Admirals");
			wait(500);
			break;
		}
		oldMode = mode;
		// In Testmode Controlled by DEFINE only Subprogram 1
		if (TESTMODE == 1){
			mode = 1;
		} 
		// Switch Subprogram
		switch(mode) {
		case 1:
			break;
		case 2:
                        scrolltext("<"SPEED"/#World Bowl XIV");
			scrolltext("<"SPEED"/#Rhein Fire");
			scrolltext("d/#vs");
			scrolltext("<"SPEED"/#Amsterdam Admirals");
			wait(250);
			break;
		case 3:
			running();
			scrolltext("<"SPEED"/#!!! TOUCHDOWN !!!");
			break;
		case 4:
			scrolltext("<"SPEEDQUICK"/#idea  and  realisation");
			scrolltext("<"SPEEDQUICK"/#DAS LABOR - Bochum");
			scrolltext("<"SPEEDQUICK"/#www.das-labor.org");
			break;
		case 5:
			rhein_fire(100);
			break;
		case 6:
			break;
		case 7:
			schachbrett(10);
			break;
		case 8:
                        break;
		case 9:
			running();
			scrolltext("<"SPEED"/#!!! TOUCHDOWN !!!");
			break;
		case 10:
			break;
			scrolltext("<"SPEED"/#Road to Worldbowl XIV");
			scrolltext("<"SPEED"/#18.3. Fire vs. Galaxy");
			scrolltext("d/#10:6");
			dropSmiley(SMILING);
			wait(250);
			scrolltext("<"SPEED"/#25.3. Fire @ Thunder");
			scrolltext(">/#22:0");
			dropSmiley(SMILING);
			wait(250);
			scrolltext("<"SPEED"/#2.4. Fire @ Centurions");
			scrolltext(">/#20:10");
			dropSmiley(SMILING);
			wait(250);
			scrolltext("<"SPEED"/#8.4. Fire vs. Sea Devils");
			scrolltext(">/#31:21");
			dropSmiley(SMILING);
			wait(250);
			scrolltext("<"SPEED"/#15.4. Fire @ Admirals");
			scrolltext(">/#31:35");
			dropSmiley(SAD);
			wait(250);
			scrolltext("<"SPEED"/#22.4. Fire vs. Admirals");
			scrolltext(">/#21:30");
			dropSmiley(SAD);
			wait(250);
			scrolltext("<"SPEED"/#29.4. Fire vs. Thunder");
			scrolltext(">/#27:24");
			dropSmiley(SMILING);
			wait(250);
			scrolltext("<"SPEED"/#6.5. Fire @ Galaxy");
			scrolltext(">/#14:16");
                        dropSmiley(SAD);
			wait(250);
			scrolltext("<"SPEED"/#14.5. Fire @ Sea Devils");
			scrolltext(">/#13:16");
                        dropSmiley(SAD);
			wait(250);
			scrolltext("<"SPEED"/#20.5. Fire vs. Centurions");
			//scrolltext(">/#21:30");
			wait(250);
			scrolltext("<"SPEED"/#27.5. World Bowl XIV");
			//scrolltext(">/#21:30");
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
		//if((!JOYISDOWN) || JOYISUP){
		//if((!JOYISDOWN) || (!JOYISLEFT) || (!JOYISRIGHT) || JOYISUP){
			while(JOYISUP) VERPEIL HIER;
			waitForFire = 1;
			mode++;
		//}
	}
}
