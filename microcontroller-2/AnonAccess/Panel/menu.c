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

extern lop_ctx_t lop0;

/******************************************************************************/

typedef struct{
	char *name;
	enum {none, submenu, execute, back} options;
	void (*fkt)(void);
}menu_t;

/******************************************************************************/

void menuexec(uint8_t n, menu_t* menu);
void open_door(void);
void lock_door(void);
void admin_menu(void);
void stat_menu(void);
void bootstrap_menu(void);
void debug_menu(void);

void print_timestamp(void);
void print_random(void);

/******************************************************************************/

char* status_string = "";


void print_status(void){
	lcd_cls();
	lcd_gotopos(1,1);
	lcd_writetext("=AnonAccess=");
	lcd_writetext(status_string);
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
	}while((('0'>c[varidx]) || ('9'<c[varidx])) && (c[varidx]!='E'));
	if(c[1]=='E')
		goto terminate;
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

void demo_getname(void){
	char name[11];
	print_status();
	lcd_gotopos(2,1);
	lcd_writetext("name:");
	readnstr(3,1,10,name);
	lcd_gotopos(4,1);
	lcd_writetext(name);
	waitforkey('E');
}

void demo_hex(void){
	char name[10];
	print_status();
	lcd_gotopos(2,1);
	lcd_writetext("hex:");
	readnhex(3,1,10,name);
	lcd_gotopos(4,1);
	lcd_hexdump(name, 10);
	waitforkey('E');
}


menu_t main_menu_mt[6] = {
	{"open door",execute, open_door},
	{"lock door",execute, lock_door},
	{"admin menu",submenu, admin_menu},
	{"statistic menu",submenu, stat_menu},
	{"bootstrap menu",submenu, bootstrap_menu},
	{"debug menu",submenu, debug_menu}
};

menu_t debug_menu_mt[5] = {
	{"main menu", back, NULL},
	{"timestamp", execute, print_timestamp},
	{"random (30)", execute, print_random},
	{"get name", execute, demo_getname},
	{"get hex string", execute, demo_hex}
};

void open_door(void){}
void lock_door(void){}
void admin_menu(void){}
void stat_menu(void){}
void bootstrap_menu(void){}

void debug_menu(void){
	menuexec(5, debug_menu_mt);
}

void print_timestamp(void){
	timestamp_t t;
	t = gettimestamp();
	print_status();
	lcd_gotopos(2,1);
	lcd_hexdump(&t,8);
	waitforkey('E');
}

void print_random(void){
	uint8_t block[30];
	fillBlockRandom(block, 30);
	print_status();
	lcd_gotopos(2,1);
	lcd_hexdump(&block,10);
	lcd_gotopos(3,1);
	lcd_hexdump(&block+10,10);
	lcd_gotopos(4,1);
	lcd_hexdump(&block+20,10);
	waitforkey('E');
}


#define R_ARROW 0x7E
#define L_ARROW 0x7F

void menuexec(uint8_t n, menu_t* menu){
  reset:
  	;
	uint8_t i,idx=0,selpos=2;
  redraw:
  	print_status();
	
	for(i=0; i<((n<3)?n:3); ++i){
		lcd_gotopos(i+2,2);
		lcd_writetext(menu[(idx+i)%n].name);
		lcd_gotopos(i+2,20);
		switch(menu[(idx+i)%n].options){
			case submenu: lcd_writechar(R_ARROW);
				break;
			case execute: lcd_writechar('*');
				break;
			case back: lcd_writechar(L_ARROW);
				break;
			case none: 
			default:
				break;
				
		}
	}
	lcd_gotopos(selpos,1);
	lcd_writechar(R_ARROW);
		
  rescan:	
	switch (read_keypad()){
		case '0': 
			if(selpos==2){
				idx = (n+idx-1)%n;
			} else {
				selpos--;
			}
			_delay_ms(1);
			while(read_keypad()=='0')
				;
			_delay_ms(1);
			goto redraw;
			break;		
		case 'C':
			if(selpos==4){
				idx = (n+idx+1)%n;
			} else {
				selpos++;
			}
			_delay_ms(1);
			while(read_keypad()=='C')
				;
			_delay_ms(1);
			goto redraw;
			break;		
		case 'E':
			_delay_ms(1);
			while(read_keypad()=='E')
				;
			_delay_ms(1);
			if(menu[(idx+selpos-2)%n].fkt!=NULL){
				menu[(idx+selpos-2)%n].fkt();
			}else{
				return;
			}
			goto reset;
			break;
		default: goto rescan; 
			break;
	}
}

void master_menu(void){
	menuexec(6, main_menu_mt);
}






