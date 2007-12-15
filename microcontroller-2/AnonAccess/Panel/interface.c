#include "config.h"
#include "lcd_tools.h"
#include "uart.h"
#include "menu.h"
#include "keypad.h"
#include "rtc.h"
#include "prng.h"
#include "lop.h"
#include "lop_debug.h"
#include <stdint.h>
#include <util/delay.h>
#include "interface.h"

char* status_string = "";


void print_status(void){
	lcd_cls();
	lcd_gotopos(1,1);
	lcd_writestr("=AnonAccess=");
	lcd_writestr(status_string);
}

void waitforkey(char key){
	while(read_keypad()!=key)
		;
	_delay_ms(1);
	while(read_keypad()==key)
		;
	_delay_ms(1);
}

char waitforanykey(void){
	char key;
	while((key=read_keypad())==' ')
		;
	_delay_ms(1);
	while(read_keypad()==key)
		;
	_delay_ms(1);
	return key;
}

void readnhex(uint8_t line, uint8_t x, uint8_t n,char * str){
	uint8_t idx=0;
	char c;
	
	while(idx<n){
		c=waitforanykey();
		lcd_gotopos(line,x+2*idx);
		lcd_writechar(c);
		if(c>='0' && c<='9'){
			c=c-'0';
		} else {
			if(c>='A' && c<='F'){
				c=c-'A'+10;
			} else {
			/* this should not happen */
			 c=0;
			}
		}
		str[idx]=(str[idx] & 0x0F) | (c<<4);
		
		c=waitforanykey();
		lcd_gotopos(line,x+2*idx+1);
		lcd_writechar(c);
		if(c>='0' && c<='9'){
			c=c-'0';
		} else {
			if(c>='A' && c<='F'){
				c=c-'A'+10;
			} else {
			/* this should not happen */
			 c=0;
			}
		}
		str[idx]=(str[idx] & 0xF0) | (c);
		++idx;
	}
}

timestamp_t abstimedifference(timestamp_t t1, timestamp_t t2){
	if(t1<t2){
		return t2-t1;
	} else {
		return t1-t2;
	}
}

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

uint8_t readnstr(uint8_t line, uint8_t x, uint8_t n,char * str){
	timestamp_t time[2]={0,0};
	uint8_t i=0;
	uint8_t idx=0,varidx=0;
	char c[2];
	char ctab[10][6]= {
		{2,' ','0'},
		{2,' ','1'},
		{4,'A','B','C','2'},
		{4,'D','E','F','3'},
		{4,'G','H','I','4'},
		{4,'J','K','L','5'},
		{4,'M','N','O','6'},
		{5,'P','Q','R','S','7'},
		{4,'T','U','V','8'},
		{5,'W','X','Y','Z','9'} };
	lcd_gotopos(line, x);
	
	if(n==0)
		return 0;
	if(n==1){
		str[0]='\0';
		return 0;
	}
	
	do{
		c[0]=waitforanykey();
		time[0]=gettimestamp();
	}while((('0'>c[0]) || ('9'<c[0])) && (c[0]!='E'));
	if(c[0]=='E')
		goto terminate;
	lcd_gotopos(line, x+i);
	str[i]=ctab[c[0]-'0'][idx+1];
	lcd_writechar(str[i]);

 nextscan:	
	varidx ^= 1;	
	do{
		c[varidx]=waitforanykey();
		time[varidx]=gettimestamp();
	}while((('0'>c[varidx]) || ('9'<c[varidx])) && (c[varidx]!='E') &&(c[varidx]!='C'));
	if(c[varidx]=='E')
		goto terminate;
	if(c[varidx]=='C'){
		/* correct the last value */
		if(i>0){
			idx = 0;
			lcd_gotopos(line, x+i);
			str[i]='\0';
			lcd_writechar(' ');
			--i;
		}
		goto nextscan;
	}
	if((c[0]==c[1]) && (abstimedifference(time[0],time[1])<=CHAR_SWITCH_DELAY)){
		/* char modification */
		idx = (idx+1)%ctab[c[0]-'0'][0];
		lcd_gotopos(line, x+i);
		str[i]=ctab[c[0]-'0'][idx+1];
		lcd_writechar(str[i]);
		goto nextscan;
	} else {
		/* next char */
		++i;
		if(i+1>=n)
			goto finalise;
		idx=0;
		lcd_gotopos(line, x+i);
		str[i]=ctab[c[varidx]-'0'][idx+1];
		lcd_writechar(str[i]);
		goto nextscan;
	}
  terminate:
  	++i;
  finalise:	
	str[i]='\0';
	return i;
}

