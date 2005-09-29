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


/*
void shift_in(char glyph, unsigned char fontNr, unsigned int delay){
	unsigned char i, y, ror, len;
	if ((glyph < font[fontNr].glyph_beg) || (glyph > font[fontNr].glyph_end)) {
            glyph = font[fontNr].glyph_def;
    } 
	glyph -= font[fontNr].glyph_beg;
	len = pgm_read_byte(font[fontNr].width_table+glyph);
	ror = (1 << 7);
	for (i = 0; i < len; i++) {
		shift_pixmap_l();
		if (i > 7) ror = (1 <<  7 -(i - 8));
		for (y = 0; y < font[fontNr].glyph_height; y++) {
			setpixel((pixel){NUM_COLS-1, y},(pgm_read_byte(font[fontNr].glyph_table + (i>7?1:0) + y * font[fontNr].store_width +
                                             glyph * font[fontNr].glyph_height * font[fontNr].store_width) & ror)?3:0 );
		}
		ror >>= 1;
		wait(delay);
	}
}


void shift_out(unsigned char cols, unsigned int delay){
	unsigned char i;
	for(i=0;i<cols;i++){
		shift_pixmap_l();
		wait(delay);
	}
}

*/

#define max(a,b) ((a>b)? a : b)

uint8_t get_next_bits(char *str, uint8_t offset, uint8_t y, uint8_t len, char **astr, uint8_t *aoffset) {
	char    glyph;
	uint8_t ret = 0, glen;
	static uint16_t bitsleft;
	static uint8_t  bitsleftc;
	static uint8_t  fontNr = 0;

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
				glyph*font[fontNr].glyph_height*font[fontNr].store_width +   // glyph offset
		       	        y*font[fontNr].store_width ); 
	};

	while(len) {
		glen = max(len, bitsleftc);
		ret <<= glen;
		ret |= (uint8_t)(bitsleft >> (16-glen));
		bitsleft  <<= glen;
		bitsleftc -= glen;

		if (!bitsleft) {
			glyph = *(*astr++) - font[fontNr].glyph_beg;
			bitsleftc = pgm_read_byte(font[fontNr].width_table+glyph);
			*aoffset  -= bitsleftc;
			bitsleft  = pgm_read_word(
				font[fontNr].glyph_table +                                   // table start
				glyph*font[fontNr].glyph_height*font[fontNr].store_width +   // glyph offset
		       	        y*font[fontNr].store_width ); 
		}
	}

	return ret;
}

void repaint(char *txt, uint8_t offset) {
	char *nextchar, glyph;
	uint8_t x, y, glen, bits, nextoff, fontNr=1;

	for(y=0; y<NUM_ROWS; y++) {
		for(x=0; x<NUM_COLS/8; x++) {
		nextchar = txt;
		nextoff = offset;

		while(1)  {
			glyph = *nextchar;
			glyph -= font[fontNr].glyph_beg;
			glen = pgm_read_byte(font[fontNr].width_table+glyph);
					
			bits = pgm_read_byte(
					font[fontNr].glyph_table +                                   // table start
					glyph*font[fontNr].glyph_height*font[fontNr].store_width +   // glyph offset
			       	        y*font[fontNr].store_width );                                // row offset

			pixmap[0][y][x] = bits;
			pixmap[1][y][x] = bits;
			pixmap[2][y][x] = bits;
			
			nextchar++;
			x++;

			if (x > (NUM_COLS/8)-1 ) break;
	     		wait(100);
		}
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

