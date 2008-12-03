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

uint8_t random8(void);
void random_block(void* dest);
void srandom32(uint32_t seed);
void random_seed(const void* buffer);
void random_add(const void* buffer);

#endif /* PRNG_H_*/


