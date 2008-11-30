/**
 * Conways Game of life 
 * Author: Daniel Otte
 * License: GPLv3
 * 
 * 
 * rename this file to gameoflife.c ...
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/sfr_defs.h> /* for debugging */
#include "config.h"
#include "prng.h"
#include "pixel.h"
#include "util.h"
#include "scrolltext.h"
#include "joystick.h"

/******************************************************************************/ 




#undef DEBUG
 
#define XSIZE NUM_COLS
#define YSIZE NUM_ROWS

/* 
 *  last line is for debug information
 */
#ifdef DEBUG 
 #undef YSIZE
 #define YSIZE (NUM_ROWS-1)
 #define DEBUG_ROW (NUM_ROWS-1)
 #define DEBUG_BIT(pos, val) \
  setpixel((pixel){(pos)%XSIZE,DEBUG_ROW+(pos)/XSIZE},(val)?3:0)
 #define DEBUG_BYTE(s,v) \
  DEBUG_BIT((s)*8+0, (v)&(1<<0)); \
  DEBUG_BIT((s)*8+1, (v)&(1<<1)); \
  DEBUG_BIT((s)*8+2, (v)&(1<<2)); \
  DEBUG_BIT((s)*8+3, (v)&(1<<3)); \
  DEBUG_BIT((s)*8+4, (v)&(1<<4)); \
  DEBUG_BIT((s)*8+5, (v)&(1<<5)); \
  DEBUG_BIT((s)*8+6, (v)&(1<<6)); \
  DEBUG_BIT((s)*8+7, (v)&(1<<7))   
#else
 #define DEBUG_BIT(s,v)
 #define DEBUG_BYTE(s,v)
#endif

//#define GLIDER_TEST

#define BITSTUFFED
#define LOOP_DETECT_BUFFER_SIZE 1

#ifndef GOL_DELAY
 #define GOL_DELAY 1 /* milliseconds */
#endif

#ifndef GOL_CYCLES
 #define GOL_CYCLES (2*60*3)
#endif

/******************************************************************************/
/******************************************************************************/

typedef enum{dead=0, alive=1} cell_t;

#ifndef BITSTUFFED

#define FIELD_XSIZE XSIZE
#define FIELD_YSIZE YSIZE

typedef cell_t field_t[FIELD_XSIZE][FIELD_YSIZE];

/******************************************************************************/

void setcell(field_t  pf, int x, int y, cell_t value){
	pf[(x+FIELD_XSIZE)%FIELD_XSIZE][(y+FIELD_YSIZE)%FIELD_YSIZE] = value;
}

/******************************************************************************/

cell_t getcell(field_t pf, int x, int y){
	return pf[(x+FIELD_XSIZE)%FIELD_XSIZE][(y+FIELD_YSIZE)%FIELD_YSIZE];
}

#else /* BITSTUFFED */

#define FIELD_XSIZE ((XSIZE+7)/8)
#define FIELD_YSIZE YSIZE

typedef uint8_t field_t[FIELD_XSIZE][FIELD_YSIZE];

struct {
	uint8_t isEditMode;
	uint8_t isMenuMode;
	uint8_t edit_cursor_x;
	uint8_t edit_cursor_y;
	
	uint16_t golDelay;
	uint8_t edit_blink;
	
	uint8_t autoGlider;
	uint8_t autoExit;
	uint8_t killGame;
	field_t pf;
	
	field_t ldbuf[LOOP_DETECT_BUFFER_SIZE]; // loop detect buffer
	uint8_t ldbuf_idx;
} golMenu;

field_t * Pf2;

/******************************************************************************/

void setcell(field_t pf, int x, int y, cell_t value){
	uint8_t t;
	x = (x+XSIZE) % XSIZE;
	y = (y+YSIZE) % YSIZE; 

	t = pf[x/8][y];
	if(value==alive){
		t |= 1<<(x&7);
	} else {
		t &= ~(1<<(x&7));
	}
	pf[x/8][y] = t;
}

/******************************************************************************/

cell_t getcell(field_t pf, int x, int y){
	x = (x+XSIZE) % XSIZE;
	y = (y+YSIZE) % YSIZE; 

	return ((pf[x/8][y])&(1<<(x&7)))?alive:dead;
}
#endif

/******************************************************************************/

