
#include "prng.h"
#include "ticketDB.h"
#include "flmDB.h"
#include "action.h"
#include "uart.h"
#include "sha256.h"
#include "hmac-sha256.h"
#include "types.h"
#include "reqvalidator.h"
#include <avr/eeprom.h>
#include "keys.h"
#include <avr/io.h>
#include "i2c_printer.h"

/*
#define DS(a)   uart_putstr_P(PSTR(a))
#define DD(a,b) uart_hexdump((a),(b))
*/
#define DS(a) {uart_putstr_P(PSTR(a)); printer_str_P(PSTR(a));} 
#define DC(a) {uart_putc(a); printer_char(a);} 
#define DD(a,b) {uart_hexdump((a),(b)); printer_hexdump((a),(b));} 


/******************************************************************************/
void door_init(void){
	DDRA = 0xD0;
	PORTA = 0xD0;
}

void main_open(void){
	PORTA &= ~0x60;
	PORTA |=  0x80;
	DS("\r\n door opening");
}

void main_close(void){
	PORTA &= ~0x80;
	PORTA |=  0x60;
	DS("\r\n door closing");
}

void dump_authblock(authblock_t * ab){
	DS("\r\n +++ authblock +++");
	DS("\r\n   UID:    "); DD((char*)&(ab->uid),  2);
	DS("\r\n   RID:    "); DD((char*)&(ab->rid), 32);
	DS("\r\n   RKey:   "); DD((char*)&(ab->rkey), 32);
	DS("\r\n   Ticket: "); DD((char*)&(ab->ticket), 32);
	DS("\r\n   HMAC:   "); DD((char*)&(ab->hmac), 32);
}

void add_user(char * nickname, sha256_hash_t pinhash,uint8_t anon, uint8_t pinflags, authblock_t *dest){
	new_account(dest, nickname, pinhash, anon, pinflags);
//	dump_authblock(&ab);
}

void rem_user(char * nickname){
	userflags_t set, clear;
	
	set.admin = set.exist = set.locked = set.notify_lostadmin = 0;
	clear.admin = clear.exist = clear.locked = clear.notify_lostadmin = 0;
	clear.exist = 1;
	modify_account(nickname, set, clear);
}

void lock_user(char * nickname){
	userflags_t set, clear;
	
	set.admin = set.exist = set.locked = set.notify_lostadmin = 0;
	clear.admin = clear.exist = clear.locked = clear.notify_lostadmin = 0;
	set.locked = 1;
	modify_account(nickname, set, clear);
}

void unlock_user(char * nickname){
	userflags_t set, clear;
	
	set.admin = set.exist = set.locked = set.notify_lostadmin = 0;
	clear.admin = clear.exist = clear.locked = clear.notify_lostadmin = 0;
	clear.locked = 1;
	modify_account(nickname, set, clear);
}

void add_admin(char * nickname){
	userflags_t set, clear;
	
	set.admin = set.exist = set.locked = set.notify_lostadmin = 0;
	clear.admin = clear.exist = clear.locked = clear.notify_lostadmin = 0;
	set.admin = 1;
	modify_account(nickname, set, clear);
}

void rem_admin(char * nickname){
	userflags_t set, clear;
	
	set.admin = set.exist = set.locked = set.notify_lostadmin = 0;
	clear.admin = clear.exist = clear.locked = clear.notify_lostadmin = 0;
	clear.admin = 1;
	modify_account(nickname, set, clear);
}

void keymigration(void){
	/* keymigration */
	DS("\r\n keymigration ...");
	do_keymigrate();
}

void rem_keymigration(void){
	DS("\r\n rem migration keys ...");
}

void self_destruct(void){
	DS("\r\n self destruction ...");
}

/******************************************************************************/

