#ifdef AVR
#   include <avr/pgmspace.h>
#else 
#   define PROGMEM
#   define pgm_read_byte(B) *(B)      
#   define pgm_read_word(B) *(B)      
#endif

#define SCROLLTEXT_C

#include "config.h"
#include "scrolltext.h"
#ifdef AVR
#   include "borg_hw.h"
#endif

#include "pixel.h"
#include "util.h"
#include "fonts.h"

#define min(a,b) ((a<b)? a : b)

uint8_t get_next_bits(char *str, uint8_t offset, uint8_t y, uint8_t len, char **astr, uint8_t *aoffset) {
	char    glyph;
	uint8_t ret = 0, glen;
	static uint16_t bitsleft;
	static uint8_t  bitsleftc;
	static uint8_t  fontNr = 1;

	if (str) {
		*astr    = str;
		*aoffset = offset;

		// skip glyphs until offset reached
		glyph = **astr - font[fontNr].glyph_beg;
		glen = pgm_read_byte(font[fontNr].width_table+glyph);
		while(*aoffset > glen) {
			*aoffset -= glen;
			glyph = **astr - font[fontNr].glyph_beg;
			glen = pgm_read_byte(font[fontNr].width_table+glyph);
		}

		bitsleftc = glen;
		bitsleft  = pgm_read_word(
				font[fontNr].glyph_table +                                   // table start
				glyph * FONT_HEIGHT+                                         // glyph offset
		       	        y ); 
	};

	while(len) {
		glen = min(len, bitsleftc);
		len  -= glen;
		ret <<= glen;
		ret |= (uint8_t)(bitsleft >> (16-glen));
		bitsleft  <<= glen;
		bitsleftc -= glen;

		if (!bitsleftc) {
			++(*astr);

			if (**astr) {
				glyph = **astr - font[fontNr].glyph_beg;
				bitsleftc = pgm_read_byte(font[fontNr].width_table+glyph);
				*aoffset  -= bitsleftc;
				bitsleft  = pgm_read_word(
					font[fontNr].glyph_table +                      // table start
					glyph * FONT_HEIGHT +                           // glyph offset
			       	        y ); 
			} else {
				--(*astr);
				bitsleft  = 0x0000;
				bitsleftc = 16;
			}
		}
	}

	return ret;
}


void repaint(char *txt, uint8_t offset) {
	char *astr;
	uint8_t x, y, bits, aoff;

	for(y=0; y<NUM_ROWS; y++) {
		bits = get_next_bits(txt, 0, y, 8, &astr, &aoff);
		for(x=NUM_COLS/8; x!=0xFF; x--) {

			pixmap[0][y][x] = bits;
			pixmap[1][y][x] = bits;
			pixmap[2][y][x] = bits;
			
			bits = get_next_bits(0, 0, y, 8, &astr, &aoff);
		}
	}
}


void scrolltext(char *str, unsigned char fontNr, unsigned int delay) {
     for(;;) {
	     repaint("1234567890", 0);
	     wait(10);
     };

     /*
     while (*tmp) {
        shift_in(*tmp++, fontNr, delay);
     }
     shift_out(NUM_COLS+8, delay);
     */
}