uint8_t countsurroundingalive(field_t pf, int x, int y){
	uint8_t ret=0;
	ret += (getcell(pf, x-1, y-1)==alive)?1:0;
	ret += (getcell(pf, x  , y-1)==alive)?1:0;
	ret += (getcell(pf, x+1, y-1)==alive)?1:0;
	
	ret += (getcell(pf, x-1, y  )==alive)?1:0;
	ret += (getcell(pf, x+1, y  )==alive)?1:0;
	
	ret += (getcell(pf, x-1, y+1)==alive)?1:0;
	ret += (getcell(pf, x  , y+1)==alive)?1:0;
	ret += (getcell(pf, x+1, y+1)==alive)?1:0;
	return ret;
}

/******************************************************************************/

void nextiteration(field_t dest, field_t src){
	int x,y;
	uint8_t tc;
	for(y=0; y<YSIZE; ++y){
		for(x=0; x<XSIZE; ++x){
			tc=countsurroundingalive(src,x,y);
			switch(tc){
	//			case 0:
	//			case 1:
	//				/* dead */
	//				setcell(dest, x,y, dead);
				case 2:
					/* keep */
					setcell(dest, x,y, getcell(src,x,y));
					break;
				case 3:
					/* alive */
					setcell(dest, x,y, alive);
					break;
				default:
					/* dead */
					setcell(dest, x,y, dead);
					break;
			}
		}
	}
} 

/******************************************************************************/

void printpf(field_t pf){
	uint8_t x,y;
	for(y=0; y<YSIZE; ++y){
		for(x=0; x<XSIZE; ++x){
			setpixel((pixel){x,y},(getcell(pf,x,y)==alive)?3:0);
		}
	}
}

void printpf_with_cursor(field_t pf, uint8_t c_x, uint8_t c_y, uint8_t timer){
	int x,y;
	for(y=0; y<YSIZE; ++y){
		for(x=0; x<XSIZE; ++x){
			if (timer > 0 && c_x == x && c_y == y) {
				setpixel((pixel){x,y},(getcell(pf,x,y)==alive)?1:2);
			} else {
				setpixel((pixel){x,y},(getcell(pf,x,y)==alive)?3:0);
			}
		}
	}
}
/******************************************************************************/

void pfcopy(field_t dest, field_t src){
	int x,y;	
	for(y=0; y<YSIZE; ++y){
		for(x=0; x<XSIZE; ++x){
			setcell(dest,x,y,getcell(src,x,y));
		}
	}
}

/******************************************************************************/
#ifndef BITSTUFFED
uint8_t pfcmp(field_t dest, field_t src){
	int x,y;	
	for(y=0; y<YSIZE; ++y){
		for(x=0; x<XSIZE; ++x){
			if (getcell(src,x,y) != getcell(dest,x,y))
				return 1;
		}
	}
	return 0;
}

/******************************************************************************/

uint8 pfempty(field_t src){
	int x,y;	
	for(y=0; y<YSIZE; ++y){
		for(x=0; x<XSIZE; ++x){
			if (getcell(src,x,y)==alive)
				return 0;
		}
	}
	return 1;
}

void pf_clear(field_t dst){
	uint8_t x,y;
	for(y=0; y<YSIZE; ++y){
		for(x=0; x<XSIZE; ++x){
			setcell(src,x,y,alive);
		}
	}
}

#else

uint8_t pfcmp(field_t dest, field_t src){
	int x,y;
	for(y=0; y<FIELD_YSIZE; ++y){
		for(x=0; x<FIELD_XSIZE; ++x){
			if (src[x][y] != dest[x][y])
				return 1;
		}
	}
	return 0;
}

/******************************************************************************/

uint8_t pfempty(field_t src){	
	int x,y;	
	for(y=0; y<FIELD_YSIZE; ++y){
		for(x=0; x<FIELD_XSIZE; ++x){
			if (src[x][y]!=0)
				return 0;
		}
	}
	return 1;
}

void pf_clear(field_t dst){	
	int x,y;	
	for(y=0; y<FIELD_YSIZE; ++y){
		for(x=0; x<FIELD_XSIZE; ++x){
			dst[x][y]=0;
		}
	}
}


#endif
/******************************************************************************/

