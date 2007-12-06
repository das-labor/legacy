
#include "prng.h"
#include "ticketDB.h"
#include "flmDB.h"
#include "action.h"
#include "uart.h"
#include "hmac-sha256.h"
#include "types.h"
#include "reqvalidator.h"
#include <avr/eeprom.h>
#include "keys.h"

#define DS(a)   uart_putstr_P(PSTR(a))
#define DD(a,b) uart_hexdump((a),(b))

/******************************************************************************/
void main_open(void){
	uart_putstr("\r\n door opening");
}

void main_close(void){
	uart_putstr("\r\n door closeing");
}

void dump_authblock(authblock_t * ab){
	DS("\r\n +++ authblock +++");
	DS("\r\n   UID:    "); DD((char*)&(ab->uid),  2);
	DS("\r\n   RID:    "); DD((char*)&(ab->rid), 32);
	DS("\r\n   RKey:   "); DD((char*)&(ab->rkey), 32);
	DS("\r\n   Ticket: "); DD((char*)&(ab->ticket), 32);
	DS("\r\n   HMAC:   "); DD((char*)&(ab->hmac), 32);
}

void add_user(char * nickname){
	authblock_t ab;
	new_account(&ab, nickname);
	dump_authblock(&ab);
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

/******************************************************************************/
/*
typedef enum{
	mainopen   = 0, 
	mainclose, 
	adduser, 
	remuser,
	lockuser,
	unlockuser,
	addadmin,
	remadmin,
	keymigrate
} action_t;

*/

void perform_action(action_t action, void * data){
	switch (action){
		case mainopen:   main_open(); break;
		case mainclose:  main_close(); break;
		case adduser:    add_user((char*)data); break;
		case remuser:    rem_user((char*)data); break;
		case lockuser:   lock_user((char*)data); break;
		case unlockuser: unlock_user((char*)data); break;
		case addadmin:   add_admin((char*)data); break;
		case remadmin:   rem_admin((char*)data); break;
		case keymigrate: keymigration(); break;
		default: /* ERROR */ break;
	}
}







