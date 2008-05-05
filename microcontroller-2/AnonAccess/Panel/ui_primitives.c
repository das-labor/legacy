/*
 * author: Daniel Otte
 * email:  daniel.otte@rub.de
 * 
 * */

#include "config.h"
#include <stdint.h>
#include <string.h> /* memset() */
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "dbz_strings.h"
#include "lcd_tools.h"
#include "keypad.h"
#include "hexdigit_tab.h"
#include "ui_primitives.h"
#include "extrachars.h"
#include "rtc.h"
#include "base64_enc.h"
#include <ctype.h>

#define DEBOUNCE_DELAY _delay_ms(10)

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

/******************************************************************************/	
	
char ui_waitforkeypress(void){
	uint8_t k;
	while((k=read_keypad())==NO_KEY)
		;
	DEBOUNCE_DELAY;
	while(k==read_keypad())
		;
	DEBOUNCE_DELAY;
	return k;
} 

/******************************************************************************/

char ui_waitforkeypresstimed(timestamp_t* tdiff){
	uint8_t k;
	timestamp_t a,b;
	while((k=read_keypad())==NO_KEY)
		;
	a = gettimestamp();	
	DEBOUNCE_DELAY;
	while(k==read_keypad())
		;
	b = gettimestamp();	
	DEBOUNCE_DELAY;
	*tdiff = b-a;
	return k;
} 

/******************************************************************************/

void ui_waitforkey(char key){
	while(key!=ui_waitforkeypress())
		;
}

/******************************************************************************/
/******************************************************************************/

void ui_primitives_init(void){
	uint8_t vram[8];
	
	lcd_setcgaddr(8);
	memset(vram, 0x10, 8); /* load |    */
	lcd_loadfont(vram);
	memset(vram, 0x18, 8); /* load ||   */
	lcd_loadfont(vram);
	memset(vram, 0x1C, 8); /* load |||  */
	lcd_loadfont(vram);
	memset(vram, 0x1E, 8); /* load |||| */
	lcd_loadfont(vram);
	lcd_loadfont_P(vcgrom[12]); /* load radioselect */
	lcd_loadfont_P(vcgrom[13]); /* load checkselect */
	lcd_loadfont_P(vcgrom[11]); /* load labor logo*/
	lcd_setddaddr(0);
}

/******************************************************************************/

void ui_progressbar(double percent, uint8_t xpos, uint8_t ypos, uint8_t width){
	uint8_t full, part, i;
	char str[3];
	if(percent>1) 
		percent=1;
	if(percent<0)
		percent=0;
			
	i = full = ((unsigned)(width*5*percent))/5;
	part = ((unsigned)(width*5*percent))%5;
	lcd_gotopos(ypos, xpos);
	
	while(i--){
		lcd_writechar((char)LCD_FULLCHAR);
	}
	if(part){
		lcd_writechar(part);
	}
	for(i=0; i<width-full-(part)?1:0; ++i)
		lcd_writechar(LCD_EMPTYCHAR);
	if(width>=3 && percent!=1){
		percent *= 100;
		utoa((unsigned)percent, str, 10);
			lcd_gotopos(ypos, xpos+width/2-2);
			lcd_writestr(str);
			lcd_writechar('%');
	}
}

/******************************************************************************/

