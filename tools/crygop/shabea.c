/* shabea.c */
/*
    This sourcefile is part of the crygop programm.
    Copyright (C) 2008  Daniel Otte

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/**
 * \file	shabea.c
 * \author	Daniel Otte 
 * \date	2007-06-07
 * \brief	SHABEA - a SHA Based Encryption Algorithm implementation
 * \par License	
 * GPL
 * 
 * SHABEAn-r where n is the blocksize and r the number of round used
 * 
 * 
 */
#include <stdlib.h>
#include <string.h>
#include "sha256.h"
#include "shabea.h"
#include "bitfunctions.h"
/*
 * 
 */
static
void memxor(uint8_t * dest, uint8_t * src, uint8_t length){
	while(length--){
		*dest++ ^= *src++;
	}
} 

/*
 * SHABEAn
 */ 
 
void shabean(void * block, uint16_t blocksize_b, void * key, uint16_t keysize_b, uint8_t enc, uint8_t rounds){
	int8_t r;		/**/
	uint16_t halfblocksize_B = (blocksize_b/2+7)/8;
	uint8_t tb[halfblocksize_B+2+(keysize_b+7)/8];	/**/
	uint16_t kbs;	/* bytes used for the key / temporary block */
	uint8_t left[halfblocksize_B], right[halfblocksize_B];
	uint8_t mask[halfblocksize_B];
	sha256_hash_t hash;
	
	memset(left,  0, halfblocksize_B);
	memset(right, 0, halfblocksize_B);
	memset(mask,  0, halfblocksize_B);

	if(halfblocksize_B>1)	
		memset(mask,  0xFF, halfblocksize_B-1);
	if((blocksize_b/2)%8>0)	
		mask[halfblocksize_B-1]=(1<<((blocksize_b/2)%8))-1;
	r = (enc?0:(rounds-1));
	kbs = (keysize_b+7)/8;
	memcpy(tb+halfblocksize_B+2, key, kbs); /* copy key to temporary block */
	tb[halfblocksize_B+0] = 0;	/* set round counter high value to zero */
	
	membitsplit(left, right, block, blocksize_b/2);
	if((blocksize_b/2)%8){	
		mask[halfblocksize_B-1] = (1<<((blocksize_b/2)%8))-1;
	}
	memand(right, mask, halfblocksize_B);
	memand(left,  mask, halfblocksize_B);

	for(;r!=(enc?(rounds):-1);enc?r++:r--){ /* enc: 0..(rounds-1) ; !enc: (rounds-1)..0 */
		memcpy(tb, right, halfblocksize_B); /* copy right half into tb */
		tb[halfblocksize_B+1] = r;
		sha256(&hash, tb, halfblocksize_B*8+16+keysize_b);
		memand(hash, mask, halfblocksize_B);
		if(!(r==(enc?(rounds-1):0))){	
			/* swap */
			memxor(hash,  left, halfblocksize_B);
			memcpy(left, right, halfblocksize_B);
			memcpy(right, hash, halfblocksize_B);
		} else {
			/* no swap */
			memxor(left, hash, halfblocksize_B);	
		}
	}
	memset(block, 0, (blocksize_b+7)/8);
	membitmerge(block, left, right, blocksize_b/2);
}


