/**
 * \author	Daniel Otte
 * \date	2007-02-10
 * \par \License GPL
 * \brief unsecure low weigth pseudo random number generator
 * \description implementation of a simple LFSR
 * 
 */

#ifndef PRNG_H_
#define PRNG_H_

#include <stdint.h>

uint8_t random8(void);
void srandom(uint32_t seed);

#endif /* PRNG_H_*/
