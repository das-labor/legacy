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

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

#define DEBOUNCE_DELAY _delay_ms(10)

uint8_t vcgrom[][8] PROGMEM = {
{	 /* 0 */
	0x0E,  /*  ###  */
    0x11,  /* #   # */
    0x13,  /* #  ## */
    0x15,  /* # # # */
    0x19,  /* ##  # */
    0x11,  /* #   # */
    0x0E,  /*  ###  */
    0x00   /*       */
}, { /* 1 */
	0x04,  /*   #   */
    0x0C,  /*  ##   */
    0x04,  /*   #   */
    0x04,  /*   #   */
    0x04,  /*   #   */
    0x04,  /*   #   */
    0x0E,  /*  ###  */
    0x00   /*       */	
}, { /* 2 */
	0x0E,  /*  ###  */
    0x11,  /* #   # */
    0x01,  /*     # */
    0x02,  /*    #  */
    0x04,  /*   #   */
    0x08,  /*  #    */
    0x1F,  /* ##### */
    0x00   /*       */	
}, { /* 3 */
	0x1F,  /* ##### */
    0x02,  /*    #  */
    0x04,  /*   #   */
    0x02,  /*    #  */
    0x01,  /*     # */
    0x11,  /* #   # */
    0x0E,  /*  ###  */
    0x00   /*       */	
}, { /* 4 */
	0x02,  /*    #  */
    0x06,  /*   ##  */
    0x0A,  /*  # #  */
    0x12,  /* #  #  */
    0x1F,  /* ##### */
    0x02,  /*    #  */
    0x02,  /*    #  */
    0x00   /*       */	
}, { /* 5 */
	0x1F,  /* ##### */
    0x10,  /* #     */
    0x1E,  /* ####  */
    0x01,  /*     # */
    0x01,  /*     # */
    0x11,  /* #   # */
    0x0E,  /*  ###  */
    0x00   /*       */	
}, { /* 6 */
	0x06,  /*   ##  */
    0x08,  /*  #    */
    0x10,  /* #     */
    0x1E,  /* ####  */
    0x11,  /* #   # */
    0x11,  /* #   # */
    0x0E,  /*  ###  */
    0x00   /*       */	
}, { /* 7 */
	0x1F,  /* ##### */
    0x11,  /* #   # */
    0x01,  /*     # */
    0x02,  /*    #  */
    0x04,  /*   #   */
    0x04,  /*   #   */
    0x04,  /*   #   */
    0x00   /*       */	
}, { /* 8 */
	0x0E,  /*  ###  */
    0x11,  /* #   # */
    0x11,  /* #   # */
	0x0E,  /*  ###  */
    0x11,  /* #   # */
    0x11,  /* #   # */
	0x0E,  /*  ###  */
    0x00   /*       */	
}, { /* 9 */
	0x0E,  /*  ###  */
    0x11,  /* #   # */
    0x11,  /* #   # */
	0x0E,  /*  #### */
    0x11,  /*     # */
    0x11,  /*    #  */
	0x0E,  /*  ##   */
    0x00   /*       */	
}, { /* % */
	0x18,  /* ##    */
    0x19,  /* ##  # */
    0x02,  /*    #  */
	0x04,  /*   #   */
    0x08,  /*  #    */
    0x13,  /* #  ## */
	0x03,  /*    ## */
    0x00   /*       */	
}, { /* labor logo :-) */
	0x0E,  /*  ###  */
    0x11,  /* #   # */
    0x15,  /* # # # */
    0x11,  /* #   # */
    0x15,  /* # # # */
    0x15,  /* # # # */
    0x11,  /* #   # */
    0x0E   /*  ###  */
}, { /* radio selected */
    0x00,  /*       */
    0x00,  /*       */
    0x0E,  /*  ###  */
    0x11,  /* #   # */
    0x15,  /* # # # */
    0x11,  /* #   # */
    0x0E,  /*  ###  */ 
    0x00   /*       */
}, { /* check selected */
    0x00,  /*       */
    0x1F,  /* ##### */
    0x11,  /* #   # */
    0x1B,  /* ## ## */
    0x15,  /* # # # */
    0x1B,  /* ## ## */
    0x1F,  /* ##### */
    0x00,  /*       */
}};

/******************************************************************************/

