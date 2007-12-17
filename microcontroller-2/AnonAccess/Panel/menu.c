#include "config.h"
#include "lcd_tools.h"
#include "uart.h"
#include "menu.h"
#include "keypad.h"
#include "rtc.h"
#include "prng.h"
#include "lop.h"
#include "lop_debug.h"
#include "interface.h"
#include "base64_enc.h"
#include "reset_counter.h"
#include "types.h"
#include "comm.h"
#include <stdint.h>
#include <util/delay.h>

extern lop_ctx_t lop0;

authblock_t ab;

/******************************************************************************/

typedef struct{
	PGM_P name;                                   /* 2 byte */
	enum {none, submenu, execute, back} options;  /* 1 byte*/
	void (*fkt)(void);                            /* 2 byte */
} menu_t;


/******************************************************************************/

void menuexec(uint8_t n, menu_t* menu);
void open_door(void);
void lock_door(void);
void admin_menu(void);
void stat_menu(void);
void bootstrap_menu(void);
void debug_menu(void);

void print_resets(void);
void print_timestamp(void);
void print_timestamp_live(void);
void print_timestamp_base64(void);
void print_timestamp_base64_live(void);
void run_serial_test(void);
void print_random(void);

/******************************************************************************/


void demo_getname(void){
	char name[11];
	print_status();
	lcd_gotopos(2,1);
	lcd_writestr_P(PSTR("name:"));
	readnstr(3,1,10,name);
	lcd_gotopos(4,1);
	lcd_writestr(name);
	waitforkey('E');
}

void demo_hex(void){
	char name[10];
	print_status();
	lcd_gotopos(2,1);
	lcd_writestr_P(PSTR("hex:"));
	readnhex(3,1,10,name);
	lcd_gotopos(4,1);
	lcd_hexdump(name, 10);
	waitforkey('E');
}

#include "uart.h"

#define TIMEOUT_VAL 1000

#define DBG(a) lcd_gotopos(1,15); lcd_writechar(a)

void run_serial_test(void){
	char tmp,tmp2;
	timestamp_t tsend;
	uint64_t ok=0,failed=0,lost=0;
	void (*backup)(uint8_t);
	
	backup = uart_hook;
	uart_hook=NULL;
	print_status();
	
	while(read_keypad()!='C'){
		tmp=getRandomByte();
		_delay_ms(100);
		DBG('1');
		uart_putc(tmp);
		DBG('2');
		tsend = gettimestamp();
		DBG('3');
		while((!uart_getc_nb(&tmp2)) && ((gettimestamp()-tsend)<TIMEOUT_VAL)){
			DBG('4');
		}
		if(gettimestamp()-tsend<TIMEOUT_VAL){
			if(tmp==tmp2){
				ok++;
			} else {
				failed++;
			}
		} else {
			lost++;
		}
		DBG('5');
		lcd_gotopos(2,2);
		lcd_hexdump(&ok, 8);
		lcd_gotopos(3,2);
		lcd_hexdump(&failed, 8);
		lcd_gotopos(4,2);
		lcd_hexdump(&lost, 8);
	}
	waitforkey('E');
	uart_hook = backup;
}


void req_authblock(void){
	char name[12];
	print_status();
	init_session();
	lcd_gotopos(2,1);
	lcd_writestr_P(PSTR("name:"));
	readnstr(3,1,10,name);
	req_bootab(name, 0);
	status_string[4]='~';
}


void view_authblock(void){
	char str[45];
	
	print_status();
	lcd_gotopos(2,1);
	lcd_writestr_P(PSTR("uid: "));
	lcd_hexdump(&(ab.uid), 2);
	waitforkey('E');
	
	print_status();
	lcd_gotopos(2,1);
	lcd_writestr_P(PSTR("ticket: "));
	base64enc(str, ab.ticket, 32);
	lcd_gotopos(2,16);
	lcd_writestrn(&(str[0]),4);
	lcd_gotopos(3,1);
	lcd_writestrn(&(str[4]),20);
	lcd_gotopos(4,1);
	lcd_writestrn(&(str[24]),20);
	waitforkey('E');
	
	print_status();
	lcd_gotopos(2,1);
	lcd_writestr_P(PSTR("rkey: "));
	base64enc(str, ab.rkey, 32);
	lcd_gotopos(2,16);
	lcd_writestrn(&(str[0]),4);
	lcd_gotopos(3,1);
	lcd_writestrn(&(str[4]),20);
	lcd_gotopos(4,1);
	lcd_writestrn(&(str[24]),20);
	waitforkey('E');
	
	print_status();
	lcd_gotopos(2,1);
	lcd_writestr_P(PSTR("rid: "));
	base64enc(str, ab.rid, 32);
	lcd_gotopos(2,16);
	lcd_writestrn(&(str[0]),4);
	lcd_gotopos(3,1);
	lcd_writestrn(&(str[4]),20);
	lcd_gotopos(4,1);
	lcd_writestrn(&(str[24]),20);
	waitforkey('E');
	
	print_status();
	lcd_gotopos(2,1);
	lcd_writestr_P(PSTR("pinhmac: "));
	base64enc(str, ab.pinhmac, 32);
	lcd_gotopos(2,16);
	lcd_writestrn(&(str[0]),4);
	lcd_gotopos(3,1);
	lcd_writestrn(&(str[4]),20);
	lcd_gotopos(4,1);
	lcd_writestrn(&(str[24]),20);
	waitforkey('E');
	
	print_status();
	lcd_gotopos(2,1);
	lcd_writestr_P(PSTR("HMAC: "));
	base64enc(str, ab.hmac, 32);
	lcd_gotopos(2,16);
	lcd_writestrn(&(str[0]),4);
	lcd_gotopos(3,1);
	lcd_writestrn(&(str[4]),20);
	lcd_gotopos(4,1);
	lcd_writestrn(&(str[24]),20);
	waitforkey('E');
}

