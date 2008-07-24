/* factorizefun.c */
/*
 *   This file is part of AnonAccess, an access system which can be used
 *    to open door or doing other things with an anonymity featured
 *    account managment.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/*
 * author: Daniel Otte 
 * email:  daniel.otte@rub.de
 * license: GPLv3
 * factorizefun - a small exiting factorizing game
 */
 
#include "config.h"
#include "lcd_tools.h"
#include "keypad.h"
#include "keypad_charset.h"
#include "ui_primitives.h"
#include "entropium.h"
#include "hexdigit_tab.h"
#include "rtc.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#define HIGHSCORES_N 10

typedef struct{
	uint64_t score;
	char name[8];
}highscore_entry_t;

highscore_entry_t highscore_table[HIGHSCORES_N] EEMEM; // = {{0,0}};

uint8_t prime_lut[] PROGMEM = {
        0x6E, 0xCB, 0xB4, 0x64, 0x9A, 0x12, 0x6D, 0x81, 
        0x32, 0x4C, 0x4A, 0x86, 0x0D, 0x82, 0x96, 0x21
};

static
uint8_t is8bitprime(uint8_t x){
	if(x==2)
		return 1;
	if((x&1)==0)
		return 0;
	x>>=1;
	return pgm_read_byte(prime_lut+x/8)&(1<<(x%8));
}

static
uint8_t is16bitprime(uint16_t x){
	if((x&1)==0)
		return 0;
	if((x&0xFF00)==0)
		return(is8bitprime((uint8_t)x));
	uint8_t t,i,j,v;
	for(i=0; i<16; ++i){
		t=pgm_read_byte(prime_lut+i);
		v=i<<4;
		v|=1;
		for(j=0; j<8; ++j){
			if(t&1){
				if(x%v==0)
					return 0;
			}
			v+=2;
			t>>=1;
		}
	}
	return 1;
}

#define INVALID ((uint8_t)-1)
#define VALID (1)

static
uint8_t polynom_formcheck(char* polynom){
	uint8_t bases=0;
	enum pol_check_state {start, base, pow, exp, mul} state=start;
	while(*polynom){
		if(isdigit(*polynom)){
			polynom++;
			if(state==start || state==mul){
				bases++;
				state=base;
			}
			if(state==pow)
				state=exp;	
		} else {
			if(*polynom=='*'){
				switch(state){
					case base: state=mul; break;
					case mul:  state=pow; break;
					case start:
					case pow: 
					default:
						return INVALID;
				}
				polynom++;
			}else{
				return INVALID;
			} 
		}		
	}
	if(state==mul || state==pow || state==start || bases==0)
		return INVALID;
	return bases;
}

static
void polynom_splitup(uint16_t *b, uint8_t *e, char *p){
	uint16_t temp_i=0;
	uint8_t index=0;
	uint8_t expf=0;
	
	while(*p){
		if(isdigit(*p)){
			temp_i = temp_i*10 + ((*p)-'0');
			p++;
		} else {
			/* *p == '*' */
			if(*(p+1)=='*'){
				expf=1;
				b[index]=temp_i;
				p+=2;
			} else {
				if(expf){
					e[index]=(uint8_t)temp_i;
					expf=0;
				} else {
					b[index]=temp_i;
					e[index]=(uint8_t)1;
				}
				index++;
				p++;
			}
			temp_i = 0;
		}
	}	
	if(expf){
		e[index]=temp_i;
		expf=0;
	} else {
		b[index]=temp_i;
		e[index]=1;
	}

} 

static
uint8_t check_polynom(uint32_t challange, char* polynom){
	uint8_t i,j,base_cnt;
	base_cnt=polynom_formcheck(polynom);
	if(base_cnt==INVALID)
		return INVALID;
	
	uint16_t bases[base_cnt];
	uint8_t exponents[base_cnt];
	polynom_splitup(bases, exponents, polynom);
	/* chek if bases are prime */
	for(i=0; i<base_cnt; ++i){
		if(is16bitprime(bases[i])==0){
			return INVALID;
		}
	}
	uint32_t a=1,b;
	for(i=0; i<base_cnt; ++i){
		b=bases[i];
		for(j=1; j<exponents[i]; ++j){
			b*=bases[i];
		}
		a*=b;
	}
	if(a==challange)
		return VALID;
	return INVALID;
}