uint8_t ui_radioselect_core(uint8_t xpos, uint8_t ypos, uint8_t width, uint8_t height,
                            const char* opts, uint8_t flash){
	uint8_t i,j,index=0, arrowpos=0;
	uint8_t select=0;
	
	uint8_t (*count)(const void*);
	void    (*split)(const void*, void**);
	void    (*write)(const void*, uint16_t);
	
	if(flash){
		count = (uint8_t (*)(const void*))dbz_strcount_P;
		split = (void    (*)(const void*, void**))dbz_splitup_P;
		write = (void    (*)(const void*, uint16_t))lcd_writestrn_P;
	} else {
		count = (uint8_t (*)(const void*))dbz_strcount;
		split = (void    (*)(const void*, void**))dbz_splitup;
		write = (void    (*)(const void*, uint16_t))lcd_writestrn;
	}
	
	uint8_t optcount=count(opts);
	char c;
	void* optp[optcount];
	if(optcount==0)
		return 0;
	split(opts, optp);
	for(;;){
		for(i=0; i<((height<optcount)?height:optcount); ++i){
			lcd_gotopos(ypos+i, xpos);
			lcd_writechar((i==arrowpos)?LCD_RARROW:' ');
			lcd_writechar(((i+index)%optcount==select)?LCD_RADIOSELECT:LCD_RADIONOSELECT);
			j=width-2;
			while(--j){
				lcd_writechar(' ');
			}
			lcd_gotopos(ypos+i, xpos+3);
			write(optp[(i+index)%optcount],width-4);
		}
		c=ui_waitforkeypress();
		if(c==ENTER_KEY)
			return select;
		if(c==UP_KEY){
			if(arrowpos==0){
				if(index!=0){
					--index;
				}
			} else {
				--arrowpos;
			}
		}
		if(c==DOWN_KEY){
			if(arrowpos==MIN(height, optcount)-1){
				if(index<optcount-3){
					++index;
				}
			} else {
				++arrowpos;
			}
		}
		if(c==SELECT_KEY){
			select=index+arrowpos;
		}			
	}
}

/******************************************************************************/

uint8_t ui_radioselect(uint8_t xpos, uint8_t ypos, uint8_t width, uint8_t height,
                       const char* opts){
	return ui_radioselect_core(xpos, ypos, width, height, opts, 0);
}

/******************************************************************************/

uint8_t ui_radioselect_P(uint8_t xpos, uint8_t ypos, uint8_t width, uint8_t height,
                         PGM_P opts){
	return ui_radioselect_core(xpos, ypos, width, height, opts, 1);
}

/******************************************************************************/
/******************************************************************************/

static
uint8_t getbit(const uint8_t* buffer, uint8_t pos){
	return (buffer[pos/8])&(1<<(pos%8));
}

/******************************************************************************/

static
void togglebit(uint8_t* buffer, uint8_t pos){
	buffer[pos/8] ^= 1<<(pos%8);
}

/******************************************************************************/

void ui_checkselect_core(uint8_t xpos, uint8_t ypos, uint8_t width, uint8_t height,
                         const char* opts, uint8_t* config, uint8_t flash){
	uint8_t i,j,index=0, arrowpos=0;
	
	uint8_t (*count)(const void*);
	void    (*split)(const void*, void**);
	void    (*write)(const void*, uint16_t);
	
	if(flash){
		count = (uint8_t (*)(const void*))dbz_strcount_P;
		split = (void    (*)(const void*, void**))dbz_splitup_P;
		write = (void    (*)(const void*, uint16_t))lcd_writestrn_P;
	} else {
		count = (uint8_t (*)(const void*))dbz_strcount;
		split = (void    (*)(const void*, void**))dbz_splitup;
		write = (void    (*)(const void*, uint16_t))lcd_writestrn;
	}
	
	uint8_t optcount=count(opts);
	char c=' ';
	void* optp[optcount];
	if(optcount==0)
		return;
	split(opts, optp);
	for(;;){
		for(i=0; i<MIN(optcount,height); ++i){
			lcd_gotopos(ypos+i, xpos);
			lcd_writechar((i==arrowpos)?LCD_RARROW:' ');
			lcd_writechar(getbit(config, (i+index)%optcount)?LCD_CHECKSELECT:LCD_CHECKNOSELECT);
			j=width-2;
			while(--j){
				lcd_writechar(' ');
			}
			lcd_gotopos(ypos+i, xpos+3);
			write(optp[(i+index)%optcount],width-4);
		}
		c = ui_waitforkeypress();
		if(c==ENTER_KEY)
			return;
		if(c==UP_KEY){
			if(arrowpos==0){
				if(index!=0){
					--index;
				}
			} else {
				--arrowpos;
			}
		}
		if(c==DOWN_KEY){
			if(arrowpos==MIN(height, optcount)-1){
					++index;
				if(index<optcount-3){
				}
			} else {
				++arrowpos;
			}
		}
		if(c==SELECT_KEY){
			togglebit(config, index+arrowpos);
		}			
	}
}

