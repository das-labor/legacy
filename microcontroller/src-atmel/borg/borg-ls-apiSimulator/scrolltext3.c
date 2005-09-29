#define SCROLLTEXT_C

#include "config.h"
#include "scrolltext2.h"
#ifdef AVR
#   include "borg_hw.h"
#endif

#include "pixel.h"
#include "util.h"
//#include "font-arial8.h"
#include "font-small6.h"

#define MAX_FONTS 1
font fonts[MAX_FONTS];


void shift_in(char glyph, unsigned char fontNr, unsigned int delay){
	unsigned char  y, s, len;
	unsigned int i;
	if (fontNr >= MAX_FONTS) 
		fontNr = MAX_FONTS - 1;
	if ((glyph < fonts[fontNr].glyph_beg) || (glyph > fonts[fontNr].glyph_end)) {
            glyph = fonts[fontNr].glyph_def;
    } 
	glyph -= fonts[fontNr].glyph_beg;
	for (i = pgm_read_byte(fonts[fontNr].fontIndex+glyph); i < pgm_read_byte(fonts[fontNr].fontIndex+glyph+1); i++) {
		shift_pixmap_l();
		for (s = 0; s < fonts[fontNr].storebytes; s++) {
			for (y = 7; y < 8; y--) {
				if (pgm_read_byte(fonts[fontNr].fontData+fonts[fontNr].storebytes*i+s) & (1 << y)) {	
						setpixel((pixel){NUM_COLS-1, y+(fonts[fontNr].storebytes-1-s)*8}, 3);
				}
			}
		}
		wait(delay);
	}
	shift_pixmap_l();
	wait(delay);
}


void shift_out(unsigned char cols, unsigned int delay){
	unsigned char i;
	for(i=0;i<cols;i++){
		shift_pixmap_l();
		wait(delay);
	}
}

void scrolltext(char *str, unsigned char fontNr, unsigned int delay) {
	 fonts[0] = font_small6;
     char *tmp = str;
     while (*tmp) {
        shift_in(*tmp++, fontNr, delay);
     }
     shift_out(NUM_COLS+8, delay);
}
