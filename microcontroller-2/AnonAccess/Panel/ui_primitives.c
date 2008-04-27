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

#define LCD_LABORLOGO     0
#define LCD_RADIOSELECT   5
#define LCD_CHECKSELECT   6
#define LCD_RADIONOSELECT 'o'
#define LCD_CHECKNOSELECT 0xDB

#define LCD_FULLCHAR  255
#define LCD_EMPTYCHAR  ' '
#define LCD_RARROW 0x7E
#define LCD_LARROW 0x7F

#define NO_KEY     ' '
#define ENTER_KEY  'F'
#define SELECT_KEY 'E'
#define UP_KEY     '0'
#define DOWN_KEY   'C'

void ui_primitives_init(void){
	uint8_t vram[8];
	
	lcd_setcgaddr(0);
	lcd_loadfont_P(vcgrom[11]);
	memset(vram, 0x10, 8);
	lcd_loadfont(vram);
	memset(vram, 0x18, 8);
	lcd_loadfont(vram);
	memset(vram, 0x1C, 8);
	lcd_loadfont(vram);
	memset(vram, 0x1E, 8);
	lcd_loadfont(vram);
	lcd_loadfont_P(vcgrom[12]);
	lcd_loadfont_P(vcgrom[13]);
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
	char c=' ';
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
		while((c=read_keypad())==NO_KEY)
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

/******************************************************************************/

uint8_t radioselect_P(PGM_P opts){
	uint8_t i,j,index=0, arrowpos=0;
	uint8_t select=0;
	uint8_t optcount=dbz_strcount_P(opts);
	char c=' ';
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
		while((c=read_keypad())==NO_KEY)
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
		while((c=read_keypad())==NO_KEY)
			;
		_delay_ms(10);
		while(c==read_keypad())
			;
		_delay_ms(10);
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
		while((c=read_keypad())==NO_KEY)
			;
		_delay_ms(10);
		while(c==read_keypad())
			;
		_delay_ms(10);
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