/******************************************************************************/

void ui_checkselect(uint8_t xpos, uint8_t ypos, uint8_t width, uint8_t height,
                    const char* opts, uint8_t* config){
	ui_checkselect_core(xpos, ypos, width, height, opts, config, 0);
}

/******************************************************************************/

void ui_checkselect_P(uint8_t xpos, uint8_t ypos, uint8_t width, uint8_t height,
                      PGM_P opts, uint8_t* config){
	ui_checkselect_core(xpos, ypos, width, height, opts, config, 1);
}

/******************************************************************************/
/******************************************************************************/

void ui_drawframe(uint8_t posx, uint8_t posy, uint8_t width, uint8_t height, char framechar){
	uint8_t i;
	/* top line */
	lcd_gotopos(posy,posx);
	for(i=0; i<width; ++i)
		lcd_writechar(framechar);
	/* bottom line */	
	lcd_gotopos(posy+height-1,posx);
	for(i=0; i<width; ++i)
		lcd_writechar(framechar);
	/* now the sides */	
	for(i=posy+1; i<posy+height; ++i){
		lcd_gotopos(i, posx);
		lcd_writechar(framechar);
		lcd_gotopos(i, posx+width-1);
		lcd_writechar(framechar);
	}
}

/******************************************************************************/
/******************************************************************************/

void ui_menuexec(menu_t* menu){
	uint8_t n=0;
	while(pgm_read_byte(&(menu[n].options))!=terminator)
		++n;
  reset:
  	;
	uint8_t i,idx=0,selpos=2;
  redraw:
  //	lcd_cls();
  	ui_printstatusline();
	
	for(i=0; i<((n<3)?n:3); ++i){
		lcd_gotopos(i+2,2);
		lcd_writestr_P((PGM_P)(pgm_read_word(&(menu[(idx+i)%n].name))));
		lcd_gotopos(i+2,20);
		switch(pgm_read_byte(&(menu[(idx+i)%n].options))){
			case autosubmenu:
			case submenu: lcd_writechar(LCD_RARROW);
				break;
			case execute: lcd_writechar('*');
				break;
			case back: lcd_writechar(LCD_LARROW);
				break;
			case none: 
			default:
				break;
				
		}
	}
	lcd_gotopos(selpos,1);
	lcd_writechar(LCD_RARROW);
		
  rescan:	
	switch (ui_waitforkeypress()){
		case UP_KEY: 
			if(selpos==2){
				idx = (idx-1+n)%n;
			} else {
				selpos--;
			}
			goto redraw;
			break;		
		case DOWN_KEY:
			if(selpos==4){
				idx = (idx+1+n)%n;
			} else {
				selpos++;
			}
			goto redraw;
			break;		
		case ENTER_KEY:	
		case SELECT_KEY:
			if(pgm_read_byte(&(menu[(idx+selpos-2)%n].options))==autosubmenu){
				ui_menuexec((menu_t*)pgm_read_word(&(menu[(idx+selpos-2)%n].x)));
			} else {
				if(pgm_read_word(&(menu[(idx+selpos-2)%n].x))!=0){
					((void(*)(void))(pgm_read_word(&(menu[(idx+selpos-2)%n].x))))();
				}else{
					return;
				}
			}
			goto reset;
			break;
		default: goto rescan; 
			break;
	}
}

/******************************************************************************/
/******************************************************************************/
static
void genaddr(uint16_t value, char* str, uint8_t len){
	str+=len-1;
	while(len--){
		*str--= pgm_read_byte(hexdigit_tab_P+(value&0x0F));
		value >>=4;
	}
}

