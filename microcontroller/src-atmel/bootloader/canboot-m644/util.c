#include <avr/pgmspace.h> 

void my_memcpy_P (unsigned char size, void * dest, PGM_VOID_P source){
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
