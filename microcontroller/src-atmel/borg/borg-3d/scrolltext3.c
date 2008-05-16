#define SCROLLTEXT_C

#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#include "config.h"
#include "scrolltext.h"
#include "pixel.h"
#include "util.h"
#include "font_uni53.h"

#ifdef AVR
#   include "borg_hw.h"
#endif

#define MAX_FONTS 1
font fonts[MAX_FONTS];
#define MAX_SPECIALCOLORS 3

/** 
BorgTextAnim
============

Dies ist eine einfache Sprache, die extra dafür entwickelt wurde, um in einer 
möglichst kompakten und einfach zu interpretierenden Schreibweise hübsche 
Text-Animationen zu ermöglichen.

Animationen bestehen aus Textblöcken die immer aus einem Kommandoteil und 
einen Textteil zusammengesetzt sind. Kommandoteil, Textteil und die Textblöcke 
untereinander werden durch das Zeichen # getrennt.

Der Kommandoteil besteht aus Befehlen und aus Warte Anweisungen. Der Befehl 
oder die Befehle werden solange ausgeführt bis man sie widerruft. Folgt eine 
Zahl nach einen Befehl, wird diese als Parameter verwendet.

Der erste Befehl in einem Textblock hat eine Zusatzfunktion.

Befehle:

 <  Nach links scrollen. [Parameter gibt die Scrollgeschwindigkeit an]
 >  Nach rechts scrollen. [Parameter gibt die Scrollgeschwindigkeit an]
 u  Nach oben scrollen. [Parameter gibt die Scrollgeschwindigkeit an] 
 d  Nach unten scrollen. [Parameter gibt die Scrollgeschwindigkeit an]

 x  Text an dieser x Position erscheinen lassen.
 y  Text an dieser y Position erscheinen lassen.

 b  Lässt den Text mit angegebener Blinkgeschwindigkeit blinken.
 p  Scrollen stoppen und warten. Parameter gibt die Wartezeit an.

 +  Lässt den nächsten Textblock zusätzlich ausführen.


Erster Befehl
=============
Steht als erstes eine Zahl, so gibt diese an, auf wieviele Textteile die 
nachfolgenden Befehle angewendet werden sollen. D.h. wenn mehrere 
aufeinanderfolgende Textblöcke den gleichen Komandoteil haben, so brauch man 
nun den Komandoteil nur einmal schreiben.

<1|+<30/#LABOR #<1|+<30/#Borg

kann somit durch

2<1|+<30/#LABOR #Borg

abgekürzt werden.

Die Richtungsscrollbefehle < > u d positionieren den Textblock an passender 
Position(so, dass er von aussen rein läuft).


Warte Anweisungen
=================
 p  Scrollen stoppen und warten. Parameter gibt die Wartezeit an.
 /  Warten, bis der Text aus dem Bildschirm heraus ist.
 |  Warten, bis Text in der x Mitte ist, oder bei Parameter bis er an der 
    angegebenen x Position steht.
 -  Warten, bis der Text in y Richtung in der Mitte ist.
 ;  Warten, bis der Block links am VORHERIGEN anschlägt.
 :  Warten, bis der Block rechts am NÄCHSTEN anschlägt.



Beispiele
=========
Erstmal einfach: Einen Text von links nach rechts durchscrollen lassen: 
</#Hello World ! 

Von links und rechts ein Wort herein laufen lassen, dann blinken, warten, 
aufhören zu blinken, und nach unten verschwinden: 
>+:bp10b0d50/#Hallo #<;bp10b0d50/#Labor

ganz abgefahren: 
<5|p2+</#Hallo Labor Borg#16<1|+>10/#L#a#u#f#s#c#h#r#i#f#t#-#B#o#r#g#d50-u50
/d50-/u50->5|80<5|30#www.das-labor.org

 */

#define LINEBYTES 1
#undef NUM_COLS
#define NUM_COLS 8

unsigned char (*text_pixmap)[NUM_ROWS][LINEBYTES];

void text_setpixel(pixel3d p, unsigned char value ){
	if(value){
		(*text_pixmap)[p.y%NUM_ROWS][p.x/8] |= shl_table[p.x%8];
	}
}

void clear_text_pixmap(unsigned char value){
	unsigned char y, z;
	for(y=0;y<NUM_ROWS;y++){
		for(z=0;z<LINEBYTES;z++){
			(*text_pixmap)[y][z] = 0;
		}
	}
}

