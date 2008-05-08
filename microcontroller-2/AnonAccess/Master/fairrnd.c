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
	uint64_t a;
	do {
		entropium_fillBlockRandom(&a, 8);
	} while (0xFFFFFFFFFFFFFFFFLL - a <= 0xFFFFFFFFFFFFFFFFLL % m);
	return a%m;
}