/******************************************************************************/
static
void data2hex(const void* buffer, char* dest, uint8_t length){
	while(length--){
		*dest++ = pgm_read_byte(hexdigit_tab_P+((*(uint8_t*)buffer)>>4));
		*dest++ = pgm_read_byte(hexdigit_tab_P+((*(uint8_t*)buffer)&0xf));
		buffer = (uint8_t*)buffer + 1;
	}
}

/******************************************************************************/
static
void data2hex_P(PGM_VOID_P buffer, char* dest, uint8_t length){
	uint8_t t;
	while(length--){
		t=pgm_read_byte(buffer);
		*dest++ = pgm_read_byte(hexdigit_tab_P+(t>>4));
		*dest++ = pgm_read_byte(hexdigit_tab_P+(t&0xf));
		buffer = (uint8_t*)buffer + 1;
	}
}

/******************************************************************************/

void ui_hexdump_core(uint8_t xpos, uint8_t ypos, uint8_t width, uint8_t height,
                     const void* data, uint16_t length, uint8_t flash){
	uint16_t offset=0;
	uint8_t addr_len=4;
	uint8_t bytesperline=0;
	uint8_t i,j;
	char c;
	void (*d2h)(const void*, char*, uint8_t);
	
	if(flash){
		d2h = (void (*)(const void*, char*, uint8_t))data2hex_P;
	} else {
		d2h = (void (*)(const void*, char*, uint8_t))data2hex;
	}
	
	while((length&(0xF<<(addr_len*4-4)))==0)
		--addr_len;
	char addrstr[addr_len];
	
	bytesperline=(width-addr_len-1)/2;
	char bytesbuffer[bytesperline*2];
	
	for(;;){
		for(i=0; i<height; ++i){
			if(offset+i*bytesperline>length)
				break;
			lcd_gotopos(ypos+i,xpos);
			genaddr(offset+i*bytesperline, addrstr, addr_len);
			lcd_writestrn(addrstr, addr_len);
			lcd_writechar(':');
			if(length-offset-i*bytesperline>=bytesperline){
				/* regular case */
				d2h(((uint8_t*)data)+offset+i*bytesperline,bytesbuffer, bytesperline);
				lcd_writestrn(bytesbuffer, 2*bytesperline);	
			} else {
				/* only a few bytes are to print */
				j=length-offset-i*bytesperline;
				d2h(((uint8_t*)data)+offset+i*bytesperline,bytesbuffer, j);
				lcd_writestrn(bytesbuffer, 2*j);
				j=width-addr_len-1-2*j;
				while(j--)
					lcd_writechar(' ');
			}
		}
		c=ui_waitforkeypress();
		if(c==UP_KEY){
			if(offset)
				offset-=bytesperline;
		}
		if(c==DOWN_KEY){
			if(offset<((signed)length)-height*(signed)bytesperline)
				offset+=bytesperline;
		}
		if(c==ENTER_KEY || c==SELECT_KEY){
			return;
		}
	}
}
/******************************************************************************/

void ui_hexdump(uint8_t xpos, uint8_t ypos, uint8_t width, uint8_t height,
                const void* data, uint16_t length){
	ui_hexdump_core(xpos, ypos, width, height, data, length, 0);
}

/******************************************************************************/

void ui_hexdump_P(uint8_t xpos, uint8_t ypos, uint8_t width, uint8_t height,
                  PGM_VOID_P data, uint16_t length){
	ui_hexdump_core(xpos, ypos, width, height, data, length, 1);
}

/******************************************************************************/
/******************************************************************************/

void lcd_writelinen(const char* text, uint16_t length){
	while(*text && *text!='\n' && *text!='\r' && length--)
		lcd_writechar(*text++);
}

/******************************************************************************/

void lcd_writelinen_P(PGM_P text, uint16_t length){
	char c;
	
	c=pgm_read_byte(text);
	while(c && c!='\n' && length--){
		lcd_writechar(c);
		c=pgm_read_byte(++text);
	}
}
/******************************************************************************/

