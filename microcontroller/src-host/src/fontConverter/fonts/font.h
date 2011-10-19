#ifdef AVR
#   include <avr/pgmspace.h>
#else 
#   define PROGMEM
#   define pgm_read_byte(B) *(B)      
#endif

typedef struct{
	unsigned char fontHeight;
	unsigned int* fontIndex;
	unsigned char* fontData;
	char glyph_beg;
	char glyph_end;
	char glyph_def;
	unsigned char storebytes;
} font;
