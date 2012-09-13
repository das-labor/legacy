#define SCROLLTEXT_C

#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <stdint.h>

#include "../config.h"
#include "scrolltext.h"

#include "../pixel.h"
#include "../util.h"
#include "font_arial8.h"
#if SCROLLTEXT_FONT == FONT_ARIAL8
	#include "font_arial8.h"
	#define FONT_NAME font_arial8
#elif SCROLLTEXT_FONT == FONT_SMALL6
	#include "font_small6.h"
	#define FONT_NAME font_small6
#elif SCROLLTEXT_FONT == FONT_C64
	#include "font_c64.h"
	#define FONT_NAME font_c64
#elif SCROLLTEXT_FONT == FONT_UNI53
	#include "font_uni53.h"
	#define FONT_NAME font_uni53
#else
	#include "font_arial8.h"
	#define FONT_NAME font_arial8
#endif

#ifdef __CYGWIN__
	#define strtok_r(a, b, c) strtok((a), (b))
#endif

#define MAX_FONTS 1
font fonts[MAX_FONTS];
#define MAX_SPECIALCOLORS 3

unsigned const char PROGMEM colorTable[MAX_SPECIALCOLORS*NUM_ROWS] = {1, 1, 2, 3, 3, 2, 1, 1,
                                                                3, 3, 2, 1, 1, 2, 3, 3,
                                                                3, 3, 2, 2, 3, 3, 2, 2
};

const char default_text[] PROGMEM = SCROLLTEXT_TEXT;
char scrolltext_text[SCROLLTEXT_BUFFER_SIZE];

/* Konzept
   =======
Text wird in Token unterteilt, jeder Token bekommt einen Command-String.
z.B.

#b</#LABOR

Es werden die Zeiger aus dem Eingabestring direkt übernommen, mit Stringlen.
Wenn der Command abgearbeitet ist wird automatisch das nächste Token eingelesen.

 */


unsigned char (*text_pixmap)[NUM_ROWS][LINEBYTES];

static void text_setpixel(pixel p, unsigned char value ){
	if(value){
		(*text_pixmap)[p.y%NUM_ROWS][p.x/8] |= shl_table[p.x%8];
	}
}

static void clear_text_pixmap(){
	memset(text_pixmap, 0, NUM_ROWS * LINEBYTES);
}