void lcd_writeB64(void* data, uint16_t length){
	char str[1+(length*4+3)/3]; /* length/3*4 */
	
	base64enc(str, data, length);
	lcd_writestr(str);
}

/******************************************************************************/

void lcd_writeB64long(uint8_t xpos, uint8_t ypos, uint8_t width, void* data, uint16_t length){
	uint8_t bytesperline;
	bytesperline = (width/4)*3;
	for(; length>=bytesperline; length-=bytesperline){
		lcd_gotopos(ypos++, xpos);
		lcd_writeB64((uint8_t*)data, bytesperline);
		data = (uint8_t*)data + bytesperline;
		if(ypos>LCD_HEIGHT)
			return;
	}
	if(length){
		if(ypos>LCD_HEIGHT)
			return;
		lcd_gotopos(ypos, xpos);
		lcd_writeB64((uint8_t*)data, length);
		
	}
}

/******************************************************************************/
static
uint16_t count_lcdstrings_P(PGM_P text, uint8_t width){
	uint16_t ret=1;
	uint8_t t=0;
	char c;
	c=pgm_read_byte(text);
	if(!c)
		return 0;
	while(c){
		if(c=='\n' || t==width){
			t=0;
			++ret;
		}else{
			++t;
		}
		c=pgm_read_byte(++text);
	}
	return ret;
}
/******************************************************************************/
static
uint16_t count_lcdstrings(const char* text, uint8_t width){
	uint16_t ret=1;
	uint8_t t=0;
	if(!*text)
		return 0;
	while(*text){
		if(*text=='\n' || t==width){
			t=0;
			++ret;
		}else{
			++t;
		}
		++text;
	}
	return ret;
}

/******************************************************************************/
static
void split_lcdstrings_P(PGM_P text, PGM_P* list, uint8_t width){
	uint8_t t=0;
	char c;
	*list = text;
	c=pgm_read_byte(text);
	while(c){
		if(c=='\n' || t==width){
			t=0;
			++list;
			*list=text+1;
		}else{
			++t;
		}
		c=pgm_read_byte(++text);
	}
}

/******************************************************************************/
static
void split_lcdstrings(const char* text, char** list, uint8_t width){
	uint8_t t=0;
	*list = (void*)((uint16_t)text); /* with this little trick the compiler can be keept quite about the const stuff */
	while(*text){
		if(*text=='\n' || t==width){
			t=0;
			++list;
			*list=(char*)((uint16_t)text)+1;
		}else{
			++t;
		}
		++text;
	}
}

/******************************************************************************/

void ui_textwindow_core(uint8_t posx, uint8_t posy, uint8_t width, uint8_t height, const void* text, uint8_t flash){
	uint16_t strn;
	uint16_t offset=0;
	uint8_t i,j;
	char c;
	uint16_t (*count)(const void*, uint8_t);
	void (*split)(const void*, void**, uint8_t);
	void (*writeln)(const void*, uint16_t);
	
	if(flash){
		count = (uint16_t (*)(const void*, uint8_t))    count_lcdstrings_P;
		split = (void (*)(const void*, void**, uint8_t))split_lcdstrings_P;
		writeln = (void (*)(const void*, uint16_t))     lcd_writelinen_P;
	}else{
		count = (uint16_t (*)(const void*, uint8_t))    count_lcdstrings;
		split = (void (*)(const void*, void**, uint8_t))split_lcdstrings;
		writeln = (void (*)(const void*, uint16_t))     lcd_writelinen;
	}
	
	if(height==0 || width==0)
		return;
	strn=count(text, width-1);
	if(strn==0)
		return;
	void* l[strn];
	split(text, l, width-1);
	
	for(;;){
		for(i=0; i<height; ++i){
			lcd_gotopos(posy+i,posx); 	
			for(j=0; j<width; ++j)
				lcd_writechar(' ');
			lcd_gotopos(posy+i,posx);
			writeln(l[offset+i],width);	
		}
		c = ui_waitforkeypress();
		if(c==ENTER_KEY || c==SELECT_KEY){
			return;
		}
		if(c==UP_KEY){
			if(offset)
				--offset;
		}
		if(c==DOWN_KEY){
			if((signed)offset<(signed)strn-height)
				++offset;
		}
	}
}
/******************************************************************************/

