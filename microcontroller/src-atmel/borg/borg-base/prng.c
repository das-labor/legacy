/**
 * \author	Daniel Otte
 * \date	2008-08-24
 * \license GPLv3 or later
 * \brief   random number generator based on noekeon running in CFB-mode
 * 
 */

#include "noekeon.h"
#include "memxor.h"
#include <stdint.h>
#include <string.h>

uint8_t random_state[16];
uint8_t random_key[16];


uint8_t random8(void){
	static uint8_t sr[16];
	static uint8_t i=0;	
	
	if(i==0){
		noekeon_enc(random_state, random_key);
		memcpy(sr, random_state, 16);
		i=15;
		return sr[15];
	}
	--i;
	return sr[i];
}

void random_block(void* dest){
	noekeon_enc(random_state, random_key);
	memcpy(dest, random_state, 16);
}

void srandom32(uint32_t seed){
	memcpy(random_key, &seed, 4);
}

void random_seed(const void* buffer){
	memcpy(random_key, buffer, 16);
}

void random_add(const void* buffer){
	memxor(random_key, buffer, 16);
}


