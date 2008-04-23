/*
 * 
 * author: Daniel Otte
 * email:  daniel.otte@rub.de
 * license: GPLv3
 * 
 */
 

#include <stdint.h>
#include <avr/eeprom.h>

#ifndef PERCNT_NO
 #define PERCNT_NO 1
#endif

#ifndef PERCNT_BPC /* bytes per counter */
 #define PERCNT_BPC 16
#endif

#define PERCNT_MBS ((PERCNT_BPC+7)/8) /* mask block size */

#ifndef PERCNT_CS
 #define PERCNT_CS 4 /* counter size (max 4)*/
#endif

uint8_t percnt_count[PERCNT_NO][3][PERCNT_BPC] EEMEM;

uint8_t percnt_mb[PERCNT_NO][3][PERCNT_MBS] EEMEM;

typedef enum {counterA=0, counterB=1, counterC=2} percnt_active_t;

percnt_active_t percnt_active[PERCNT_NO];

#ifdef PERCNT_CACHE
uint32_t percnt_cache[PERCNT_NO];
#endif

/**
 * this function resets the counter given in the parameter back to zero
 *  this is quite unsafe
 */
void percnt_reset(uint8_t counter){
	uint8_t i;
	for(i=0; i<PERCNT_BPC; ++i){
		eeprom_write_byte(&(percnt_count[counter][counterA][i]), 0);
	}
	for(i=0; i<PERCNT_BPC; ++i){
		eeprom_write_byte(&(percnt_count[counter][counterB][i]), 0);
	}
	for(i=0; i<PERCNT_BPC; ++i){
		eeprom_write_byte(&(percnt_count[counter][counterC][i]), 0);
	}
	for(i=0; i<PERCNT_MBS; ++i){
		eeprom_write_byte(&(percnt_mb[counter][counterA][i]), 0);
	}
	for(i=0; i<PERCNT_MBS; ++i){
		eeprom_write_byte(&(percnt_mb[counter][counterB][i]), 0);
	}
	for(i=0; i<PERCNT_MBS; ++i){
		eeprom_write_byte(&(percnt_mb[counter][counterC][i]), 0);
	}
}

static
uint8_t bitcnt(const void* a, uint8_t length){
	uint8_t lut[16]= { 0, 1, 1, 2,  /* 0..3 */
		               1, 2, 2, 3,  /* 4..7 */
		               1, 2, 2, 3,  /* 8..B */
		               2, 3, 3, 4}; /* C..F */
	uint8_t ret=0;
	while(length--){
		ret += lut[(*((uint8_t*)a))>>4];
		ret += lut[(*((uint8_t*)a))&0x0f];
	}
	return ret;
}

/**
 * returns 0 if everything went rigth, 1 or even 2 in the case of failure
 */
static
uint8_t percnt_writecntx(uint32_t v, uint8_t counter, percnt_active_t subc){
	uint8_t maskb[PERCNT_MBS];
	uint8_t free;
	uint8_t i, j=0, t;
	eeprom_read_block(maskb, &(percnt_mb[counter][subc][0]), PERCNT_MBS);
	free = PERCNT_BPC - bitcnt(maskb, PERCNT_MBS);
	
	for(i=0; i<PERCNT_CS; ++i){
		if(free < PERCNT_CS){
			/* we don't have enough bytes left, so we can't count anymore :-( */
			return 1;
		}
		/* get next useable byte */
		while((maskb[j/8]>>(j%8))&1)
			++j;
		t=eeprom_read_byte(&(percnt_count[counter][subc][j]));
		if(t!=((uint8_t*)&v)[i]){
			/* we have to do a write */
			eeprom_write_byte(&(percnt_count[counter][subc][j]),((uint8_t*)&v)[i]);
			/* and check back */
			t=eeprom_read_byte(&(percnt_count[counter][subc][j]));
			if(t!=((uint8_t*)&v)[i]){
				/* thats not good, readback failed */
				/* mark byte as bad */
				maskb[j/8] |= 1<<(j%8);
				eeprom_write_byte(&(percnt_mb[counter][subc][j/8]), maskb[j/8]);
				/* check back the maskblock write*/
				t = eeprom_read_byte(&(percnt_mb[counter][subc][j/8]));
				if(t!=maskb[j/8]){
					/* uargghh! that should not happen, our bad byte table has a bad byte*/
					return 2;
				}
				--i; /* this makes this byte gets another try */
			}	
		}
		++j;
	}
	return 0;
}

/******************************************************************************/

static
uint32_t percnt_readcntx(uint8_t counter, percnt_active_t subc){
	uint32_t ret=0;
	uint8_t maskb[PERCNT_MBS];
	uint8_t i, j=0;
	eeprom_read_block(maskb, &(percnt_mb[counter][subc][0]), PERCNT_MBS);
	for(i=0; i<PERCNT_CS; ++i){
		while(((maskb[j/8])>>(j%8))&1)
			++j;
		((uint8_t*)&ret)[i]=eeprom_read_byte(&(percnt_count[counter][subc][j]));
	}
	return ret;
}

/******************************************************************************/

uint32_t percnt_get(uint8_t counter){
	return percnt_readcntx(counter, percnt_active[counter]);
}

/******************************************************************************/

uint8_t percnt_inc(uint8_t counter){
	uint32_t t;
	t = percnt_get(counter);
	percnt_active[counter] += 1;
	if (percnt_active[counter] == 3)
		percnt_active[counter] = 0;
	return percnt_writecntx(t+1, counter, percnt_active[counter]);
}