void update_pixmap(){
	unsigned char y, z, buf, i;
	for(y=0;y<NUM_ROWS;y++){
		for(z=0;z<8;z++){
			buf = (*text_pixmap)[y][0];
			for (i = 0; i < 8; i++) {
				setpixel3d((pixel3d){7-i, i, 7-y}, buf&shl_table[i]?3:0);
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

#define DIRECTION_RIGHT 0x01
#define DIRECTION_DOWN 0x02

struct blob_t_struct;
typedef struct blob_t_struct{
	struct blob_t_struct * next, * last;
	char *str;
	char *commands;
	enum waitfor_e waitfor;
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
	unsigned char * str = (unsigned char*)blob->str;
	
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
	while(*blob->commands != 0) {
		switch (*blob->commands++) {
		case '<':
			blob->direction &= ~DIRECTION_RIGHT;
			if ((tmp = getnum(blob)) != 0xFFFF) { 
				blob->delayx_rld = tmp;
			} else {
				blob->delayx_rld = 10;
			}
			blob->delayx = blob->delayx_rld;
			break;
		case '>':
			blob->direction |= DIRECTION_RIGHT;
			if ((tmp = getnum(blob)) != 0xFFFF) { 
				blob->delayx_rld = tmp;
			} else {
				blob->delayx_rld = 10;
			}
			blob->delayx = blob->delayx_rld;
			break;
		case 'd':
			blob->direction |= DIRECTION_DOWN;
			if ((tmp = getnum(blob)) != 0xFFFF) { 
				blob->delayy_rld = tmp;
			} else {
				blob->delayy_rld = 10;
			}
			blob->delayy = blob->delayy_rld;
			break;
		case 'u':
			blob->direction &= ~DIRECTION_DOWN;
			if ((tmp = getnum(blob)) != 0xFFFF) { 
				blob->delayy_rld = tmp;
			} else {
				blob->delayy_rld = 10;
			}
			blob->delayy = blob->delayy_rld;
			break;
		case 'x'://Place string at this x Position
			if ((tmp = getnum(blob)) != 0xFFFF) { 
				blob->posx = tmp;
			} else {
				blob->posx =  NUM_COLS/2 + blob->sizex/2;
			}
			break;
		case 'y'://Place string at this y Position
			if ((tmp = getnum(blob)) != 0xFFFF) { 
				blob->posy = tmp - blob->sizey;
			}
			break;
		case 'b'://blink blob
			if ((tmp = getnum(blob)) != 0xFFFF) { 
				blob->delayb_rld = tmp;
			} else {
				blob->delayb_rld = 50;
			}
			blob->delayb = blob->delayb_rld;
			break;
		case '|':
			if ((tmp = getnum(blob)) != 0xFFFF) { 
				blob->tox = tmp;
			} else {
				blob->tox =  NUM_COLS/2 + blob->sizex/2;
			}
			blob->waitfor = wait_posx;
			return retval;
			break;
		case '-':
			if ((tmp = getnum(blob)) != 0xFFFF) { 
				blob->toy = tmp;
			} else { 
				blob->toy =  0;
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
	/*char * strtok_r ( char * string, const char * delim, char ** last)*/ 
	static unsigned char chop_cnt;
	static char *last; static char delim[] = "#";
	static char *lastcommands;
	unsigned int tmp;
	
	if(str){
		chop_cnt = 0;
	}
	
	blob_t *blob = malloc(sizeof (blob_t));
	
	if (!chop_cnt) {
		blob->commands = strtok_r (str, delim, &last);
		if ( blob->commands == 0) 
			goto fail;
		
		if ((tmp = getnum(blob)) != 0xFFFF) {
			chop_cnt = tmp;
			lastcommands = blob->commands;
		}
	}
	
	if(chop_cnt){
		chop_cnt--;
		blob->commands = lastcommands;
	}
	
	blob->str = strtok_r(0, delim, &last);

	if (blob->str == 0) goto fail;

	blob->fontIndex = fonts[0].fontIndex;
	blob->fontData = fonts[0].fontData;
	
	unsigned char tmp1;
	char *strg = blob->str;
	unsigned char glyph_beg = fonts[0].glyph_beg;
	unsigned char glyph_end = fonts[0].glyph_end;

	//translate the string: subtract 1 to get offset in Table
	while ((tmp1 = *strg)) {
		if ((tmp1>=glyph_beg) && (tmp1<glyph_end)) {
			*strg = 1 + tmp1 - glyph_beg;
		} else {
			*strg = 1;
		}
		strg++;
	}
	
	blob->space = 1;
	
	blob->sizey = 8;
	blob->sizex = getLen(blob);
	if (*blob->commands == '<') {
		blob->posx = 0;
		blob->posy = 0;
	} else if(*blob->commands == '>') {
		blob->posx = NUM_COLS+blob->sizex;
		blob->posy = 0;
	} else if(*blob->commands == 'd') {
		blob->posy = -blob->sizey;
		blob->posx = NUM_COLS/2 + blob->sizex/2;
	} else if(*blob->commands == 'u') {
		blob->posy = blob->sizey;
		blob->posx = NUM_COLS/2 + blob->sizex/2;
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
	
	if (blob->delayx_rld && (!(blob->delayx--))) {
		blob->delayx = blob->delayx_rld;
		(blob->direction & DIRECTION_RIGHT) ? blob->posx-- : blob->posx++;
	}
	
	if (blob->delayy_rld && (!(blob->delayy--))) {
		blob->delayy = blob->delayy_rld;
		(blob->direction & DIRECTION_DOWN) ? blob->posy++ : blob->posy--;
	}
	
	if (blob->delayb_rld) {
		if (!(blob->delayb--)) {
			blob->delayb = blob->delayb_rld;
			blob->visible ^= 1;
		}
	} else {
		blob->visible = 1;
	}
	
	unsigned char done=0;
	switch (blob->waitfor) {
		case wait_posy:
			if (blob->posy == blob->toy)
				done = 1;
			break;
		case wait_posx:
			if (blob->posx == blob->tox)
				done = 1;
			break;
		case wait_out:
			if ((blob->posx - blob->sizex) > NUM_COLS || blob->posx < 0) 
				done = 1;
			if ((blob->posy) > NUM_ROWS || (blob->posy + blob->sizey) <0 ) 
				done = 1;			
			break;
		case wait_timer:
			if (0 == blob->timer--) {
				done = 1;
			}
			break;
		case wait_col_l:
			if (blob->last) {
				if ((blob->last->posx - blob->last->sizex) == blob->posx) {
					done=1;
				}
			} else {
				done = 1;
			}
			break;
		case wait_col_r:
			if (blob->next) {
				if (blob->next->posx == (blob->posx - blob->sizex)) {
					done=1;
				}
			} else {
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
	
	unsigned int posx; 
	unsigned char posy;
	
	if(!blob->visible) 
		return;
	
	unsigned char * str = (unsigned char*)blob->str;
	posx = blob->posx;
	posy = blob->posy;
	
	glyph = (*blob->str)-1;
	charPos = PW(blob->fontIndex[glyph]);
	charEnd = PW(blob->fontIndex[glyph+1]) -1 ;

	while (posx >= NUM_COLS) {
		if (charPos < charEnd) {                  
			charPos++;
			posx--;
		} else {
			posx -= blob->space + 1;
			if (!(glyph = *++str)) 
				return;      
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
					text_setpixel((pixel3d){x, y, 0},1);
			}
			mask <<= 1;
		}
		
		if (charPos < charEnd) {                  
			charPos++;
		} else {
			x -= blob->space;
			if (!(glyph = *++str)) return;       
			glyph -= 1;   
			charPos = PW(blob->fontIndex[glyph]);
			charEnd = PW(blob->fontIndex[glyph+1]) - 1;
		}
	}
}

extern jmp_buf newmode_jmpbuf;

void scrolltext(char *str) {
	jmp_buf tmp_jmpbuf;
	char tmp_str[SCROLLTEXT_STRING_SIZE];
	int ljmp_retval;

	clear_screen(0);
	
	fonts[0] = font_uni53;
	
	text_pixmap = malloc(NUM_ROWS * LINEBYTES);
	
	memcpy(tmp_str, str, SCROLLTEXT_STRING_SIZE);
	
	blob_t *startblob=0, *aktblob, *nextblob=0;

	memcpy(tmp_jmpbuf, newmode_jmpbuf, sizeof(jmp_buf));
	
	
	if ((ljmp_retval = setjmp(newmode_jmpbuf))) {
		while (startblob) {
			aktblob = startblob;
			startblob = aktblob->next;
			free(aktblob);
		}
		free(text_pixmap);
		memcpy (newmode_jmpbuf, tmp_jmpbuf, sizeof(jmp_buf));
		longjmp(newmode_jmpbuf, ljmp_retval);
	}
	
	if (!(startblob = setupBlob(tmp_str))){
		goto exit;
	}
	
	unsigned char retval;
	do {
		startblob->next = 0;
		startblob->last = 0;
		while (startblob) {
			aktblob = startblob;
			while (aktblob) {
				retval = updateBlob(aktblob);
				if (!retval) {
					nextblob = aktblob->next;
				} else if (retval == 1){
					if (aktblob == startblob){
						startblob = aktblob->next;
					} else {
						aktblob->last->next = aktblob->next;
					}
					if (aktblob->next){
						aktblob->next->last = aktblob->last;
					}
					nextblob = aktblob->next;
					free(aktblob);
			 	} else if (retval == 2){
					blob_t * newblob = setupBlob(0);
					if (newblob){
						newblob->last = aktblob;
						newblob->next = aktblob->next;
						if (aktblob->next){
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
			while (aktblob) {
				drawBlob(aktblob);
				aktblob = aktblob->next;
			}
			update_pixmap();
			wait(2);	
		};
		startblob = setupBlob(0);
		//showBlob(startblob);
	} while (startblob);
	
exit:	
	free(text_pixmap);
	memcpy (newmode_jmpbuf, tmp_jmpbuf, sizeof(jmp_buf));
}
