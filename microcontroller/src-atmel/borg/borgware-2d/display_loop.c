
#include <setjmp.h>
#include <stdio.h>

#include "config.h"
#include "scrolltext/scrolltext.h"
#include "animations/snake.h"
#include "animations/programm.h"
#include "animations/matrix.h"
#include "animations/gameoflife.h"
#include "animations/stonefly.h" 
#include "animations/flyingdots.h"
#include "animations/breakout_demo.h"
#include "animations/ltn_ant.h"
#include "animations/amphibian.h"
#include "animations/laborlogo.h"
#include "animations/27c3.h"
#include "animations/fpmath_patterns.h"
#include "animations/mherweg.h"
#include "borg_hw/borg_hw.h"
#include "can/borg_can.h"
#include "random/prng.h"
#include "random/persistentCounter.h"
#include "mcuf/mcuf.h"
#include "menu/menu.h"
#include "pixel.h"
#include "joystick/joystick.h"


volatile unsigned char oldMode, oldOldmode, mode;

jmp_buf newmode_jmpbuf;

#ifndef MENU_SUPPORT
void snake_game(void);
void tetris(void);
void tetris_bastet(void);
void tetris_fp(void);
void borg_invaders(void);
void borg_breakout(unsigned char demomode);
#endif

void display_loop(){
//	mcuf_serial_mode();

	mode = setjmp(newmode_jmpbuf);
	oldOldmode = oldMode;

#ifdef JOYSTICK_SUPPORT
	waitForFire = 1;
#endif

	for(;;){
#ifndef MENU_SUPPORT
		clear_screen(0);
#endif
		oldMode = mode;
		switch(mode++) {

#ifdef ANIMATION_SCROLLTEXT
		case 1:
			scrolltext(scrolltext_text);

#ifdef RANDOM_SUPPORT
			{
				char a[28];
				sprintf(a,"</# counter == %lu  ", (unsigned long) percnt_get());
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
			snake_animation();
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
			random_bright(30);
			break;
#endif

#ifdef ANIMATION_STONEFLY 
		case 9: 
			stonefly(); 
			break; 
#endif 
 
#ifdef ANIMATION_GAMEOFLIFE
		case 10:
			gameoflife();
			break;
#endif

#ifdef ANIMATION_FLYINGDOTS
		case 11:
			flyingdots();
			break;
#endif

#ifdef ANIMATION_BREAKOUT
		case 12:
			breakout_demo();
			break;
#endif

#ifdef ANIMATION_MHERWEG
		case 13:
			lines1();
			dots1();
			movinglines();
			checkbox();
			rectangle1();
			rectangles();
			break;
#endif

#ifdef ANIMATION_LTN_ANT
		case 14:
			ltn_ant();
			break;
#endif

#ifdef ANIMATION_LABORLOGO
		case 15:
			laborlogo();
			break;
#endif

#ifdef ANIMATION_AMPHIBIAN
		case 16:
			amphibian();
			break;
#endif

#ifdef ANIMATION_LOGO_27C3
		case 17:
			logo_27c3();
			break;
#endif

#ifdef ANIMATION_PLASMA
		case 18:
			plasma();
			break;
#endif

#ifdef ANIMATION_PSYCHEDELIC
		case 19:
			psychedelic();
			break;
#endif


#ifdef ANIMATION_TESTS
		case 31:
			test_level(1);
			break;

		case 32:
			test_level(2);
			break;

		case 33:
			test_level(3);
			break;

		case 35:
			test_palette();
			test_palette2();
			break;
#endif

#ifdef MENU_SUPPORT
		case 42: 
			mode = 1;
			break;

		case 43:
			menu();
			mode = oldOldmode;
#else
		case 42:
			if (JOYISFIRE)
				mode = 43;
			else
				mode = 1;
			break;

		case 43:
			waitForFire = 0;   // avoid circular jumps
			while (JOYISFIRE); // wait until user released the fire button
			wait(25);          // wait for button to settle

#ifdef GAME_TETRIS
			tetris();
#endif

#ifdef GAME_BASTET
			tetris_bastet();
#endif

#ifdef GAME_TETRIS_FP
			tetris_fp();
#endif

#ifdef GAME_SPACE_INVADERS
			borg_invaders();
#endif

#ifdef GAME_SNAKE
			snake_game();
#endif

#ifdef GAME_BREAKOUT
			borg_breakout(0);
#endif

			while (JOYISFIRE); // avoid an unwanted restart of the game loop
			wait(25);          // wait for button to settle
			mode = oldOldmode; // restore old animation mode
			waitForFire = 1;   // reenable joystick query of the wait() function
			break;
#endif

#ifdef ANIMATION_OFF
		case 0xFF:
			off();
			break;
#endif
		}
	}
}
