/**
 * \author	Daniel Otte
 * \date	2007-02-10
 * \par \License GPL
 * \brief unsecure low weigth pseudo random number generator
 * \description implementation of a simple LFSR
 * 
 */

#include <stdint.h>

#ifdef AVR
	#include <util/parity.h>
#else
	uint8_t nibble_parity_table[]={
		0,1,1,0,	/* 0,1,2,3 */
		1,0,0,1,	/* 4,5,6,7 */
		1,0,0,1,	/* 8,9,A,B */
		0,1,1,0 	/* C,D,E,F */
	};
	
	uint8_t parity_even_bit(uint8_t p){
		return nibble_parity_table[p>>4]^nibble_parity_table[4&0xf];
	}
#endif

/* CRC32 polynom x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x + 1 */
/* 1000.100_1100.0001_0001.1101_1011.0111 */
//#define POLYNOM 0x/* we use the CRC32 polynom in hope it is irreducible */
#define POLYNOM 0x80802301	/* 1000.000_1000.0000_0010.0011_0000.0001 */
uint32_t state = 0x0BADC0DE;

uint8_t random8(void){
	uint8_t i,t[4];
	for(i=0;i<8;++i){
		*((uint32_t*)t)=state&POLYNOM;
		state = state<<1 | (parity_even_bit(t[0]) ^ parity_even_bit(t[1]) ^ 
							parity_even_bit(t[2]) ^ parity_even_bit(t[3]));
	}
	
	return (uint8_t)state;
}

void srandom(uint32_t seed){
	if(seed)
		state = seed;
}

