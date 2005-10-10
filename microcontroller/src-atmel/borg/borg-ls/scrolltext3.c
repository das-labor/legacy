#define SCROLLTEXT_C

#include <stdlib.h>
#include <string.h>
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
                                                                3, 3, 2, 2, 3, 3, 2, 2
};


/* Konzept
   =======
Text wird in Token unterteilt, jeder Token bekommt einen Command-String.
z.B.

#b<#LABOR

Es werden die Zeiger aus dem Eingabestring direkt übernommen, mit Stinglen.
Wenn der Command abgearbeitet ist wird automatisch das nächste Token eingelesen.

 */


unsigned char text_pixmap[NUM_ROWS][LINEBYTES];

void text_setpixel(pixel p, unsigned char value ){
	if(value){
		text_pixmap[p.y%NUM_ROWS][p.x/8] |= shl_table[p.x%8];
	}
}

void clear_text_pixmap(unsigned char value){
	unsigned char y, z;
	for(y=0;y<NUM_ROWS;y++){
		for(z=0;z<LINEBYTES;z++){
			text_pixmap[y][z] = 0;
		}
	}
	
}

void update_pixmap(){
	unsigned char x, y, z;
	for(x=0;x<NUMPLANE;x++){
		for(y=0;y<NUM_ROWS;y++){
			for(z=0;z<LINEBYTES;z++){
				pixmap[x][y][z] = text_pixmap[y][z];
			}
		}
	}
}

enum waitfor_e{
	wait_new,
	wait_posy,
	wait_posx,
	wait_out,
	wait_timer,
	wait_col_l,
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
	
	const unsigned int* fontIndex;
	const unsigned char* fontData;
	unsigned char space;
}blob_t;


/*
void showBlob(blob_t * blob){
	unsigned char * str = blob->str;
	unsigned char tmp[200], x=0;
	while(*str){
		tmp[x++] = (*str++) + ' ' -1;
	}
	tmp[x] = 0;
	printf("this:\t%x\n",blob);
	printf("last:\t%x\n",blob->last);
	printf("next:\t%x\n",blob->next);
	printf("str:\t%s\n",tmp);
	printf("cmd:\t%s\n",blob->commands);
	printf("sizex\t%d\n",blob->sizex);
	printf("posx\t%d\n",blob->posx);
	printf("posy\t%d\n",blob->posy);
	printf("tox\t%d\n",blob->tox);
	printf("toy\t%d\n",blob->toy);
	printf("delayy_rld\t%d\n",blob->delayx_rld);
	printf("delayx_rld\t%d\n",blob->delayy_rld);
	printf("timer\t%d\n",blob->timer);
	printf("\n");
}
*/


#define PW(a) pgm_read_word(&(a))
#define PB(a) pgm_read_byte(&(a))

unsigned int getLen(blob_t *blob) {
	unsigned char glyph;
	unsigned int strLen = 0;
	unsigned char * str = blob->str;
	
	while ((glyph = *str++)) {
		glyph -= 1;
		strLen += PW(blob->fontIndex[glyph+1]) -
			PW(blob->fontIndex[glyph]); 
			strLen += blob->space;
	}      
	return strLen;
}



unsigned int getnum(blob_t * blob){
	unsigned int num=0;
	unsigned char gotnum = 0;
	
	while( (*blob->commands >= '0') && (*blob->commands <='9') ){
		gotnum = 1;
		num *= 10;
		num += *blob->commands - '0';
		blob->commands++;
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
	while(*blob->commands != 0){
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
		case ';':
			blob->waitfor = wait_col_l;
			return (retval);
			break;
		case '+':
			retval = 2;
			break;
		}
	}
	return 1;//this blob is finished, and can be deleted.
}


