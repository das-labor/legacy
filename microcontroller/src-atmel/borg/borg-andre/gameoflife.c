/**
 * Conways Game of life 
 * Author: Daniel Otte
 * License: GPLv3
 * 
 * 
 */

#include <stdint.h>
#include <util/delay.h>
#include <avr/sfr_defs.h> /* for debugging */
#include "config.h"
#include "prng.h"
#include "pixel.h"
#include "util.h"

/******************************************************************************/ 

#define DEBUG
 
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
//#define BITSTUFFED

#ifndef GOL_DELAY
 #define GOL_DELAY 1 /* milliseconds */
#endif

#ifndef GOL_CYCLES
 #define GOL_CYCLES (2*60*3)
#endif

/******************************************************************************/

typedef enum{dead=0, alive=1} cell_t;

#ifndef BITSTUFFED

#define FIELD_XSIZE XSIZE
#define FIELD_YSIZE YSIZE

typedef cell_t field_t[FIELD_XSIZE][FIELD_YSIZE];

void setcell(field_t  pf, int x, int y, cell_t value){
	pf[(x+FIELD_XSIZE)%FIELD_XSIZE][(y+FIELD_YSIZE)%FIELD_YSIZE] = value;
}

cell_t getcell(field_t pf, int x, int y){
	return pf[(x+FIELD_XSIZE)%FIELD_XSIZE][(y+FIELD_YSIZE)%FIELD_YSIZE];
}

#else /* BITSTUFFED */

#define FIELD_XSIZE ((XSIZE+7)/8)
#define FIELD_YSIZE YSIZE

typedef uint8_t field_t[FIELD_XSIZE][FIELD_YSIZE];


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

cell_t getcell(field_t pf, int x, int y){
	x = (x+XSIZE) % XSIZE;
	y = (y+YSIZE) % YSIZE; 

	return ((pf[x/8][y])&(1<<(x&7)))?alive:dead;
}
#endif



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


void printpf(field_t pf){
	int x,y;
	for(y=0; y<YSIZE; ++y){
		for(x=0; x<XSIZE; ++x){
			setpixel((pixel){x,y},(getcell(pf,x,y)==alive)?3:0);
		}
	}
}

void pfcopy(field_t dest, field_t src){
	int x,y;	
	for(y=0; y<YSIZE; ++y){
		for(x=0; x<XSIZE; ++x){
			setcell(dest,x,y,getcell(src,x,y));
		}
	}
}

int gameoflife(){
	DEBUG_BYTE(0,0); // set debug bytes to zero
	DEBUG_BYTE(1,0);
	field_t pf1={{0}},pf2={{0}};
	int x,y;
	uint16_t cycle;
	
	/* initalise the field with random */
	for(y=0;y<YSIZE;++y){
		for(x=0;x<XSIZE; ++x){
			setcell(pf1,x,y,(random8()&1)?alive:dead);
		}
	}
#ifdef GLIDER_TEST	
	/* initialise with glider */
	for(y=0;y<YSIZE;++y){
		for(x=0;x<XSIZE; ++x){
			setcell(pf1,x,y,dead);
		}
	}
	/*
	 *  #
	 *   #
	 * ###
	 */
		setcell(pf1, 1, 0, alive);
			setcell(pf1, 2, 1, alive);
	setcell(pf1, 0, 2, alive); setcell(pf1, 1, 2, alive);setcell(pf1, 2, 2, alive);
	 
#endif	
	printpf(pf1);
	for(cycle=1; cycle<GOL_CYCLES; ++cycle){
		DEBUG_BYTE(0,(uint8_t)cycle&0xff);
		DEBUG_BYTE(1, 0);
		wait(GOL_DELAY);
		pfcopy(pf2,pf1);
		nextiteration(pf1,pf2);
		printpf(pf1);
	}
	
	return 0;
}

