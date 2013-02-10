#ifndef FONT_H
#define FONT_H

#ifdef AVR
	#include <avr/pgmspace.h>
#else
	#define PROGMEM
	#define pgm_read_word(a) ((uint16_t)*(a))
	#define pgm_read_byte(a) ((uint8_t)*(a))
#endif

typedef struct {
	unsigned char fontHeight;
	const unsigned int* fontIndex;
	const unsigned char* fontData;
	unsigned char glyph_beg;
	unsigned char glyph_end;
	unsigned char glyph_def;
	unsigned char storebytes;
} font;

#endif //FONT_H
