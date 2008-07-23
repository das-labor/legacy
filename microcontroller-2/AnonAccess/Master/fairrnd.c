/**
 * 
 * 
 * 
 * 
 * 
 * 
 **/
#include <stdint.h>
#include "entropium.h" 
#include "fairrnd.h"

uint32_t fairrnd(uint32_t m){
	uint64_t limit;
	limit=((uint64_t)(0xFFFFFFFFFFFFFFFFLL / (uint64_t)m))*(uint64_t)m;
	uint64_t a;
	do {
		entropium_fillBlockRandom(&a, 8);
	} while (a>=limit);
	return a%m;
}