void ui_textwindow(uint8_t posx, uint8_t posy, uint8_t width, uint8_t height, char* text){
	ui_textwindow_core(posx,posy,width,height, text, 0);
}

/******************************************************************************/

void ui_textwindow_P(uint8_t posx, uint8_t posy, uint8_t width, uint8_t height, PGM_P text){
	ui_textwindow_core(posx,posy,width, height, text, 1);
}

/******************************************************************************/
/******************************************************************************/

uint8_t read_decimaln(uint8_t xpos, uint8_t ypos, char* str, uint8_t n){
	uint8_t idx=0;
	char c;
	LCD_CURSOR_ON;
	for(;;){
		lcd_gotopos(ypos,xpos+idx);
		lcd_writechar(' ');
		lcd_gotopos(ypos,xpos+idx);
		c=ui_waitforkeypress();
		if(c>='0' && c<='9' && idx<n){
			lcd_writechar(c);
			str[idx]=c;
			++idx;
		}else{
			if(c==ENTER_KEY || c==SELECT_KEY){
				break;
			}
			if(c==CORRECT_KEY){
				if(idx)
					--idx;
			}
		}
	}
	str[idx]='\0';
	LCD_CURSOR_OFF;
	return idx;
}

/******************************************************************************/

uint8_t read_pinn(uint8_t xpos, uint8_t ypos, char disp,char* str, uint8_t n){
	uint8_t idx=0;
	char c;
	if(disp!=0)
		LCD_CURSOR_ON;
	for(;;){
		lcd_gotopos(ypos,xpos+idx);
		lcd_writechar(' ');
		lcd_gotopos(ypos,xpos+idx);
		c=ui_waitforkeypress();
		if(c>='0' && c<='9' && idx<n){
			lcd_writechar(disp);
			str[idx]=c;
			++idx;
		}else{
			if(c==ENTER_KEY || c==SELECT_KEY){
				break;
			}
			if(c==CORRECT_KEY){
				if(idx)
					--idx;
			}
		}
	}
	str[idx]='\0';
	LCD_CURSOR_OFF;
	return idx;
}

/******************************************************************************/
#define KEYPAD_SPECIALDELAY 1500 /* milliseconds */

uint8_t read_hexn(uint8_t xpos, uint8_t ypos, char* str, uint8_t n){
	uint8_t idx=0;
	char c;
	timestamp_t td;
	LCD_CURSOR_ON;
	for(;;){
		lcd_gotopos(ypos,xpos+idx);
		lcd_writechar(' ');
		lcd_gotopos(ypos,xpos+idx);
		c=ui_waitforkeypresstimed(&td);
		if(((c>='0'&&c<='9')|| (c>='A'&&c<='F'&&td<KEYPAD_SPECIALDELAY)) && idx<n){
			lcd_writechar(c);
			str[idx]=c;
			++idx;
		}else{
			if((c==ENTER_KEY || c==SELECT_KEY)&& td>=KEYPAD_SPECIALDELAY){
				break;
			}
			if(c==CORRECT_KEY && td>=KEYPAD_SPECIALDELAY){
				if(idx)
					--idx;
			}
		}
	}
	str[idx]='\0';
	LCD_CURSOR_OFF;
	return idx;	
}

/******************************************************************************/

/*
 0: ' '0
 1: ' '1
 2: ABC2
 3: DEF3
 4: GHI4
 5: JKL5
 6: MNO6
 7: PQRS7
 8: TUV8
 9: WXYZ9

*/

#define CHAR_SWITCH_DELAY 1000 /* 1 sec */

/******************************************************************************/