void insertglider(field_t pf){
	/*
	 *  #
	 *   #
	 * ###
	 */
	uint8_t r1, r2;
	r1 = random8() % XSIZE;
	r2 = random8() % YSIZE;
		
		                		setcell(pf, 1+r1, 0+r2, alive);
	                                                    				setcell(pf, 2+r1, 1+r2, alive);
	setcell(pf, 0+r1, 2+r2, alive); 	setcell(pf, 1+r1, 2+r2, alive); 	setcell(pf, 2+r1, 2+r2, alive);
}

/******************************************************************************/


void editMode(void) {
	uint8_t x,y, isAlreadyFired = 0;
	pixel lasttoggle = (pixel){XSIZE,YSIZE};

	uint16_t edit_timeout = 0;
	
	while (golMenu.isEditMode > 0 && edit_timeout++ < 5000) {
		if (JOYISUP) {
			edit_timeout = 0;
			lasttoggle = (pixel){XSIZE,YSIZE};
			isAlreadyFired = 0;
			golMenu.edit_cursor_y--;
		} else if (JOYISDOWN) {
			edit_timeout = 0;
			lasttoggle = (pixel){XSIZE,YSIZE};
			isAlreadyFired = 0;
			golMenu.edit_cursor_y++;
		} else if (JOYISLEFT) {
			edit_timeout = 0;
			lasttoggle = (pixel){XSIZE,YSIZE};
			isAlreadyFired = 0;
			golMenu.edit_cursor_x++;
		} else if (JOYISRIGHT) {
			edit_timeout = 0;
			lasttoggle = (pixel){XSIZE,YSIZE};
			isAlreadyFired = 0;
			golMenu.edit_cursor_x--;
		}
		golMenu.edit_cursor_x = golMenu.edit_cursor_x % XSIZE;
		golMenu.edit_cursor_y = golMenu.edit_cursor_y % YSIZE;
		
		if (JOYISFIRE) {
			if (isAlreadyFired == 0) {
				isAlreadyFired = 1;
				setcell(golMenu.pf, golMenu.edit_cursor_x, golMenu.edit_cursor_y,(getcell(golMenu.pf,golMenu.edit_cursor_x,golMenu.edit_cursor_y)==alive)?dead:alive);
				if (edit_timeout < 10 && lasttoggle.x == golMenu.edit_cursor_x && lasttoggle.y == golMenu.edit_cursor_y) {
					golMenu.isEditMode = 0;
					scrolltext("<10/#continue");
					printpf(golMenu.pf);
					wait(300);
					if (JOYISFIRE) {
						scrolltext("</# clear ");
						for(y=0;y<YSIZE;++y){
							for(x=0;x<XSIZE; ++x){
								setcell(golMenu.pf,x,y,dead);
							}
						}
					}
					break;
				}
				lasttoggle = (pixel){golMenu.edit_cursor_x,golMenu.edit_cursor_y};
				edit_timeout = 0;
			}
		} else {
			isAlreadyFired = 0;
		}
		printpf_with_cursor(golMenu.pf, golMenu.edit_cursor_x, golMenu.edit_cursor_y, golMenu.edit_blink++);
		wait(100);
		if (golMenu.edit_blink < 4) golMenu.edit_blink = golMenu.edit_blink % 2;
	}
	golMenu.isEditMode = 0;
	return;
}

uint8_t areYouSure() {
	uint8_t timeout;
	while (1) {
		timeout = 0;
		scrolltext("<14/# Are you sure? Fire!");
		setpixel((pixel){6,2},3);
		setpixel((pixel){7,2},3);
		setpixel((pixel){8,2},3);
		setpixel((pixel){5,3},3);
		setpixel((pixel){9,3},3);
		setpixel((pixel){5,4},3);
		setpixel((pixel){5,5},3);
		setpixel((pixel){6,6},3);
		setpixel((pixel){7,6},3);
		setpixel((pixel){8,6},3);
		setpixel((pixel){9,7},3);
		setpixel((pixel){9,8},3);
		setpixel((pixel){5,9},3);
		setpixel((pixel){9,9},3);
		setpixel((pixel){6,10},3);
		setpixel((pixel){7,10},3);
		setpixel((pixel){8,10},3);
		setpixel((pixel){7,12},3);
		while (timeout++ < 100) {
			if (JOYISUP || JOYISLEFT || JOYISRIGHT || JOYISDOWN) {
				scrolltext("<14/# cancel  ");
				return 0;
			} else if (JOYISFIRE) {
				while (JOYISFIRE) {
					wait(20); //to prevent unwanted press of fire after exit
				}
				return 1;
			}
			wait(50);
		}
	}
}