void update_pixmap(){
	unsigned char x, y, z;
	for(x=NUMPLANE;x--;){
		for(y=NUM_ROWS;y--;){
			for(z=LINEBYTES;z--;){
				pixmap[x][y][z] = (*text_pixmap)[y][z];
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
	wait_col_r
};
#ifdef NDEBUG
	typedef unsigned char waitfor_e_t;
#else
	typedef enum waitfor_e waitfor_e_t;
#endif


#define DIRECTION_RIGHT 0x01
#define DIRECTION_DOWN 0x02

struct blob_t_struct;
typedef struct blob_t_struct{
	struct blob_t_struct * next, * last;
	char *str;
	char *commands;
	waitfor_e_t waitfor;
	int sizex;
	char sizey;
	int posx;
	char posy;
	int tox;
	char toy;
	unsigned char delayx, delayx_rld;
	unsigned char delayy, delayy_rld;
	unsigned char delayb, delayb_rld;
	unsigned char visible;
	unsigned char direction;
	unsigned int timer;

	const unsigned int* fontIndex;
	const unsigned char* fontData;
	unsigned char font_storebytes;/*bytes per char*/
	unsigned char space;
#ifndef AVR
	char scrolltextBuffer[SCROLLTEXT_BUFFER_SIZE];
#endif
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

static unsigned int getLen(blob_t *blob) {
	unsigned char glyph;
	unsigned int strLen = 0;
	unsigned char * str = (unsigned char*)blob->str;
	uint8_t space = blob->space * blob->font_storebytes;

	while ((glyph = *str++)) {
		glyph -= 1;
		strLen += PW(blob->fontIndex[glyph+1]) - PW(blob->fontIndex[glyph]);
		strLen += space;
	}
	return strLen/blob->font_storebytes;
}


static unsigned int getnum(blob_t * blob){
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
				blob->delayx_rld = SCROLL_X_SPEED;
			}
			blob->delayx = blob->delayx_rld;
			break;
		case '>':
			blob->direction |= DIRECTION_RIGHT;
			if((tmp = getnum(blob)) != 0xFFFF){
				blob->delayx_rld = tmp;
			}else{
				blob->delayx_rld = SCROLL_X_SPEED;
			}
			blob->delayx = blob->delayx_rld;
			break;
		case 'd':
			blob->direction |= DIRECTION_DOWN;
			if((tmp = getnum(blob)) != 0xFFFF){
				blob->delayy_rld = tmp;
			}else{
				blob->delayy_rld = SCROLL_Y_SPEED;
			}
			blob->delayy = blob->delayy_rld;
			break;
		case 'u':
			blob->direction &= ~DIRECTION_DOWN;
			if((tmp = getnum(blob)) != 0xFFFF){
				blob->delayy_rld = tmp;
			}else{
				blob->delayy_rld = SCROLL_Y_SPEED;
			}
			blob->delayy = blob->delayy_rld;
			break;
		case 'x'://Place string at this x Position
			if((tmp = getnum(blob)) != 0xFFFF){
				blob->posx = tmp;
			}else{
				blob->posx =  NUM_COLS/2 + blob->sizex/2;
			}
			break;
		case 'y'://Place string at this y Position
			if((tmp = getnum(blob)) != 0xFFFF){
				blob->posy = tmp - blob->sizey;
			}
			break;
		case 'b'://blink blob
			if((tmp = getnum(blob)) != 0xFFFF){
				blob->delayb_rld = tmp;
			}else{
				blob->delayb_rld = 50;
			}
			blob->delayb = blob->delayb_rld;
			break;
		case '|':
			if((tmp = getnum(blob)) != 0xFFFF){
				blob->tox = tmp;
			}else{
				blob->tox = (NUM_COLS - 2 + blob->sizex)/2;
			}
			blob->waitfor = wait_posx;
			return retval;
			break;
		case '-':
			if((tmp = getnum(blob)) != 0xFFFF){
				blob->toy = tmp;
			}else{
				blob->toy = (UNUM_ROWS-blob->sizey) / 2; //MARK
			}
			blob->waitfor = wait_posy;
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
		case ':':
			blob->waitfor = wait_col_r;
			return (retval);
			break;
		case '+':
			retval = 2;
			break;
		}
	}
	return 1;//this blob is finished, and can be deleted.
}


blob_t * setupBlob(char * str){
	static unsigned char chop_cnt;
	static char *last; static char delim[] = "#";
	static char *lastcommands;
	unsigned int tmp;

	blob_t *blob = malloc(sizeof (blob_t));

	if(str){
#ifndef AVR
		// on non-AVR archs strtok_r fails for some reason if it operates on a
		// string which is located on another stack frame, so we need our own copy
		memcpy (blob->scrolltextBuffer, str, SCROLLTEXT_BUFFER_SIZE);
		str = blob->scrolltextBuffer;
#endif
		chop_cnt = 0;
	}

	if(!chop_cnt){
		blob->commands = strtok_r (str, delim, &last);

		if( blob->commands == 0) goto fail;

		if((tmp = getnum(blob)) != 0xFFFF){
			chop_cnt = tmp;
			lastcommands = blob->commands;
		}
	}

	if(chop_cnt){
		chop_cnt--;
		blob->commands = lastcommands;
	}

	blob->str = strtok_r (NULL, delim, &last);

	if ( blob->str == 0) goto fail;

	blob->fontIndex = fonts[0].fontIndex;
	blob->fontData = fonts[0].fontData;
	blob->font_storebytes = fonts[0].storebytes;

	unsigned char tmp1, *strg = (unsigned char*)blob->str;
	unsigned char glyph_beg = fonts[0].glyph_beg;
	unsigned char glyph_end = fonts[0].glyph_end;

	//translate the string: subtract 1 to get offset in Table
	while((tmp1 = *strg)){
		if((tmp1>=glyph_beg) && (tmp1<glyph_end)){
			*strg = 1 + tmp1 - glyph_beg;
		}else{
			*strg = 1;
		}
		strg++;
	}

	blob->space = 1;

	blob->sizey = fonts[0].fontHeight;
	blob->sizex = getLen(blob);
	if(*blob->commands == '<'){
		blob->posx = 0;
		blob->posy = (UNUM_ROWS-blob->sizey)/2; //MARK
	}else if(*blob->commands == '>'){
		blob->posx = NUM_COLS+blob->sizex;
		blob->posy = (UNUM_ROWS-blob->sizey)/2; //MARK
	}else if(*blob->commands == 'd'){
		blob->posy = -blob->sizey;
		blob->posx = (UNUM_COLS - 2 + blob->sizex)/2; //MARK
	}else if(*blob->commands == 'u'){
		blob->posy = blob->sizey;
		blob->posx = (UNUM_COLS - 2 + blob->sizex)/2; //MARK
	}else if(*blob->commands == 'x'){
		blob->posy = (UNUM_ROWS-blob->sizey)/2; //MARK
	}else if(*blob->commands == 'y'){
		blob->posx = (UNUM_COLS - 2 + blob->sizex)/2; //MARK
	}

	blob->delayx_rld = 0;
	blob->delayy_rld = 0;
	blob->delayb_rld = 0;

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

	if(blob->delayy_rld && (!(blob->delayy--))){
		blob->delayy = blob->delayy_rld;
		(blob->direction & DIRECTION_DOWN)?blob->posy++:blob->posy--;
	}

	if(blob->delayb_rld){
		if(!(blob->delayb--)){
			blob->delayb = blob->delayb_rld;
			blob->visible ^= 1;
		}
	}else{
		blob->visible = 1;
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
			if((blob->posy) > NUM_ROWS || (blob->posy + blob->sizey) <0 ) done = 1;
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
		case wait_col_r:
			if(blob->next){
				if(blob->next->posx == (blob->posx - blob->sizex)){
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
	unsigned char byte=0, glyph, storebytes;
	unsigned int charPos, charEnd;

	unsigned int posx; unsigned char posy, toy;

	if(!blob->visible) return;

	unsigned char * str = (unsigned char*)blob->str;
	posx = blob->posx;
	posy = blob->posy;
	toy = posy + blob->sizey;
	storebytes = blob->font_storebytes;

	glyph = (*blob->str)-1;
	charPos = PW(blob->fontIndex[glyph]);
	charEnd = PW(blob->fontIndex[glyph+1]);

	while (posx >= NUM_COLS) {
		charPos += storebytes;
		if (charPos < charEnd) {
			posx--;
		}else{
			posx -= blob->space + 1;
			if (!(glyph = *++str)) return;
			glyph -= 1;
			charPos = PW(blob->fontIndex[glyph]);
			charEnd = PW(blob->fontIndex[glyph+1]);
		}
	}
	for (x = posx; x >= 0; x-- ) {
		unsigned char mask = 0;
		unsigned int datpos;
		datpos = charPos;

		for (y = posy; (y < NUM_ROWS) && (y < toy); y++) {

			if((mask<<=1) == 0){
				mask = 0x01;
				byte = PB(blob->fontData[datpos++]);
			}

			if ((byte & mask) && y >= 0 ) {
					text_setpixel((pixel){x, y},1);
			}
		}
		charPos += storebytes;
		if (charPos < charEnd) {
		}else{
			x -= blob->space;
			if (!(glyph = *++str)) return;
			glyph -= 1;
			charPos = PW(blob->fontIndex[glyph]);
			charEnd = PW(blob->fontIndex[glyph+1]);
		}
	}
}

extern jmp_buf newmode_jmpbuf;

void scrolltext(char *str) {
	jmp_buf tmp_jmpbuf;
	char tmp_str[SCROLLTEXT_BUFFER_SIZE];
	int ljmp_retval;

	fonts[0] = FONT_NAME;

	unsigned char auto_pixmap[NUM_ROWS][LINEBYTES];
	text_pixmap = &auto_pixmap;

	if(scrolltext_text[0] == 0){
		strcpy_P(scrolltext_text, default_text);
	}
	memcpy(tmp_str, str, SCROLLTEXT_BUFFER_SIZE);

	blob_t *startblob=0, *aktblob, *nextblob=0;

	memcpy (tmp_jmpbuf, newmode_jmpbuf, sizeof(jmp_buf));
	if((ljmp_retval = setjmp(newmode_jmpbuf))){
		while(startblob){
			aktblob = startblob;
			startblob = aktblob->next;
			free(aktblob);
		}
		memcpy (newmode_jmpbuf, tmp_jmpbuf, sizeof(jmp_buf));
		longjmp(newmode_jmpbuf, ljmp_retval);
	}

	if (!(startblob = setupBlob(tmp_str))){
		goto exit;
	}

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
			clear_text_pixmap();
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

exit:
	memcpy (newmode_jmpbuf, tmp_jmpbuf, sizeof(jmp_buf));
}
