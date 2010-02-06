#ifndef FONT_H
#define FONT_H

#include <avr/pgmspace.h>

typedef struct{
	unsigned char fontHeight;
	const unsigned int* fontIndex;
	const unsigned char* fontData;
	unsigned char glyph_beg;
	unsigned char glyph_end;
	unsigned char glyph_def;
	unsigned char storebytes;
} font;

#endif //FONT_H