void menuModeUndo(void) {
	if (golMenu.ldbuf_idx == 0) {
		golMenu.ldbuf_idx = LOOP_DETECT_BUFFER_SIZE-1;
	} else {
		golMenu.ldbuf_idx = (golMenu.ldbuf_idx-1)%LOOP_DETECT_BUFFER_SIZE;
	}
	pfcopy(golMenu.pf, golMenu.ldbuf[golMenu.ldbuf_idx]);
	printpf(golMenu.pf);
	wait(150);
}
void menuModeRedo(void) {
	golMenu.ldbuf_idx = (golMenu.ldbuf_idx+1)%LOOP_DETECT_BUFFER_SIZE;
	pfcopy(golMenu.pf, golMenu.ldbuf[golMenu.ldbuf_idx]);
	printpf(golMenu.pf);
	wait(150);
}

uint8_t menuModeSetScreen(void) {
	//works atm only for 16x16-Borgs! (TODO: IFDEF-foo)
	int x,y;
	uint8_t edit_timeout = 0;
	uint8_t selected = 0;
	uint8_t entries_cnt = 3;
	uint8_t last_selected = 0xff;
	uint16_t cfg[FIELD_YSIZE];

	while (JOYISFIRE) {
		wait(20);
	}
	while (edit_timeout++ < 250) {
		if (last_selected != selected) {
			last_selected = selected;
			pf_clear(Pf2);
			for (y=0; y<FIELD_YSIZE; ++y){
				cfg[y] = 0x0000;
			}
			switch (selected) {
				case 0:
					cfg[0] = 0xffff;
					cfg[2] = 0xffff;
					cfg[3] = 0xffff;
					cfg[4] = 0xffff;
				break;
				case 1:
					cfg[0] = 0xffff;
					cfg[4] = 0xffff;
					cfg[6] = 0xffff;
					cfg[7] = 0xffff;
				break;
				case 2:
					cfg[0] = 0xffff;
					cfg[1] = 0xffff;
					cfg[2] = 0x2342;
					cfg[3] = 0x00ff;
				break;
			}	
			for(y=0; y<YSIZE; ++y){
				uint16_t msk = 1;
				for(x=0; x<XSIZE; ++x){
					if (cfg[y] & msk) {
						setcell(Pf2, x, y, alive);
					}
					msk <<= 1;
				}
			}
			printpf(Pf2);
		}
		wait(50);
		if (JOYISLEFT) {
			if (selected == 0) {
				selected = entries_cnt-1;
			} else {
				selected--;
			}
			edit_timeout = 0;
		} else if (JOYISRIGHT) {
			selected++;
			selected = selected % entries_cnt;
			edit_timeout = 0;
		} else if (JOYISUP || JOYISDOWN) {
			return 0;
		} else if (JOYISFIRE) {
			pfcopy(golMenu.pf, Pf2);
			scrolltext("</# New Screen");
			golMenu.isMenuMode = 0;
			return 2;
		}
	}
	golMenu.isMenuMode = 0;
	return 2;
}
uint8_t menuModeHandleFire(uint8_t selected, uint8_t edit_timeout) {
	uint8_t x, y;
	switch (selected) {
		case 0:
			//AutoGlider on / off
			golMenu.autoGlider = (golMenu.autoGlider + 1) % 2;
			return 0;
		break;
		case 1:
			//AutoExit on / off
			golMenu.autoExit = (golMenu.autoExit + 1) % 2;
			return 0;
		break;
		case 2:
			//Clear Screen
			if (areYouSure() == 0) {
				return 0;
				break;
			}
			for(y=0;y<YSIZE;++y){
				for(x=0;x<XSIZE; ++x){
					setcell(golMenu.pf,x,y,dead);
				}
			}
			scrolltext("</# Clean Screen");
			golMenu.isMenuMode = 0;
			return 2;
		break;
		case 3:
			//Set Screen
			return menuModeSetScreen();
		break;
		case 4:
			//Random Screen
			if (areYouSure() == 0) {
				return 0;
				break;
			}
			for(y=0;y<YSIZE;++y){
				for(x=0;x<XSIZE; ++x){
					setcell(golMenu.pf,x,y,(random8()&1)?alive:dead);
				}
			}
			scrolltext("</# Random Screen");
			golMenu.isMenuMode = 0;
			return 2;
		break;
		case 5:
			//Insert Items
			// todo ...
			return 0;
		break;
		case 6:
			//Edit Mode
			golMenu.isEditMode = 0;
			golMenu.isMenuMode = 0;
			return 2;
		break;
		case 7:
			//Delay (no action needed here)
			return 0;
		break;
		case 8:
			//Undo
			//menuModeUndo();
			//return 1;
			//Menü sofort verlassen
			golMenu.isMenuMode = 0;
			return 2;
		break;
		case 9:
			//foobar
			// todo ...
			return 0;
		break;
		case 10:
			//GlobusMode on / off
			// todo ...
			return 0;
		break;
		case 11:
			//WorldDef
			// todo ...
			return 0;
		break;
		case 12:
			//Exit Menu
			golMenu.isMenuMode = 0;
			return 2;
		break;
		case 13:
			//Exit Game of Life
			if (areYouSure() == 0) break;
			golMenu.killGame = 1;
			golMenu.isMenuMode = 0;
			return 2;
		break;
		default:
		break;
	}
	return edit_timeout;
}
uint8_t menuModeHandleLeft(uint8_t selected, uint8_t edit_timeout) {
	switch (selected) {
		case 0:
			golMenu.autoGlider = 0;
			return 0;
		break;
		case 1:
			golMenu.autoExit = 0;
			return 0;
		break;
		case 7:
			golMenu.golDelay--;
			if (golMenu.golDelay < 10) golMenu.golDelay = 10;
			return 0;
		break;
		case 8:
			menuModeUndo();
			return 1;
		break;
		default:
		break;
	}
	return edit_timeout;
}
uint8_t menuModeHandleRight(uint8_t selected, uint8_t edit_timeout) {
	switch (selected) {
		case 0:
			golMenu.autoGlider = 1;
			return 0;
		break;
		case 1:
			golMenu.autoExit = 1;
			return 0;
		break;
		case 7:
			golMenu.golDelay++;
			if (golMenu.golDelay > 60000) golMenu.golDelay = 60000;
			return 0;
		break;
		case 8:
			menuModeRedo();
			return 1;
		break;
		default:
		break;
	}
	return edit_timeout;
}