static
uint64_t play_factorize_level(uint8_t level, uint64_t points){
	uint32_t challange;
	uint8_t  n;
	char str[LCD_WIDTH+2];
	timestamp_t t1,t2;
	entropium_fillBlockRandom(&challange, 4);
	challange |= 1;
	challange &= (1<<(level+3))-1;
	challange |= 1<<(level+3);
	ultoa(challange, str, 10);
	
	lcd_cls();
	lcd_gotopos(1,LCD_WIDTH-16);
	lcd_hexdump(&points, 8);
	lcd_gotopos(2,1);
	lcd_writestr_P(PSTR("Factorize:"));	
	lcd_gotopos(3, LCD_WIDTH-strlen(str)-2);
	lcd_writestr(str);
	t1=gettimestamp();
	read_strn(1,4, polynom_cs, str, LCD_WIDTH);
	t2=gettimestamp();
	lcd_cls();
	lcd_gotopos(2,1);
	lcd_writestr_P(PSTR("checking ..."));	
	n=check_polynom(challange, str);
	if(n==INVALID)
		return 0;
	t2 -= t1;
	uint64_t res;
	res=(challange*1000*level)/t2;
	return res;
}

static
uint8_t rate_score(uint64_t score){
	uint8_t i;
	uint64_t refscore;
	for(i=HIGHSCORES_N; i>0; --i){
		eeprom_read_block(&refscore, &(highscore_table[i-1].score), 8);
		if(score<refscore)
			return i;
	}
	return i;
}

static
void insert_score(char* name, uint64_t score, uint8_t position){
	highscore_entry_t buffer;
	int8_t i;
	for(i=HIGHSCORES_N-2; i>= position; --i){
		eeprom_read_block(&buffer, &(highscore_table[i]), 16);
		eeprom_write_block(&(highscore_table[i+1]), &buffer, 16);
	}
	memset(buffer.name, ' ', 8);
	strncpy(buffer.name, name, 8);
	if(strlen(name)<8){
		buffer.name[strlen(name)]=' ';
	}
	buffer.score=score;
	eeprom_write_block(&(highscore_table[i+1]), &buffer, 16);
}

void factorize_showhighscore(void){
	char* text;
	uint8_t i,j;
	uint16_t k;
	uint64_t t;
	if(!(text=malloc((LCD_WIDTH+1)*(HIGHSCORES_N+1)+2))){
		return;
	}
	/* print highscore in textbuffer */
	memset(text, ' ', (LCD_WIDTH+1)*(HIGHSCORES_N+1));
	text[(LCD_WIDTH+1)*(HIGHSCORES_N+1)] = 
	  text[(LCD_WIDTH+1)*(HIGHSCORES_N+1)+1] = '\0';
	memcpy_P(text+2,PSTR("factorize HIGHSCORE"), 19);
	for(i=1; i<=HIGHSCORES_N; ++i){
		eeprom_read_block(text+LCD_WIDTH*i, highscore_table[i-1].name, 8);
		eeprom_read_block(&t, &(highscore_table[i-1].score), 8);
		k=(LCD_WIDTH)*(i)+10;
		//data2hex(&t, text+k, 8);
		for(j=0; j<16; j++){
			text[k++] = pgm_read_byte(hexdigit_tab_P+(t&0x0f));
			t>>=4;
		}
	}
	ui_textwindow(1,1,LCD_WIDTH,LCD_HEIGHT, text);
	free(text);
}

void factorize_play(void){
	uint64_t t,points=0;
	uint8_t i=0;
	while((t=play_factorize_level(i, points)) && (i<29)){
		points+=t;
		++i;
		lcd_cls();
		lcd_gotopos(2, 5);
		lcd_writestr_P(PSTR("next level"));
		ui_waitforkey('F');
	}
	lcd_cls();
	if(t==0){
		lcd_gotopos(2,LCD_WIDTH/2-4);
		lcd_writestr_P(PSTR("GAME OVER"));
	}else{
		lcd_gotopos(2,LCD_WIDTH/2-3);
		lcd_writestr_P(PSTR("YOU WON"));
	}
	lcd_gotopos(3,1);
	lcd_writestr_P(PSTR("Points:"));
	lcd_gotopos(4,LCD_WIDTH/2-8);
	lcd_hexdump(&points, 8);
	ui_waitforkey('F');
	uint8_t n;
	n=rate_score(points);
	if(n<HIGHSCORES_N){
		char str[9];
		lcd_cls();
		lcd_gotopos(1,LCD_WIDTH/2-7);
		lcd_writestr_P(PSTR("New HIGHSCORE"));
		lcd_gotopos(2, 2);
		lcd_writestr_P(PSTR("enter name:"));
		read_strn(2,3, alphanumext_cs, str, 8);
		str[8]='\0';
		insert_score(str, points, n);
	}
	lcd_cls();
}
