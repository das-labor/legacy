//pc compatibility of AVR-pgmspace stuff for compiling simulator

#ifdef AVR
	#include <avr/pgmspace.h>
#else
	#define PROGMEM
	#define strcpy_P strcpy
	#define memcpy_P memcpy
	#define pgm_read_byte(a) ((*(uint8_t*)(a)))
	#define pgm_read_word(a) ((*(uint16_t*)(a)))
#endif