uint8_t read_strn(uint8_t xpos, uint8_t ypos, PGM_P charset, char * str, uint8_t n){
	timestamp_t time[2]={0,0};
	uint8_t idx=0,varidx=0;
	char c[2]={0,0};
	uint8_t toggle=0;
	uint8_t charsetn;
	uint8_t tabidx=100;

	charsetn=dbz_strcount_P(charset);
	PGM_P ctab[charsetn];
	dbz_splitup_P(charset, ctab);
	
	if(n==0)
		return 0;
	if(n==1){
		str[0]='\0';
		return 0;
	}
	
	LCD_CURSOR_ON;
	for(;;){
	go_on:
		toggle ^= 1;
		lcd_gotopos(ypos, xpos+idx);
		lcd_writechar(' ');
		lcd_gotopos(ypos, xpos+idx);
		c[toggle]=ui_waitforkeypress();
		time[toggle]=gettimestamp();
		if((c[0]==c[1]) && 
		   (tabidx<charsetn) && 
		   (abstimedifference(time[0],time[1])<=CHAR_SWITCH_DELAY)){
			/* we should just modify the char */
			char x;
			varidx += 1;
			varidx %= strlen_P(ctab[tabidx]);
			x = pgm_read_byte((ctab[tabidx])+varidx);
			lcd_gotopos(ypos, xpos+idx-1);
			lcd_writechar(x);
			str[idx-1]=x;
		}else{
			if(c[toggle]==ENTER_KEY || c[toggle]==SELECT_KEY){
				/* terminate */
				str[idx]='\0';
				LCD_CURSOR_OFF;
				return idx;
			}
			if(c[toggle]==CORRECT_KEY){
				tabidx=100;
				if(idx)
					--idx;
			}else{
				/* "normal" keypress */
				tabidx=100;
				/* 1) translate key code in table index */
				if('0'<=c[toggle] && '9'>=c[toggle])
					tabidx = c[toggle] -'0';
				if(c[toggle]=='A')
					tabidx = 10;
				if(c[toggle]=='B')
					tabidx = 11;
				if(c[toggle]=='D')
					tabidx = 12;
				/* 2) check if we have an entry for that */
				if(tabidx>=charsetn)
					goto go_on;
				if(pgm_read_byte(ctab[tabidx])==1)
					goto go_on;
				if(idx==n)
					goto go_on;
				/* 3) set the value and display it */
				str[idx]=pgm_read_byte(ctab[tabidx]);
				lcd_writechar(str[idx]);
				idx++;
				varidx=0;		
			}
		}
	}
	
}

/******************************************************************************/

char ui_statusstring[6] = "     ";

void ui_printstatusline(void){
	lcd_cls();
	lcd_gotopos(1,1);
	lcd_writestr("=AnonAccess=");
	lcd_writestr(ui_statusstring);
}

/******************************************************************************/

void ui_loginit(ui_loglist_t* log, uint8_t size){
	uint8_t i;
	
	log->free = NULL;
	log->idx = 0;
	while(size && !(log->entrys=malloc(size*sizeof(ui_loglist_entry_t))))
		size--;
	log->entrysn=size;
	if(!size)
		return;
	for(i=0; i<size; ++i){
		log->entrys[i].storage_type=none_st;
		log->entrys[i].str = NULL;
	}
}

/******************************************************************************/

void ui_logrm(ui_loglist_t* log){
	log->idx = ((log->idx)?log->idx:log->entrysn)-1;
	switch(log->entrys[log->idx].storage_type){
		case copy_st:
		case heap_st:
			free(log->entrys[log->idx].str); break;
		case ram_st:
		    if(log->free)
		    	log->free(&(log->entrys[log->idx]));
		    break;
		default:
			break;
	}
}

/******************************************************************************/

void ui_logfree(ui_loglist_t* log){
	uint8_t i;
	for(i=0; i<log->entrysn; ++i){
		ui_logrm(log);
	}
	free(log->entrys);
}

/******************************************************************************/

uint16_t stringprintlen(char* str){
	uint16_t i=0;
	while(isprint(str[i]))
		++i;
	return i;
}

/******************************************************************************/