/******************************************************************************/
/******************************************************************************/

const char open_door_PS[]      PROGMEM = "open door";
const char lock_door_PS[]      PROGMEM = "lock door";
const char admin_menu_PS[]     PROGMEM = "admin menu";
const char statistic_menu_PS[] PROGMEM = "statistic menu";
const char bootstrap_menu_PS[] PROGMEM = "bootstrap menu";
const char debug_menu_PS[]     PROGMEM = "debug menu";
menu_t main_menu_mt[6] = {
	{open_door_PS,execute, open_door},
	{lock_door_PS,execute, lock_door},
	{admin_menu_PS,submenu, admin_menu},
	{statistic_menu_PS,submenu, stat_menu},
	{bootstrap_menu_PS,submenu, bootstrap_menu},
	{debug_menu_PS,submenu, debug_menu}
};

/******************************************************************************/

const char main_menu_PS[]      PROGMEM = "main menu";
const char serial_test_PS[]    PROGMEM = "test serial loop";
const char reset_PS[]          PROGMEM = "print resets";
const char timestamp_PS[]      PROGMEM = "timestamp";
const char timestamp_live_PS[]      PROGMEM = "timestamp (live)";
const char timestamp_base64_PS[]    PROGMEM = "timestamp (B64)";
const char timestamp_base64_live_PS[]      PROGMEM = "timestamp (l,B64)";
const char random_PS[]         PROGMEM = "random (30)";
const char get_name_PS[]       PROGMEM = "get name";
const char get_hex_string_PS[] PROGMEM = "get hex string";

menu_t debug_menu_mt[10] = {
	{main_menu_PS, back, NULL},
	{serial_test_PS, execute, run_serial_test},
	{reset_PS, execute, print_resets},
	{timestamp_PS, execute, print_timestamp},
	{timestamp_live_PS, execute, print_timestamp_live},
	{timestamp_base64_PS, execute, print_timestamp_base64},
	{timestamp_base64_live_PS, execute, print_timestamp_base64_live},
	{random_PS, execute, print_random},
	{get_name_PS, execute, demo_getname},
	{get_hex_string_PS, execute, demo_hex}
};

/******************************************************************************/

const char req_AB_PS[]    PROGMEM = "request AB";
const char view_AB_PS[]   PROGMEM = "view AB";

menu_t bootstrap_menu_mt[3] = {
	{main_menu_PS, back, NULL},
	{req_AB_PS, execute, req_authblock},
	{view_AB_PS, execute, view_authblock}
};

/******************************************************************************/
/******************************************************************************/

void open_door(void){
	init_session();
	submit_ab(&ab);
	send_mainopen();
}

void lock_door(void){
	init_session();
	submit_ab(&ab);
	send_mainclose();
}

void admin_menu(void){}
void stat_menu(void){}

void print_resets(void){
	uint64_t t;
	t = resetcnt_read();
	print_status();
	lcd_gotopos(2,1);
	lcd_hexdump(&t,8);
	waitforkey('E');
}

void bootstrap_menu(void){
	menuexec(3, bootstrap_menu_mt);
}

void debug_menu(void){
	menuexec(10, debug_menu_mt);
}

void master_menu(void){
	menuexec(6, main_menu_mt);
}

void print_timestamp(void){
	timestamp_t t;
	t = gettimestamp();
	print_status();
	lcd_gotopos(2,1);
	lcd_hexdump(&t,8);
	waitforkey('E');
}

void print_timestamp_live(void){
	timestamp_t t;
	print_status();	
	while(read_keypad()!='E'){
		t = gettimestamp();
		lcd_gotopos(2,1);
		lcd_hexdump(&t,8);
	}
	while(read_keypad()=='E')
		;
	_delay_ms(1);
}

void print_timestamp_base64(void){
	timestamp_t t;
	char str[(sizeof(timestamp_t)+2)/3*4+1]; /* should make 13 */
	
	t = gettimestamp();
	base64enc(str,&t,sizeof(timestamp_t));
	print_status();
	lcd_gotopos(2,1);
	lcd_writestr(str);
	waitforkey('E');
}

void print_timestamp_base64_live(void){
	timestamp_t t;
	char str[(sizeof(timestamp_t)+2)/3*4+1]; /* should make 13 */
	print_status();	
	while(read_keypad()!='E'){
		t = gettimestamp();
		base64enc(str,&t,sizeof(timestamp_t));
//		str[((sizeof(timestamp_t)*8)+5)/6] = '\0';

		lcd_gotopos(2,1);
		lcd_writestr(str);
	}
	while(read_keypad()=='E')
		;
	_delay_ms(1);
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
		lcd_writestr_P(menu[(idx+i)%n].name);
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







