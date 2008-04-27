#include "config.h"
#include "lcd_tools.h"
#include "uart.h"
#include "menu.h"
#include "keypad.h"
#include "ui_primitives.h"
#include "rtc.h"
#include "prng.h"
#include "lop.h"
#include "lop_debug.h"
#include "interface.h"
#include "base64_enc.h"
#include "reset_counter.h"
#include "types.h"
#include "comm.h"
#include "24C04.h"
#include "cardio.h"
#include <stdint.h>
#include <util/delay.h>

extern lop_ctx_t lop0;

authblock_t ab;

/******************************************************************************/

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
void dump_card(void);
void write_card(void);
void display_analysis(void);


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
/*
void run_serial_test(void){
	char tmp,tmp2;
	timestamp_t tsend;
	uint64_t ok=0,failed=0,lost=0;
	void (*backup)(uint8_t);
	
	backup = uart_hook;
	uart_hook=NULL;
	print_status();
	
	while(read_keypad()!='C'){
#ifdef UART_XON_XOFF
		do{
			tmp=getRandomByte();
		}while(tmp==0x11 || tmp==0x13);
#else
		tmp=getRandomByte();	
#endif		
		
		uart_putc(tmp);
		tsend = gettimestamp();
		while((!uart_getc_nb(&tmp2)) && ((gettimestamp()-tsend)<TIMEOUT_VAL)){
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
*/

#define BUFFERSIZE 100

void run_serial_test(void){
	char tmp,tmp2, bufferout[BUFFERSIZE], bufferin[BUFFERSIZE];
	timestamp_t tsend;
	uint8_t idxin,idxout;
	uint64_t ok=0,failed=0,lost=0;
	void (*backup)(uint8_t);
	
	backup = uart_hook;
	uart_hook=NULL;
	print_status();
	
	while(read_keypad()!='C'){
#ifdef UART_XON_XOFF
		for(idxout=0; idxout<BUFFERSIZE; ++idxout){
			do{
				tmp=getRandomByte();
			}while(tmp==0x11 || tmp==0x13);
			bufferout[idxout]=tmp;
		}
#else
		fillBlockRandom(bufferout, BUFFERSIZE);
#endif			
		/* blast it out */
		for(idxout=0; idxout<BUFFERSIZE; ++idxout){
			uart_putc(bufferout[idxout]);
		}
		tsend = gettimestamp();
		idxin=0;
		while((idxin<BUFFERSIZE) && (!uart_getc_nb(&tmp2)) && ((gettimestamp()-tsend)<TIMEOUT_VAL)){
			bufferin[idxin++]=tmp2;
		}
		if(gettimestamp()-tsend<TIMEOUT_VAL){
			if(!memcmp(bufferin, bufferout, BUFFERSIZE)){
				ok++;
			} else {
				failed++;
			}
		} else {
			lost++;
		}
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
const char dump_card_PS[] PROGMEM = "dump ICC";
const char write_card_PS[] PROGMEM = "AB -> ICC";
const char display_analysis_PS[] PROGMEM = "display analysis";

menu_t debug_menu_mt[] = {
	{main_menu_PS, back, {NULL}},
	{serial_test_PS, execute, {run_serial_test}},
	{reset_PS, execute, {print_resets}},
	{timestamp_PS, execute, {print_timestamp}},
	{timestamp_live_PS, execute, {print_timestamp_live}},
	{timestamp_base64_PS, execute, {print_timestamp_base64}},
	{timestamp_base64_live_PS, execute, {print_timestamp_base64_live}},
	{random_PS, execute, {print_random}},
	{get_name_PS, execute, {demo_getname}},
	{get_hex_string_PS, execute, {demo_hex}},
	{dump_card_PS, execute, {dump_card}},
	{write_card_PS, execute, {write_card}},
	{display_analysis_PS, execute, {display_analysis}},
	{NULL, terminator, {NULL}}
};

/******************************************************************************/

const char req_AB_PS[]    PROGMEM = "request AB";
const char view_AB_PS[]   PROGMEM = "view AB";

menu_t bootstrap_menu_mt[] = {
	{main_menu_PS, back, {NULL}},
	{req_AB_PS, execute, {req_authblock}},
	{view_AB_PS, execute, {view_authblock}},
	{NULL, terminator, {NULL}}
};

const char open_door_PS[]      PROGMEM = "open door";
const char lock_door_PS[]      PROGMEM = "lock door";
const char admin_menu_PS[]     PROGMEM = "admin menu";
const char statistic_menu_PS[] PROGMEM = "statistic menu";
const char bootstrap_menu_PS[] PROGMEM = "bootstrap menu";
const char debug_menu_PS[]     PROGMEM = "debug menu";
menu_t main_menu_mt[] = {
	{open_door_PS,execute, {open_door}},
	{lock_door_PS,execute, {lock_door}},
	{admin_menu_PS,submenu, {admin_menu}},
	{statistic_menu_PS,submenu, {stat_menu}},
	{bootstrap_menu_PS,autosubmenu, {bootstrap_menu_mt}},
	{debug_menu_PS,autosubmenu, {debug_menu_mt}},
	{NULL, terminator, {NULL}}
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
	menuexec(bootstrap_menu_mt);
}

void debug_menu(void){
	menuexec(debug_menu_mt);
}

void master_menu(void){
	menuexec(main_menu_mt);
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

void replace_unprinable(char * str, uint16_t len){
	while(len--){
		if((*str>='A' && *str<='Z') || 
		   (*str>='a' && *str<='z') ||
		   (*str>='0' && *str<='9') ||
		   (*str==' ')){
		   	;
		}else{
			*str='.';
		}
		str++;
	}
}

void dump_card(void){
	uint8_t buffer[17];
	uint16_t i;
//	E24C04_block_read(0xA0, 0, buffer, 16);
	buffer[16]='\0';
	lop_dbg_str_P(&lop0, PSTR("\r\nICC dump:\r\n"));
	E24C04_init();
	for(i=0; i<256; i+=16){
		E24C04_block_read(0xA0, i, buffer, 16);
		lop_dbg_hexdump(&lop0, buffer, 16);
		lop_dbg_str_P(&lop0, PSTR("   "));
		replace_unprinable((char*)buffer, 16);
		lop_dbg_str(&lop0, (char*)buffer);
		lop_dbg_str_P(&lop0, PSTR("\r\n"));
	}
}

void write_card(void){
	writeABtoCard(&ab);
}

void display_analysis(void){
	uint16_t offset=0,i;
	char c;
	lcd_cls();
	lcd_gotopos(1,1);
	lcd_writestr_P(PSTR("-=display analysis=-"));
	lcd_gotopos(2,2);
	lcd_writestr_P(PSTR("position:"));
	while((c=read_keypad())!='C'){
		switch (c){
			case '0': if(offset>0) --offset; break;
			case '3': if(offset<256-LCD_WIDTH) ++offset; break;
			default: break;
		}
		if(c!=' '){
			lcd_gotopos(3,4);
			lcd_hexdump(&offset, 1);
			lcd_gotopos(4,1);
			for(i=offset; i<offset+LCD_WIDTH; ++i){
				lcd_writechar((char)i);
			}
			_delay_ms(3);
		}
	}
}







