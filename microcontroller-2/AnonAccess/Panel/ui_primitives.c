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
#include "lcd_tools.h"
#include "keypad.h"

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

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
}};

void ui_primitives_init(void){
	uint8_t vram[8];
	uint8_t i;
	
	lcd_setcgaddr(0);
	for(i=0; i<8; ++i){
		vram[i]=pgm_read_byte(&(vcgrom[11][i]));
	}
	lcd_loadfont(vram);
	memset(vram, 0x10, 8);
	lcd_loadfont(vram);
	memset(vram, 0x18, 8);
	lcd_loadfont(vram);
	memset(vram, 0x1C, 8);
	lcd_loadfont(vram);
	memset(vram, 0x1E, 8);
	lcd_loadfont(vram);
	lcd_setddaddr(0);
}

#define LCD_FULLCHAR  255
#define LCD_EMPTYCHAR  ' '
#define LCD_RARROW 0x7E
#define LCD_LARROW 0x7F


#define ENTER_KEY  'F'
#define SELECT_KEY 'E'
#define UP_KEY     '0'
#define DOWN_KEY   'C'



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

static
uint8_t dbz_strcount(char* str){
	uint8_t ret=1;
	if(*str=='\0')
			return 0;
	for(;;){
		while(*str++)
			;
		if(*str=='\0')
			return ret;
		++ret;
	}	
}

/******************************************************************************/

static
void dbz_splitup(char* dbzstr, char** strings){
	if(*dbzstr=='\0')
		return;
	*strings++ = dbzstr;
	for(;;){	
		while(*dbzstr++)
			;
		if(*dbzstr=='\0')
			return;
		*strings++ = dbzstr;
	}
}

/******************************************************************************/

uint8_t radioselect(char* opts){
	uint8_t i,index=0, arrowpos=0;
	uint8_t select=0;
	uint8_t optcount=dbz_strcount(opts);
	char c=' ';
	char* optp[optcount];
	if(optcount==0)
		return 0;
	dbz_splitup(opts, optp);
	for(;;){
		for(i=0; i<((3<optcount)?3:optcount); ++i){
			lcd_gotopos(2+i, 1);
			lcd_writechar((i==arrowpos)?LCD_RARROW:' ');
			lcd_writestr_P(PSTR("( )                "));
			if((i+index)%optcount==select){
				lcd_gotopos(2+i, 3);
				lcd_writechar('x');
			}
			lcd_gotopos(2+i, 6);
			lcd_writestrn(optp[(i+index)%optcount],LCD_WIDTH-6);
		}
		while((c=read_keypad())==' ')
			;
		_delay_ms(10);
		while(c==read_keypad())
			;
		_delay_ms(10);
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

