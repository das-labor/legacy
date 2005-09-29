#ifdef AVR
#   include <avr/pgmspace.h>
#else 
#   define PROGMEM
#   define pgm_read_byte(B) *(B)      
#   define pgm_read_word(B) *(B)  
#endif

typedef struct{
	unsigned char fontHeight;
	unsigned int* fontIndex;
	unsigned char* fontData;
	unsigned char glyph_beg;
	unsigned char glyph_end;
	unsigned char glyph_def;
	unsigned char storebytes;
} font;