void menuMode(void) {
	char a[23];
	char b[42];
	uint8_t selected = 0;
	uint8_t entries_cnt = 14;
	uint8_t edit_timeout = 0;
	
	scrolltext("<9/#  Menu:  ");
	
	while (golMenu.isMenuMode > 0 && edit_timeout < 100) {
		wait(50);
		if (JOYISUP) {
			if (selected == 0) {
				selected = entries_cnt-1;
			} else {
				selected--;
			}
			edit_timeout = 0;
		} else if (JOYISDOWN) {
			selected++;
			selected = selected % entries_cnt;
			edit_timeout = 0;
		} else if (JOYISLEFT) {
			edit_timeout = menuModeHandleLeft(selected, edit_timeout);
		} else if (JOYISRIGHT) {
			edit_timeout = menuModeHandleRight(selected, edit_timeout);
		} else if (JOYISFIRE) {
			edit_timeout = menuModeHandleFire(selected, edit_timeout);
		}
		
		if (edit_timeout % 70 == 0) {
			switch (selected) {
				case 0:
					sprintf(a, "AutoGlider %u", golMenu.autoGlider);
				break;
				case 1:
					sprintf(a, "AutoExit %u", golMenu.autoExit);
				break;
				case 2:
					sprintf(a, "Clear Screen");
				break;
				case 3:
					sprintf(a, "Set Screen");
				break;
				case 4:
					sprintf(a, "Set Random");
				break;
				case 5:
					sprintf(a, "Insert Items (not yet)");
				break;
				case 6:
					sprintf(a, "Edit Mode");
				break;
				case 7:
					sprintf(a, "Delay %u", golMenu.golDelay);
				break;
				case 8:
					sprintf(a, "Undo");
				break;
				case 9:
					sprintf(a, "foobar");
				break;
				case 10:
					sprintf(a, "GlobusMode 1 (not yet)");
				break;
				case 11:
					sprintf(a, "WorldDef 23/3 (not yet)");
				break;
				case 12:
					sprintf(a, "Exit Menu");
				break;
				case 13:
					sprintf(a, "Exit Game");
				break;
				default:
					sprintf(a, "bug");
				break;
			}
			sprintf(b, "<8/# %s ", a);
			scrolltext(b);
		}
		edit_timeout++;
	}
	golMenu.isMenuMode = 0;
	return;
}