uint8_t ui_logappend(ui_loglist_t* log, void* str, storage_type_t st, state_t state){
	if(log->entrys[log->idx].storage_type!=none_st){
		ui_logrm(log);
	}
	if(st==copy_st){
		uint16_t len=stringprintlen((char*)str);
		if(!(log->entrys[log->idx].str=malloc(len+1)))
			return 1;
		memcpy(log->entrys[log->idx].str, str, len);
		((char*)(log->entrys[log->idx].str))[len]='\0';
	}else{
		log->entrys[log->idx].str = str;
	}
	log->entrys[log->idx].storage_type = st;
	log->entrys[log->idx].state = state;
	log->idx++;
	if(log->idx==log->entrysn)
		log->idx=0;
	return 0;
}

/******************************************************************************/

void ui_logchangelaststate(ui_loglist_t* log, state_t state){
	uint8_t idx;
	idx=log->idx;
	idx=(idx)?idx-1:log->entrysn-1;
	log->entrys[idx].state = state;
}

/******************************************************************************/

ui_loglist_entry_t* ui_logget(ui_loglist_t* log, uint8_t index){
	uint16_t idx;

	idx=(log->idx - index + log->entrysn - 1)%log->entrysn;
	return &(log->entrys[idx]);
}

/******************************************************************************/

uint8_t ui_logactcount(ui_loglist_t* log){
	uint8_t i=0;
	while(i<log->entrysn && log->entrys[i].storage_type!=none_st)
		++i;
	return i;
}

/******************************************************************************/

uint8_t ui_logstate_char[] PROGMEM = 
	{UI_SUCCESS_CHAR, UI_FAILED_CHAR, UI_PROCESSING_CHAR, UI_INFORMATIVE_CHAR};

/******************************************************************************/

void ui_logprintx(uint8_t xpos, uint8_t ypos, uint8_t width, uint8_t height,
                  ui_loglist_t* log, uint8_t offset){
	uint8_t i;
	int8_t j;
	ui_loglist_entry_t* e;
	
	for(i=0; i<height; ++i){
		lcd_gotopos(ypos+i, xpos);
		e = ui_logget(log, height-i+offset-1);
		switch(e->storage_type){
			case copy_st:
			case heap_st:
			case stack_st:
			case ram_st:
				lcd_writestrn(e->str, width-1);
				j=width-strlen(e->str)-1;
				break;
			case flash_st:
				lcd_writestrn_P(e->str, width-1);
				j=width-strlen_P(e->str)-1;
				break;
			case none_st:
			default:
				j=width; break;
		}
		while(j-->0)
			lcd_writechar(' ');
		if(e->storage_type!=none_st){
			lcd_writechar(pgm_read_byte(ui_logstate_char+e->state));
		}
	}

}
/******************************************************************************/

void ui_logprint(uint8_t xpos, uint8_t ypos, uint8_t width, uint8_t height,
                 ui_loglist_t* log){
	ui_logprintx(xpos, ypos, width, height, log, 0);
}

/******************************************************************************/

void ui_logreader(uint8_t xpos, uint8_t ypos, uint8_t width, uint8_t height,
                 ui_loglist_t* log){
	uint8_t offset=0, max;         
	char key;
	
	max = ui_logactcount(log);
	if(height>max){
		max=0;
	}else{
		max=max-height;
	}
	for(;;){
		ui_logprintx(xpos, ypos, width, height, log, offset);
		key = ui_waitforkeypress();
		if(key==ENTER_KEY || key==SELECT_KEY){
			return;
		}
		if(key==UP_KEY){
			if(offset<max)
			offset++;
		}
		if(key==DOWN_KEY){
			if(offset)
				offset--;
		}
	}
}	
/******************************************************************************/

int8_t ui_keyortimeout(uint16_t timeout){
	char key;
	while(timeout--){
		if((key=read_keypad())!=NO_KEY){
			DEBOUNCE_DELAY;
			while(read_keypad()==key)
				;
			DEBOUNCE_DELAY;
			return key;
		}
		_delay_ms(1);
	}
	return -1;
}








