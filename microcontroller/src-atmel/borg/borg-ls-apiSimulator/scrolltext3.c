#define SCROLLTEXT_C

#include "config.h"
#include "scrolltext2.h"
#ifdef AVR
#   include "borg_hw.h"
#endif

#include "pixel.h"
#include "util.h"
//#include "font-arial8.h"
//#include "font-small6.h"
//#include "font-v5.h"
#include "font-uni53.h"

#define MAX_FONTS 1
font fonts[MAX_FONTS] = {font_uni53};


void shift_in(unsigned char glyph, unsigned char fontNr, unsigned int delay){
	unsigned char  y, s, len, byte;
	unsigned int i;
	if (fontNr >= MAX_FONTS) 
		fontNr = MAX_FONTS - 1;
	if ((glyph < fonts[fontNr].glyph_beg) || (glyph > fonts[fontNr].glyph_end)) {
            glyph = fonts[fontNr].glyph_def;
    } 
	glyph -= fonts[fontNr].glyph_beg;
	for (i = pgm_read_word(fonts[fontNr].fontIndex+glyph); i < pgm_read_word(fonts[fontNr].fontIndex+glyph+1); i++) {
		shift_pixmap_l();
		for (s = 0; s < fonts[fontNr].storebytes; s++) {
            byte = pgm_read_byte(fonts[fontNr].fontData+fonts[fontNr].storebytes*i+s);
			for (y = 7; y < 8; y--) {
				if (byte & (1 << y)) {	
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
     unsigned char x, y;
     //char *tmp = str;
     //while (*tmp) {
     //   shift_in(*tmp++, fontNr, delay);
     //}
     //shift_out(NUM_COLS+8, delay);
     clear_screen(0);
     draw_Text(str, 72, 1, 0, 1, 3);
     wait(3000);
     clear_screen(0);
     for (x = 0; x < 80; x++) {
         draw_Text(str, x, 0, 0, 1, x%2?1:3);
         wait(100-x);
         clear_screen(0);
     }
     draw_Text(str, 80, 3, 0, 1, 3);
     wait(70);
     clear_screen(0);
     for (x = 80; x > 60; x--) {
         draw_Text(str, x, 0, 0, 1, 3);
         wait(70);
         clear_screen(0);
     }
}

               // Pos
void draw_Text(char *str, char posx, char posy, unsigned char fontNr, unsigned char space, unsigned char color) {
	unsigned char byte;
    char x, y, glyph = *str;
	unsigned int charC, charEnd;
	if (fontNr >= MAX_FONTS) 
		fontNr = MAX_FONTS - 1;
	if ((glyph < fonts[fontNr].glyph_beg) || (glyph > fonts[fontNr].glyph_end)) {
       glyph = fonts[fontNr].glyph_def;
    } 
	glyph -= fonts[fontNr].glyph_beg;
	
	charC = pgm_read_word(fonts[fontNr].fontIndex+glyph);
	charEnd = pgm_read_word(fonts[fontNr].fontIndex+glyph+1);
	if (fontNr >= MAX_FONTS) 
		fontNr = MAX_FONTS - 1;
	for (x = posx; x; x--) {
        byte = pgm_read_byte(fonts[fontNr].fontData+fonts[fontNr].storebytes*charC);
		for (y = posy; y < NUM_ROWS; y++) {
			if (byte & (1 << (y-posy))) {	
               setpixel((pixel){x, y}, color);
			}	
		}
		if (charC < charEnd) {                  
            charC++;
        } 
        if (charC >= charEnd) {
            x -= space;
            if (!(glyph = *++str)) return;      
            if ((glyph < fonts[fontNr].glyph_beg) || (glyph > fonts[fontNr].glyph_end)) {      
               glyph = fonts[fontNr].glyph_def;
            } 
            glyph -= fonts[fontNr].glyph_beg;   
            charC = pgm_read_word(fonts[fontNr].fontIndex+glyph);
            charEnd = pgm_read_word(fonts[fontNr].fontIndex+glyph+1);
        }
	}	
}
