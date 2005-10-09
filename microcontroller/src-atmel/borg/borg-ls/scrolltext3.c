#define SCROLLTEXT_C

#include <stdlib.h>
#include "config.h"
#include "scrolltext.h"
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

#define PW(a) pgm_read_word(&(a))

void draw_Text(char *str, unsigned int posx, char posy, unsigned char fontNr, unsigned char space, unsigned char color) {
	char x, y; 
	unsigned char byte, glyph = *str;
	unsigned int charPos, charEnd;
	if (color > NUMPLANE+MAX_SPECIALCOLORS) 
		color = 3;
	if (fontNr >= MAX_FONTS) 
		fontNr = MAX_FONTS - 1;
	
	if ((glyph < fonts[fontNr].glyph_beg) || (glyph > fonts[fontNr].glyph_end)) {
		glyph = fonts[fontNr].glyph_def;
	}
	glyph -= fonts[fontNr].glyph_beg;
	charPos = PW(fonts[fontNr].fontIndex[glyph]);
	charEnd = PW(fonts[fontNr].fontIndex[glyph+1]);

	while (posx > NUM_COLS) {
		if (charPos < charEnd) {                  
			charPos++;
		}else{
			if (!(glyph = *++str)) return;      
			if ((glyph < fonts[fontNr].glyph_beg) || (glyph > fonts[fontNr].glyph_end)) {      
				glyph = fonts[fontNr].glyph_def;
			} 
			glyph -= fonts[fontNr].glyph_beg;
			charPos = PW(fonts[fontNr].fontIndex[glyph]);
			charEnd = PW(fonts[fontNr].fontIndex[glyph+1]);
		}
		posx--;
	}
	for (x = posx; x >= 0; x--) {
		byte = pgm_read_byte(fonts[fontNr].fontData+fonts[fontNr].storebytes*charPos);
		for (y = posy; y < NUM_ROWS; y++) {
			if (byte & (1 << (y-posy)) && y-posy >= 0 && y >= 0 && x < NUM_COLS) {	
               setpixel((pixel){x, y}, color <= NUMPLANE? color: 
                                       pgm_read_byte(colorTable+(color-NUMPLANE-1)*NUM_ROWS+y-posy));
			}	
		}
		if (charPos < charEnd) {                  
			charPos++;
		}else{
			x -= space;
			if (!(glyph = *++str)) return;      
			if ((glyph < fonts[fontNr].glyph_beg) || (glyph > fonts[fontNr].glyph_end)) {      
				glyph = fonts[fontNr].glyph_def;
			} 
			glyph -= fonts[fontNr].glyph_beg;   
			charPos = pgm_read_word(fonts[fontNr].fontIndex+glyph);
			charEnd = pgm_read_word(fonts[fontNr].fontIndex+glyph+1);
		}
	}
}


void textAnim(char *str);

void scrolltext(char *str, unsigned char fontNr, unsigned int delay) {
	fonts[0] = font_uni53;
	char x;
	
	textAnim(str);
	return;
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

#b<#LABOR

Es werden die Zeiger aus dem Eingabestring direkt übernommen, mit Stinglen.
Wenn der Command abgearbeitet ist wird automatisch das nächste Token eingelesen.

 */

enum waitfor_e{
	wait_posy,
	wait_posx,
	wait_out,
	wait_timer
};

#define DIRECTION_RIGHT 0x01

struct blob_t_struct;

typedef struct blob_t_struct{
	struct blob_t_struct * next, * last;
	char *str;
	char *commands;
	enum waitfor_e waitfor;
	int sizex;
	int posx;
	char posy;
	int tox;
	char toy;
	unsigned char delayx, delayx_rld;
	unsigned char delayy, delayy_rld;
	unsigned char direction;
	unsigned int timer;
	
	const uint16_t *glyph_table;
	const unsigned char *width_table;
}blob_t;


#define ESC_CHAR '#'
#define MAX_TOKEN 256

unsigned int getnum(blob_t * blob){
	unsigned int num=0;
	unsigned char gotnum = 0;
	
	while( (blob->commandLen != 0) && (*blob->commands >= 0) && (*blob->commands <=9) ){
		gotnum = 1;
		num *= 10;
		num += *blob->commands - '0';
		blob->commands++;
		blob->commandLen--;
	}
	
	if(gotnum){
		return num;
	}else{
		return 0xffff;
	}
}

unsigned char blobNextCommand(blob_t * blob){
	unsigned int tmp;
	unsigned char retval = 0;
	while(blob->commandLen != 0){
		blob->commandLen--;
		switch (*blob->commands++){
		case '<':
			blob->direction &= ~DIRECTION_RIGHT;
			if((tmp = getnum(blob)) != 0xFFFF){ 
				blob->delayx_rld = tmp;
			}else{
				blob->delayx_rld = 5;
			}
			blob->delayx = blob->delayx_rld;
			break;
		case '>':
			blob->direction |= DIRECTION_RIGHT;
			if((tmp = getnum(blob)) != 0xFFFF){ 
				blob->delayx_rld = tmp;
			}else{
				blob->delayx_rld = 5;
			}
			blob->delayx = blob->delayx_rld;
			break;
		case '|':
			if((tmp = getnum(blob)) != 0xFFFF){ 
				blob->tox = tmp;
			}else{
				blob->tox =  NUM_COLS/2 + blob->sizex/2;
			}
			blob->waitfor = wait_posx;
			return retval;
			break;
		case 'p':
			blob->delayx_rld = 0;
			blob->delayy_rld = 0;
			if((tmp = getnum(blob)) != 0xFFFF){ 
				blob->timer = tmp*64;
			}else{
				blob->timer =  30*64;
			}
			blob->waitfor = wait_timer;
			return retval;
			break;
		case '/':
			blob->waitfor = wait_out;
			return retval;
			break;
		case '+':
			retval = 2;
			break;
		}
	}
	return 1;//this blob is finished, and can be deleted.
}


unsigned char updateBlob(blob_t * blob){
	
	if(blob->delayx_rld && (!(blob->delayx--))){
		blob->delayx = blob->delayx_rld;
		(blob->direction & DIRECTION_RIGHT)?blob->posx--:blob->posx++;
	}
	
	unsigned char done=0;
	switch (blob->waitfor){
		case wait_posy:
			if (blob->posy == blob->toy)done = 1;
			break;
		case wait_posx:
			if (blob->posx == blob->tox)done = 1;
			break;
		case wait_out:
			if((blob->posx - blob->sizex) > NUM_COLS || blob->posx < 0) done = 1;
			break;
		case wait_timer:
			if(0 == blob->timer--){
				done = 1;
			}
			break;
		default:
			break;
	}
	if(done){
		return (blobNextCommand(blob));
	}
	return 0;
}

void drawBlob(blob_t * blob){
	draw_Text(blob->str, blob->posx, blob->posy, 0, 1, 7);
}


void textAnim(char *str) {
	blob_t * startblob;

	startblob = malloc(sizeof (blob_t));
	
	startblob->str = str;
	startblob->commands = "<|p3</";
	startblob->commandLen = 6;
	startblob->sizex = getLen(str, 0, 1);
	startblob->posx = 0;
	startblob->posy = 0;
	startblob->delayx_rld = 0;
	
	blobNextCommand(startblob);
	
	while(1){
		if(updateBlob(startblob)==1){
			free(startblob);
		 	return;
		}
		clear_screen(0);
		drawBlob(startblob);
		wait(10);
	}
}
