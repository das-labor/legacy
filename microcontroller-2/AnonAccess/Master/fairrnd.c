/**
 * 
 * 
 * 
 * 
 * 
 * 
 **/
#include <stdint.h>
#include "prng.h" 
#include "fairrnd.h"

uint32_t fairrnd(uint32_t m){
	uint64_t a;
	do {
		fillBlockRandom(&a, 8);
	} while (0xFFFFFFFFFFFFFFFFLL - a <= 0xFFFFFFFFFFFFFFFFLL % m);
	return a%m;
}