void waitForNextItteration(void) {
	char a[42];
	uint16_t i;
	for (i = 0; i < golMenu.golDelay; i++) {
		wait(10);
		if (JOYISUP) {
			golMenu.isEditMode = 1;
		} else if (JOYISDOWN) {
			golMenu.isMenuMode = 1;
		} else if (JOYISRIGHT) {
			golMenu.golDelay--;
			if (golMenu.golDelay < 10) golMenu.golDelay = 10;
			if (golMenu.golDelay % 500 == 0) {
				sprintf(a,">8/#delay %u", golMenu.golDelay);
				scrolltext(a);
				printpf(golMenu.pf);
			}
		} else if (JOYISLEFT) {
			golMenu.golDelay++;
			if (golMenu.golDelay > 60000) golMenu.golDelay = 60000;
			if (golMenu.golDelay % 500 == 0) {
				sprintf(a,">8/#delay %u", golMenu.golDelay);
				scrolltext(a);
				printpf(golMenu.pf);
			}
		} else if (JOYISFIRE) {
			scrolltext("</#Exit Game?");
			if (areYouSure() > 0) {
				golMenu.killGame = 1;
				return;
			}
		}
		
		if (golMenu.isMenuMode > 0) {
			menuMode();
		}
		if (golMenu.isEditMode > 0) {
			editMode();
		}
		
	}
	return;
}

int gameoflife(){
	field_t pf2;
	int x,y;
	uint8_t i;
	uint16_t cycle;
	golMenu.isEditMode    = 0;
	golMenu.isMenuMode    = 0;
	golMenu.edit_cursor_x = XSIZE / 2;
	golMenu.edit_cursor_y = YSIZE / 2;
	golMenu.golDelay      = 2;
	golMenu.edit_blink    = 230;
	golMenu.autoGlider    = 0;
	golMenu.autoExit      = 0;
	golMenu.killGame      = 0;
	golMenu.ldbuf_idx=0;

	Pf2 = &pf2; //for use of pf2 from othr funtions

//start:	
	/* initalise the field with random */
	for(y=0;y<YSIZE;++y){
		for(x=0;x<XSIZE; ++x){
			setcell(golMenu.pf,x,y,(random8()&1)?alive:dead);
		}
	}
#ifdef GLIDER_TEST	
	/* initialise with glider */
	for(y=0;y<YSIZE;++y){
		for(x=0;x<XSIZE; ++x){
			setcell(golMenu.pf,x,y,dead);
		}
	}
	insertglider(golMenu.pf);	 
#endif	
	
	waitForFire = 0; // NICHT den Borg in der wait()-fkt "neustarten" wenn feuer gedrückt wird

	/* the main part */
	printpf(golMenu.pf);
	while(golMenu.killGame == 0){
		waitForNextItteration();
		pfcopy(pf2,golMenu.pf);
		nextiteration(golMenu.pf,pf2);
		printpf(golMenu.pf);
		/* loop detection */
		if(golMenu.autoGlider > 0 && !pfcmp(golMenu.pf, pf2)){
			insertglider(golMenu.pf);
			cycle=1;
		}
		if((golMenu.killGame > 0) || (golMenu.autoExit > 0 && pfempty(golMenu.pf))){
			/* kill game */
			waitForFire = 1;
			return 0;
		}
		/* auto glider */
		if (golMenu.autoGlider > 0) {
			for(i=0; i<LOOP_DETECT_BUFFER_SIZE; ++i) {
				if(!pfcmp(golMenu.pf, golMenu.ldbuf[i])) {
					insertglider(golMenu.pf);
					cycle=1;
				}
			}
		}
		pfcopy(golMenu.ldbuf[golMenu.ldbuf_idx], golMenu.pf);
		golMenu.ldbuf_idx = (golMenu.ldbuf_idx+1)%LOOP_DETECT_BUFFER_SIZE;
	}
	
	waitForFire = 1;
	return 0;
}

