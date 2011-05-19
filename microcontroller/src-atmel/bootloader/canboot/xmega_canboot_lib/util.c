#include <avr/pgmspace.h>
//#include <avr/boot.h>

#include "util.h"


#ifdef pgm_read_byte_far
	void my_memcpy_P (void * dest, PGM_VOID_P source, unsigned char size){
		uint8_t * d = dest;
		while(size--){
			*d++ = pgm_read_byte_far(((FLASHEND + 1ul) & 0xff0000ul ) | (uint16_t)(source++));
		}
	
	}
#else



	void my_memcpy_P (void * dest, PGM_VOID_P source, unsigned char size){
			asm volatile(
			"\nmy_memcpy_lp:\n\t"
			"lpm r25,Z+\n\t"
			"st X+,r25\n\t"
			"dec %0\n\t"
			"brne my_memcpy_lp\n\t"
			:: "r"(size), "x"(dest), "z"(source)
			:"r25"
			);
	}

#endif