#define SCROLLTEXT_C

#include "config.h"
#include "scrolltext.h"/
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
font fonts[MAX_FONTS];
#define MAX_SPECIALCOLORS 3
unsigned char PROGMEM colorTable[MAX_SPECIALCOLORS*NUM_ROWS] = {1, 1, 2, 3, 3, 2, 1, 1,   
                                                                3, 3, 2, 1, 1, 2, 3, 3,    
                                                                3, 3, 2, 2, 3, 3, 2, 2};

/*
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
*/

unsigned int getLen(char *str, unsigned char fontNr, unsigned char space) {
    char glyph;
    unsigned int strLen = 0;	
    if (fontNr >= MAX_FONTS) 
    	fontNr = MAX_FONTS - 1;
	
    while ((glyph = *str++)) {
        if ((glyph < fonts[fontNr].glyph_beg) || (glyph > fonts[fontNr].glyph_end)) {
           glyph = fonts[fontNr].glyph_def;
        } 
        glyph -= fonts[fontNr].glyph_beg;
        strLen += pgm_read_word(fonts[fontNr].fontIndex+glyph+1) -
                  pgm_read_word(fonts[fontNr].fontIndex+glyph); 
        strLen += space;
    }      
    return strLen-space; // den letzten space wieder abziehen
}


void draw_Text(char *str, unsigned int posx, char posy, unsigned char fontNr, unsigned char space, unsigned char color) {
	unsigned char byte;
    char x, y, glyph = *str;
	unsigned int charC, charEnd;
	if (color > NUMPLANE+MAX_SPECIALCOLORS) 
	   color = 3;
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
	while (posx > NUM_COLS) {
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
        posx--;
    }
	for (x = posx; x >= 0; x--) {
        byte = pgm_read_byte(fonts[fontNr].fontData+fonts[fontNr].storebytes*charC);
		for (y = posy; y < NUM_ROWS; y++) {
			if (byte & (1 << (y-posy)) && y-posy >= 0 && y >= 0 && x < NUM_COLS) {	
               setpixel((pixel){x, y}, color <= NUMPLANE? color: 
                                       pgm_read_byte(colorTable+(color-NUMPLANE-1)*NUM_ROWS+y-posy));
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

void scrolltext(char *str, unsigned char fontNr, unsigned int delay) {
	fonts[0] = font_uni53;
	char x;
     unsigned int posx = (getLen(str, 0, 1)+NUM_COLS)/2;
     //char *tmp = str;
     //while (*tmp) {
     //   shift_in(*tmp++, fontNr, delay);
     //}
     //shift_out(NUM_COLS+8, delay);
     clear_screen(0);
     draw_Text(str, posx, 0, 0, 1, 5);
     wait(3000);
     clear_screen(0);
     for (x = 0; x < 80; x++) {
         draw_Text(str, x, 0, 0, 1, 7);
         wait(100-x);
         clear_screen(0);
     }
     for (x = 80; x > 60; x--) {
         draw_Text(str, x, 0, 0, 1, 4);
         wait(70);
         clear_screen(0);
     }
     for (x = -7; x < 8; x++) {
         draw_Text(str, posx, x, 0, 1, 5);
         if (!x) wait(500);
         wait(150);
         clear_screen(0);
     }
     clear_screen(3);
     draw_Text(str, posx, 0, 0, 1, 0);
     wait(3000);
}


/* Konzept
   =======
Text wird in Token unterteilt, jeder Token bekommt einen Command-String.
z.B.

LABOR#b<#

Es werden die Zeiger aus dem Eingabestring direkt übernommen, mit Stinglen.
Wenn der Command abgearbeitet ist wird automatisch das nächste Token eingelesen.

Commands:
	
 */
typedef struct {
	char *str;
	char strLen;
	char *commands;
	char commandLen;
	unsigned int posx;
	char posy;
	unsigned int tox;
	char toy;
	unsigned char fontNr:3, bink:1, space:3, color:4;
} stringToken;	

#define ESC_CHAR '#'
#define MAX_TOKEN 256

void textAnim(char *str) {
	stringToken tokens[MAX_TOKEN];	
	unsigned char i, j;
	i = 0;
	j = 0;
	while (str[i+1] && str[i] != '#') {
		i++; 
		j++;
	}
	  	
	
	// parse string token
	while (1) {
			
	}
} 
