/**
 * \author	Daniel Otte
 * \date	2008-08-24
 * \license GPLv3 or later
 * \brief   random number generator based on noekeon running in CFB-mode
 * 
 */

#ifndef PRNG_H_
#define PRNG_H_

#include <stdint.h>
#include "string.h"
#include "noekeon.h"
#include "memxor.h"

extern uint8_t random_state[16];
extern uint8_t random_key[16];

uint8_t random8(void);

inline static void random_block(void* dest){
	noekeon_enc(random_state, random_key);
	memcpy(dest, random_state, 16);
}

inline static void srandom32(uint32_t seed){
	memcpy(random_key, &seed, 4);
}

inline static void random_seed(const void* buffer){
	memcpy(random_key, buffer, 16);
}

inline static void random_add(const void* buffer){
	memxor(random_key, buffer, 16);
}

#endif /* PRNG_H_*/


