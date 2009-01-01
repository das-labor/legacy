
#include <setjmp.h>
#include <stdio.h>

#include "config.h"
#include "scrolltext/scrolltext.h"
#include "animations/programm.h"
#include "animations/matrix.h"
#include "animations/gameoflife.h"
#include "borg_hw/borg_hw.h"
#include "can/borg_can.h"
#include "random/prng.h"
#include "random/persistentCounter.h"
#include "mcuf/mcuf.h"
#include "menu/menu.h"
#include "pixel.h"
#include "joystick.h"


volatile unsigned char oldMode, oldOldmode, mode;

jmp_buf newmode_jmpbuf;

void display_loop(){
//	mcuf_serial_mode();
	
#ifdef RANDOM_SUPPORT
	percnt_inc();
#endif

	mode = setjmp(newmode_jmpbuf);
	oldOldmode = oldMode;

#ifdef JOYSTICK_SUPPORT	
	waitForFire = 1;
#endif

	for(;;){
		oldMode = mode;
		switch(mode++) {

#ifdef ANIMATION_SCROLLTEXT
		case 1:
			scrolltext(scrolltext_text);
			
#ifdef RANDOM_SUPPORT
			{ char a[14];
				sprintf(a,"</# counter == %lu  ", percnt_get());
				scrolltext(a);
			}
#endif
			break;
#endif

#ifdef ANIMATION_SPIRALE
		case 2:
			spirale(5);
			break;
#endif

#ifdef ANIMATION_JOERN1
		case 3:
			joern1();
			break;
#endif

#ifdef ANIMATION_SNAKE
		case 4:
			snake();
			break;
#endif

#ifdef ANIMATION_SCHACHBRETT
		case 5:
			schachbrett(20);
			break;
#endif			

#ifdef ANIMATION_FEUER
		case 6:
			feuer();
			break;
#endif

#ifdef ANIMATION_MATRIX
		case 7:
			matrix();
			break;
#endif

#ifdef ANIMATION_RANDOM_BRIGHT
		case 8:
			random_bright(200);
			break;
#endif

#ifdef ANIMATION_GAMEOFLIFE
		case 9:	
			gameoflife();
			break;
#endif

		case 29:
		  mode = 1;
		  break;

#ifdef ANIMATION_TESTS
		case 31:
			test_level1();
			break;
		case 32:
			test_level2();
			break;
		case 33:
			test_level3();
			break;
		case 35:
			test1();
			while(1);
			break;
#endif

#ifdef ANIMATION_OFF
		case 0xFF:
			off();
			break;
#endif

#ifdef MENU_SUPPORT
		case 43:
			menu();
			mode = oldOldmode;
#endif
		}
	}
}