char waitforkeypress(void){
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


void print_progressbar(double percent, uint8_t xpos, uint8_t ypos, uint8_t width){
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

uint8_t radioselect(char* opts){
	uint8_t i,j,index=0, arrowpos=0;
	uint8_t select=0;
	uint8_t optcount=dbz_strcount(opts);
	char c;
	char* optp[optcount];
	if(optcount==0)
		return 0;
	dbz_splitup(opts, optp);
	for(;;){
		for(i=0; i<((3<optcount)?3:optcount); ++i){
			lcd_gotopos(2+i, 1);
			lcd_writechar((i==arrowpos)?LCD_RARROW:' ');
			lcd_writechar(((i+index)%optcount==select)?LCD_RADIOSELECT:LCD_RADIONOSELECT);
			j=LCD_WIDTH-2;
			while(--j){
				lcd_writechar(' ');
			}
			lcd_gotopos(2+i, 4);
			lcd_writestrn(optp[(i+index)%optcount],LCD_WIDTH-4);
		}
		c=waitforkeypress();
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
			if(arrowpos==MIN(2, optcount-1)){
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

uint8_t radioselect_P(PGM_P opts){
	uint8_t i,j,index=0, arrowpos=0;
	uint8_t select=0;
	uint8_t optcount=dbz_strcount_P(opts);
	char c;
	PGM_P optp[optcount];
	if(optcount==0)
		return 0;
	dbz_splitup_P(opts, optp);
	for(;;){
		for(i=0; i<((3<optcount)?3:optcount); ++i){
			lcd_gotopos(2+i, 1);
			lcd_writechar((i==arrowpos)?LCD_RARROW:' ');
			lcd_writechar(((i+index)%optcount==select)?LCD_RADIOSELECT:LCD_RADIONOSELECT);
			j=LCD_WIDTH-2;
			while(--j){
				lcd_writechar(' ');
			}
			lcd_gotopos(2+i, 4);
			lcd_writestrn_P(optp[(i+index)%optcount],LCD_WIDTH-4);
		}
		c = waitforkeypress();
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
			if(arrowpos==MIN(2, optcount-1)){
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
static
uint8_t getbit(const uint8_t* buffer, uint8_t pos){
	return (buffer[pos/8])&(1<<(pos%8));
}

static
void togglebit(uint8_t* buffer, uint8_t pos){
	buffer[pos/8] ^= 1<<(pos%8);
}
/******************************************************************************/

void checkselect(char* opts, uint8_t* config){
	uint8_t i,j,index=0, arrowpos=0;
	uint8_t optcount=dbz_strcount(opts);
	char c=' ';
	char* optp[optcount];
	if(optcount==0)
		return;
	dbz_splitup(opts, optp);
	for(;;){
		for(i=0; i<((3<optcount)?3:optcount); ++i){
			lcd_gotopos(2+i, 1);
			lcd_writechar((i==arrowpos)?LCD_RARROW:' ');
			lcd_writechar(getbit(config, (i+index)%optcount)?LCD_CHECKSELECT:LCD_CHECKNOSELECT);
			j=LCD_WIDTH-2;
			while(--j){
				lcd_writechar(' ');
			}
			lcd_gotopos(2+i, 4);
			lcd_writestrn(optp[(i+index)%optcount],LCD_WIDTH-4);
		}
		c = waitforkeypress();
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
			if(arrowpos==MIN(2, optcount-1)){
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

void checkselect_P(PGM_P opts, uint8_t* config){
	uint8_t i,j,index=0, arrowpos=0;
	uint8_t optcount=dbz_strcount_P(opts);
	char c;
	PGM_P optp[optcount];
	if(optcount==0)
		return;
	dbz_splitup_P(opts, optp);
	for(;;){
		for(i=0; i<((3<optcount)?3:optcount); ++i){
			lcd_gotopos(2+i, 1);
			lcd_writechar((i==arrowpos)?LCD_RARROW:' ');
			lcd_writechar(getbit(config, (i+index)%optcount)?LCD_CHECKSELECT:LCD_CHECKNOSELECT);
			j=LCD_WIDTH-2;
			while(--j){
				lcd_writechar(' ');
			}
			lcd_gotopos(2+i, 4);
			lcd_writestrn_P(optp[(i+index)%optcount],LCD_WIDTH-4);
		}
		c = waitforkeypress();
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
			if(arrowpos==MIN(2, optcount-1)){
				if(index<optcount-3){
					++index;
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

void draw_frame(uint8_t posx, uint8_t posy, uint8_t width, uint8_t height, char framechar){
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

void menuexec(menu_t* menu){
	uint8_t n=0;
	while(menu[n].options!=terminator)
		++n;
  reset:
  	;
	uint8_t i,idx=0,selpos=2;
  redraw:
  	lcd_cls();
  //	print_status();
	
	for(i=0; i<((n<3)?n:3); ++i){
		lcd_gotopos(i+2,2);
		lcd_writestr_P(menu[(idx+i)%n].name);
		lcd_gotopos(i+2,20);
		switch(menu[(idx+i)%n].options){
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
	switch (waitforkeypress()){
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
			if(menu[(idx+selpos-2)%n].options==autosubmenu){
				menuexec((menu_t*)menu[(idx+selpos-2)%n].x.submenu);
			} else {
				if(menu[(idx+selpos-2)%n].x.fkt!=NULL){
					(menu[(idx+selpos-2)%n].x.fkt)();
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

void genaddr(uint16_t value, char* str, uint8_t len){
	str+=len-1;
	while(len--){
		*str--= pgm_read_byte(hexdigit_tab_P+(value&0x0F));
		value >>=4;
	}
}

void data2hex(const void* buffer, char* dest, uint8_t length){
	while(length--){
		*dest++ = pgm_read_byte(hexdigit_tab_P+((*(uint8_t*)buffer)>>4));
		*dest++ = pgm_read_byte(hexdigit_tab_P+((*(uint8_t*)buffer)&0xf));
		buffer = (uint8_t*)buffer + 1;
	}
}

void ui_hexdump(const void* data, uint16_t length){
	uint16_t offset=0;
	uint8_t addr_len=4;
	uint8_t bytesperline=0;
	uint8_t i,j;
	char c;
	while((length&(0xF<<(addr_len*4-4)))==0)
		--addr_len;
	char addrstr[addr_len];
	
	bytesperline=(LCD_WIDTH-addr_len-1)/2;
	char bytesbuffer[bytesperline*2];
	
	for(;;){
		for(i=0; i<3; ++i){
			if(offset+i*bytesperline>length)
				break;
			lcd_gotopos(2+i,1);
			genaddr(offset+i*bytesperline, addrstr, addr_len);
			lcd_writestrn(addrstr, addr_len);
			lcd_writechar(':');
			if((j=length-offset-i*bytesperline)>=bytesperline){
				/* regular case */
				data2hex(((uint8_t*)data)+offset+i*bytesperline,bytesbuffer, bytesperline);
				lcd_writestrn(bytesbuffer, 2*bytesperline);	
			} else {
				/* only a few bytes are to print (j) */
				data2hex(((uint8_t*)data)+offset+i*bytesperline,bytesbuffer, j);
				lcd_writestrn(bytesbuffer, 2*j);
				j=LCD_WIDTH-addr_len-1-2*j;
				while(j--)
					lcd_writechar(' ');
			}
		}
		c=waitforkeypress();
		if(c==UP_KEY){
			if(offset)
				offset-=bytesperline;
		}
		if(c==DOWN_KEY){
			if(offset<((signed)length)-3*(signed)bytesperline)
				offset+=bytesperline;
		}
		if(c==ENTER_KEY || c==SELECT_KEY){
			return;
		}
	}
}

/******************************************************************************/
void lcd_writelinen(const char* text, uint16_t length){
	while(*text && *text!='\n' && length--)
		lcd_writechar(*text++);
}

uint16_t count_lcdstrings(char* text, uint8_t width){
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

void split_lcdstrings(char* text, char** list, uint8_t width){
	uint8_t t=0;
	*list = text;
	while(*text){
		if(*text=='\n' || t==width){
			t=0;
			++list;
			*list=text+1;
		}else{
			++t;
		}
		++text;
	}
}

void ui_textwindow(uint8_t posx, uint8_t posy, uint8_t width, uint8_t height, char* text){
	uint16_t strn;
	uint16_t offset=0;
	uint8_t i,j;
	char c;
	
	if(height==0 || width==0)
		return;
	strn=count_lcdstrings(text, width-1);
	if(strn==0)
		return;
	char* l[strn];
	split_lcdstrings(text, l, width-1);
	
	for(;;){
		for(i=0; i<height; ++i){
			lcd_gotopos(posy+i,posx); 	
			for(j=0; j<width; ++j)
				lcd_writechar(' ');
			lcd_gotopos(posy+i,posx);
			lcd_writelinen(l[offset+i],width);	
		}
		c = waitforkeypress();
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

