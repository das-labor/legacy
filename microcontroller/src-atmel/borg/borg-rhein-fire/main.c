
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
  unsigned char ani_cnt=1; 
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
			//scrolltext("</#D~sseldorf Rhein Fire");			
			break;
		case 2:
		  scrolltext("</#Rhein Fire");
		  scrolltext("d/#@");
		  scrolltext("</#Berlin Thunder");
			// scrolltext("</#Rhein  Fire");
			break;
		case 3:
		  rhein_fire_logo(5000);
			break;
		case 4:
		  scrolltext("</#Rhein Fire");
		  scrolltext("u/#@");
		  scrolltext("</#Berlin Thunder");
		  //scrolltext("10u30-2+/#R#H#E#I#N# #F#I#R#E");
		  //scrolltext(">/#@");
		  //scrolltext("14d30-0+/#B#E#R#L#I#N# #T#H#U#N#D#E#R");
			break;
		case 5:		  
			rhein_fire(100);
			break;
		case 6:
		  scrolltext("</#www.das-labor.org");
		  scrolltext("</#Bochum");
			break;
		case 7:
		  schachbrett(20);			
			break;
		case 8:	
		  scrolltext("</#D~sseldorf Rhein Fire");			
			break;
		case 9:	
		  break;
		case 10:		  
		  switch (ani_cnt) {
		    case 1:
		      //scrolltext("<20/#Ani 1");
			    feuer();
			    break;
		    case 2:
		      //scrolltext("<20/#Ani 2");
			    matrix();
			    break;
		    case 3:
		      //scrolltext("<20/#Ani 3");
    			spirale(5);
    			break;
    		case 4:
    		  //scrolltext("<20/#Ani 4");
			    joern1();
			    break;
		    case 5:
		      //scrolltext("<20/#Ani 5");
			    rhein_fire(1000);
			    ani_cnt=0;
			    break;
			}
			ani_cnt++;
			break;
		case 11:
			mode=1;
			break;
		case 0xFF:
			//off();
			break;
		case 43:
			//scrolltext("</#...");
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