blob_t * setupBlob(unsigned char * str){
	/*char * strtok_r ( char * string, const char * delim, char ** last)*/ 
	static unsigned char chop_cnt;
	static char *last; static char delim[] = "#";
	static unsigned char *lastcommands;
	
	if(str){
		chop_cnt = 0;
	}
	
	blob_t *blob = malloc(sizeof (blob_t));
	
	if(!chop_cnt){
		blob->commands = strtok_r (str, delim, &last);
		if( blob->commands == 0) goto fail;
		
		unsigned int tmp;
		if((tmp = getnum(blob)) != 0xFFFF){
			chop_cnt = tmp;
			lastcommands = blob->commands;
		}
	}
	
	if(chop_cnt){
		chop_cnt--;
		blob->commands = lastcommands;
	}
	
	blob->str = strtok_r (0, delim, &last);

	if ( blob->str == 0) goto fail;

	blob->fontIndex = fonts[0].fontIndex;
	blob->fontData = fonts[0].fontData;
	
	unsigned char tmp, *strg = blob->str;
	unsigned char glyph_beg = fonts[0].glyph_beg;
	unsigned char glyph_end = fonts[0].glyph_end;

	//translate the string: subtract 1 to get offset in Table
	while((tmp = *strg)){
		if((tmp>=glyph_beg) && (tmp<glyph_end)){
			*strg = 1 + tmp - glyph_beg;
		}else{
			*strg = 1;
		}
		strg++;
	}
	
	blob->space = 1;
	
	blob->sizex = getLen(blob);
	if(*blob->commands == '<'){
		blob->posx = 0;
		blob->posy = 0;
	}else if(*blob->commands == '>'){
		blob->posx = NUM_COLS+blob->sizex;
		blob->posy = 0;
	}
	
	blob->delayx_rld = 0;
	blob->delayy_rld = 0;
	
	blob->waitfor = wait_new;
	
	return blob;
	
fail:
	free(blob);
	return 0;//no more blobs to parse
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
		case wait_col_l:
			if(blob->last){
				if((blob->last->posx - blob->last->sizex) == blob->posx){
					done=1;
				}
			}else{
				done = 1;
			}
			break;
		default:
			done = 1;
			break;
	}
	if(done){
		return (blobNextCommand(blob));
	}
	return 0;
}

void drawBlob(blob_t *blob) {
	char x, y; 
	unsigned char byte, glyph; 
	unsigned int charPos, charEnd;
	
	unsigned int posx; unsigned char posy;
	unsigned char color = 3;
	unsigned char * str = blob->str;
	posx = blob->posx;
	posy = blob->posy;
	
	glyph = (*blob->str)-1;
	charPos = PW(blob->fontIndex[glyph]);
	charEnd = PW(blob->fontIndex[glyph+1]) -1 ;

	while (posx >= NUM_COLS) {
		if (charPos < charEnd) {                  
			charPos++;
			posx--;
		}else{
			posx -= blob->space + 1;
			if (!(glyph = *++str)) return;      
			glyph -= 1;
			charPos = PW(blob->fontIndex[glyph]);
			charEnd = PW(blob->fontIndex[glyph+1]) - 1;
		}
	}
	for (x = posx; x >= 0; x-- ) {
		unsigned char mask = 0x01;
		byte = PB(blob->fontData[charPos]);
		for (y = posy; y < NUM_ROWS; y++) {
			
			if ((byte & mask) && y >= 0 ) {	
					text_setpixel((pixel){x, y},1);
			}
			mask <<= 1;
		}
		
		if (charPos < charEnd) {                  
			charPos++;
		}else{
			x -= blob->space;
			if (!(glyph = *++str)) return;       
			glyph -= 1;   
			charPos = PW(blob->fontIndex[glyph]);
			charEnd = PW(blob->fontIndex[glyph+1]) - 1;
		}
	}
}



void scrolltext(char *str, unsigned char fontNr, unsigned int delay) {
	fonts[0] = font_uni53;
	
	blob_t *startblob, *aktblob, *nextblob;

	startblob = setupBlob(str);
	//showBlob(startblob);
	
	unsigned char retval;
	do{
		startblob->next = 0;
		startblob->last = 0;
		while(startblob){
			aktblob = startblob;
			while(aktblob){
				retval = updateBlob(aktblob);
				if(!retval){
					nextblob = aktblob->next;
				}else if(retval == 1){
					if(aktblob == startblob){
						startblob = aktblob->next;
					}else{
						aktblob->last->next = aktblob->next;
					}
					if(aktblob->next){
						aktblob->next->last = aktblob->last;
					}
					nextblob = aktblob->next;
					free(aktblob);
			 	}else if(retval == 2){
					blob_t * newblob = setupBlob(0);
					if (newblob){
						newblob->last = aktblob;
						newblob->next = aktblob->next;
						if(aktblob->next){
							aktblob->next->last = newblob;
						}
						aktblob->next = newblob;
					}
					nextblob = aktblob->next;
				}
				aktblob = nextblob;
			}
						
			aktblob = startblob;
			clear_text_pixmap(0);
			while(aktblob){
				drawBlob(aktblob);
				aktblob = aktblob->next;
			}
			update_pixmap();
			wait(2);
			
		};
		startblob = setupBlob(0);
		//showBlob(startblob);
	}while(startblob);
}

