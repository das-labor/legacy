#include "grafiklcd.h"

#include "font_small6.h"

#define MAX_FONTS 1

font fonts[MAX_FONTS];


void draw_Text(char *str, unsigned int posx, char posy, unsigned char fontNr, 
               unsigned char space, unsigned char color) {
	unsigned char byte, align;
    char x, y, glyph = *str;
	unsigned int charC, charEnd;
	
	fonts[0] = font_small6;

	align = fonts[fontNr].storebytes - 1;

    if (fontNr >= MAX_FONTS) 
		fontNr = MAX_FONTS - 1;
	if ((glyph < fonts[fontNr].glyph_beg) || (glyph > fonts[fontNr].glyph_end)) {
       glyph = fonts[fontNr].glyph_def;
    } 
	glyph -= fonts[fontNr].glyph_beg;
	charC = pgm_read_word(fonts[fontNr].fontIndex+glyph);
	charEnd = pgm_read_word(fonts[fontNr].fontIndex+glyph+1);
	// skip part outside the display
	while (posx > NUM_COLS) { 
	    if (charC < charEnd) {                  
            charC++;
        } 
        if (charC >= charEnd) {
            x += space;
            if (!(glyph = *++str)) return;      
            if ((glyph < fonts[fontNr].glyph_beg) || (glyph > fonts[fontNr].glyph_end)) {      
               glyph = fonts[fontNr].glyph_def;
            } 
            glyph -= fonts[fontNr].glyph_beg;   
            charC = pgm_read_word(fonts[fontNr].fontIndex+glyph);
            charEnd = pgm_read_word(fonts[fontNr].fontIndex+glyph+1);
        }
        posx--;
    }
	for (x = posx; x >= 0; x++) {
        byte = pgm_read_byte(fonts[fontNr].fontData+charC);

		for (y = posy; y < 8+posy; y++) {
			if (byte & (1 << (y-posy)) && y-posy >= 0 && y >= 0 
			    && x < NUM_COLS) {	
               dispSetPix(x, y, color);
			}	
		}
	
		if (charC < charEnd) {
            charC++;
        } 
        if (charC >= charEnd) {
            x += space;
            if (!(glyph = *++str)) return;      
            if ((glyph < fonts[fontNr].glyph_beg) || (glyph > fonts[fontNr].glyph_end)) {      
               glyph = fonts[fontNr].glyph_def;
            } 
            glyph  -= fonts[fontNr].glyph_beg;   
            charC   = pgm_read_word(fonts[fontNr].fontIndex+glyph);
            charEnd = pgm_read_word(fonts[fontNr].fontIndex+glyph+1);
        }
		align++;
		if (!align) {
    		align = fonts[fontNr].storebytes - 1;
		}
	}	
}
