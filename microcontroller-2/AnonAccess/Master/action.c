/* action.c */
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



#include "entropium.h"
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
#include <string.h>
#include "i2c_printer.h"


#include "comm.h"
/*
#define DS(a)   uart_putstr_P(PSTR(a))
#define DD(a,b) uart_hexdump((a),(b))
*/
/*
#define DS(a) {uart_putstr_P(PSTR(a)); printer_str_P(PSTR(a));} 
#define DC(a) {uart_putc(a); printer_char(a);} 
#define DD(a,b) {uart_hexdump((a),(b)); printer_hexdump((a),(b));} 
*/

//#define DS(a) {;} 
#define DS(a) send_str(TERMINALUNIT_ID, PSTR(a), STR_CLASS_DBG_P)
#define DC(a) {;} 
#define DD(a,b) {;} 


/******************************************************************************/
void door_init(void){
	DDRA = 0xD0;
	PORTA = 0xD0;
}

uint8_t main_open(void){
	PORTA &= ~0x60;
	PORTA |=  0x80;
	DS("\r\n>> door opening <<");
	return 0;
}

uint8_t main_close(void){
	PORTA &= ~0x80;
	PORTA |=  0x60;
	DS("\r\n>> door closing <<");
	return 0;
}

void dump_authblock(authblock_t * ab){
	DS("\r\n +++ authblock +++");
	DS("\r\n   UID:    "); DD((char*)&(ab->uid),  2);
	DS("\r\n   RID:    "); DD((char*)&(ab->rid), 32);
	DS("\r\n   RKey:   "); DD((char*)&(ab->rkey), 32);
	DS("\r\n   Ticket: "); DD((char*)&(ab->ticket), 32);
	DS("\r\n   HMAC:   "); DD((char*)&(ab->hmac), 32);
}

void modify_account_id(uint8_t* id, userflags_t set, userflags_t clear){
	if(*id){
		char name[*id+1];
		memcpy(name, id+1, *id);
		name[*id] = '\0';
		modify_account(name, set, clear);
	} else {
		modify_account_byuid(*((uid_t*)(id+1)), set, clear);
	}
}

void add_user(char * nickname, sha256_hash_t pinhash,uint8_t anon, uint8_t pinflags, authblock_t *dest){
	new_account(dest, nickname, pinhash, anon, pinflags);
//	dump_authblock(&ab);
}

uint8_t rem_user(uint8_t* id){
	userflags_t set, clear;
	
	set.admin = set.exist = set.locked = set.notify_lostadmin = 0;
	clear.admin = clear.exist = clear.locked = clear.notify_lostadmin = 0;
	clear.exist = 1;
	modify_account_id(id, set, clear);
	return 0;
}

uint8_t lock_user(uint8_t* id){
	userflags_t set, clear;
	
	set.admin = set.exist = set.locked = set.notify_lostadmin = 0;
	clear.admin = clear.exist = clear.locked = clear.notify_lostadmin = 0;
	set.locked = 1;
	modify_account_id(id, set, clear);
	return 0;
}

uint8_t unlock_user(uint8_t* id){
	userflags_t set, clear;
	
	set.admin = set.exist = set.locked = set.notify_lostadmin = 0;
	clear.admin = clear.exist = clear.locked = clear.notify_lostadmin = 0;
	clear.locked = 1;
	modify_account_id(id, set, clear);
	return 0;
}

uint8_t add_admin(uint8_t* id){
	userflags_t set, clear;
	
	set.admin = set.exist = set.locked = set.notify_lostadmin = 0;
	clear.admin = clear.exist = clear.locked = clear.notify_lostadmin = 0;
	set.admin = 1;
	modify_account_id(id, set, clear);
	return 0;
}

uint8_t rem_admin(uint8_t* id){
	userflags_t set, clear;
	
	set.admin = set.exist = set.locked = set.notify_lostadmin = 0;
	clear.admin = clear.exist = clear.locked = clear.notify_lostadmin = 0;
	clear.admin = 1;
	modify_account_id(id, set, clear);
	return 0;
}

uint8_t keymigration(void){
	/* keymigration */
	DS("\r\n keymigration ...");
	do_keymigrate();
	return 0;
}

uint8_t rem_keymigration(void){
	DS("\r\n rem migration keys ...");
	return 0;
}

uint8_t lock_nick(uint8_t *nick){
	uint8_t key[32];
	sha256_hash_t mac;
	userflags_t f,ff;
	
	load_ridkey(key);
	hmac_sha256(mac, key, 256, nick+1, (*nick)*256);
	delete_key(key, 32);
	f.admin = f.anonymous = f.exist = f.force_admin_pin = f.force_normal_pin =
	          f.locked = f.notify_lostadmin = 0;
	f.lock_nick = 1;
	ff = f;
	ff.lock_nick = 0;
	flmdb_makeentry(mac,f,ff,1);
	return 0;
}

uint8_t open_nick(uint8_t *nick){
	uint8_t key[32];
	sha256_hash_t mac;
	userflags_t f,ff;
	
	load_ridkey(key);
	hmac_sha256(mac, key, 256, nick+1, (*nick)*256);
	delete_key(key, 32);
	f.admin = f.anonymous = f.exist = f.force_admin_pin = f.force_normal_pin =
	          f.locked = f.notify_lostadmin = 0;
	f.lock_nick = 1;
	ff = f;
	ff.lock_nick = 0;
	flmdb_makeentry(mac,ff,f,1);
	return 0;
}

void self_destruct(void){
	DS("\r\n self destruction ...");
}

/******************************************************************************/

