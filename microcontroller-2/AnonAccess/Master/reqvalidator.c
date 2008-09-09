/* reqvalidator.c */
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



#include "config.h"
#include <stdint.h>
#include <string.h>
#include <util/delay.h>
#include "hmac-sha256.h"
#include "action.h"
#include "ticketDB.h"
#include "flmDB.h"
#include "reqvalidator.h"
#include "shabea.h"
#include "keys.h"
#include "types.h"
#include "entropium.h"
#include "fairrnd.h"
#include "uart.h"
#include "rtc.h"
#include "hexdigit_tab.h"
#include "main_test_tools.h"
#include "i2c_printer.h"
/*
#define DS(a) uart_putstr_P(PSTR(a))
#define DD(a,b) uart_hexdump((a),(b))
*/
/*
#define DS(a) {uart_putstr_P(PSTR(a)); printer_str_P(PSTR(a));} 
#define DC(a) {uart_putc(a); printer_char(a);} 
#define DD(a,b) {uart_hexdump((a),(b)); printer_hexdump((a),(b));} 
*/
/*
#define DS(a) printer_str_P(PSTR(a))
#define DC(a) printer_char(a)
#define DD(a,b) printer_hexdump((a),(b)) 
*/
//#define DS(a)   {;} 
#define DC(a)   {;} 
#define DD(a,b) {;} 
#define DS(a) send_str(TERMINALUNIT_ID, PSTR(a), STR_CLASS_DBG_P)
#define DSs(a) send_str(TERMINALUNIT_ID, a, STR_CLASS_DBG)

#include "comm.h" /* only for debugging purpose */

/**
 * 
 *  Operation 	 Beschreibung                	 Requirements
 * MainOpen 	Tür öffnen 	                    <valid card>
 * AddUser 	    neuen Benutzer Anlegen          <valid admin card + pin>
 * RemUser 	    Benutzer löschen                2*<valid admin card + pin>
 * AddAdmin 	Benutzer zum Admin machen       2*<valid admin card + pin>
 * RemAdmin 	Admin Rechte entziehen          2*<valid admin card + pin>
 * KeyMigration Keys aus dem Gerät extrahieren 	3*<valid admin card + pin> 
 */

bool check_permissions(uint8_t users, uint8_t admins, action_t action){
	action_requirements_t requirement_table[]=
	{ {1, 0},   // mainopen
	  {1, 0},   // mainclose
	  {0, 1},   // adduser
	  {0, 2},   // remuser
	  {0, 1},   // lockuser
	  {0, 1},   // unlockuser
	  {0, 3},   // addadmin
	  {0, 3},   // remadmin
	  {0, 3},   // keymigration
	  {0, 1},   // lock nick
	  {0, 1},   // open nick
	  {0, 1},   // get info
	  {0, 1},   // search
	  {0, 1},   // search continue
	  {1, 0}    // get statistics
	        }; 
	return (requirement_table[action].users_req<=users 
	     && requirement_table[action].admins_req<=admins);
}

bool final_action(action_t action){
	return true;
}
/*******************************************************************************
 * 
 * 
 * 
 * 
 * 
 */

bool check_pin(authblock_t * ab, sha256_hash_t pinhash){
	if(!ticketdb_userexists(ab->uid))
		return false;	
	uint8_t refhmac[32],key[32];
	uint8_t msg[32];
	
	/* load data from DB */
	ticketdb_getUserPinMac(ab->uid, msg); /* load seed in the first 32 byte */
	load_ticketkeydiv2(msg);
	hmac_sha256(key, msg, 256, ab->ticket, 256);
	delete_key(msg, 32);
	memcpy(msg, pinhash, 32);
	shabea256(msg, key, 256, 0, 16); /* shabea in decrypt mode */
	delete_key(key, 32);

	/* calculate reference MAC */
	load_pinmac_key(key);
	hmac_sha256(refhmac,key,256,pinhash,256);
	delete_key(key, 32);
	 /* now we have the reference hmac */
	
	if(memcmp(refhmac, msg, 32)){
		return false;
	} else {
		return true;
	}
} 
 
void change_pin(authblock_t * ab, sha256_hash_t pinhash){
	if(!ticketdb_userexists(ab->uid))
		return;
	uint8_t hmac[32], key[32], key2[32];

	/* generate hmac */
	load_pinmac_key(key);
	hmac_sha256(hmac, key, 256, pinhash, 256);
	delete_key(key, 32);

	/* encrypt hmac */
	load_ticketkeydiv2(key2);
	hmac_sha256(key, key2, 256, ab->ticket, 256);
	delete_key(key2, 32);
	shabea256(hmac, key, 256, 1, 16);
	delete_key(key, 32);	

	/* store PINMAC */
	ticketdb_setUserPinMac(ab->uid, hmac);	
} 
 
authcredvalid_state_t check_authblock(authblock_t * ab){
	uint8_t key[32], key2[32];
	uint8_t hmac[32], pinhmac[32];
	uint8_t refhmac[32];
	userflags_t flags;
	flags.admin = flags.locked = flags.notify_lostadmin = 0;
	flags.exist = 1;

	/* check authblocks HMAC */
	load_absignkey(key);
	hmac_sha256(hmac,key,256,ab,8*(sizeof(authblock_t)-32));
	delete_key(key, 32);
	if(memcmp(hmac, ab->hmac, 32)){
		/* verification failed, maybe someone modifyed the ab */
		DS("\r\nHMAC failed");
		return invalid_cred;
	}
	
	/* check ticket */
	load_ticketkey(key);
	hmac_sha256(hmac,key,256,ab->ticket,256);
	delete_key(key, 32);
	
	if(!ticketdb_userexists(ab->uid)){
	/*	
		DS("\r\nUser does not exist: ");
		DD(&(ab->uid), 2);
	//	DS(" max: ");
	*/
		uint16_t j;
		uint8_t f;
		j = ticketdb_getstatMaxUsers();
		ticketdb_getUserFlags(ab->uid, (userflags_t*)&f);
	//	DD(&j, 2);
		char errstr[] = "User XXXX ZZ (YYYY) !exist ";
		errstr[5] = pgm_read_byte(hexdigit_tab_P + (((ab->uid)>>12)&0xF));
		errstr[6] = pgm_read_byte(hexdigit_tab_P + (((ab->uid)>> 8)&0xF));
		errstr[7] = pgm_read_byte(hexdigit_tab_P + (((ab->uid)>> 4)&0xF));
		errstr[8] = pgm_read_byte(hexdigit_tab_P + (((ab->uid)>> 0)&0xF));
		errstr[10] = pgm_read_byte(hexdigit_tab_P + ((f>> 4)&0xF));
		errstr[11] = pgm_read_byte(hexdigit_tab_P + ((f>> 0)&0xF));		
		errstr[11+3] = pgm_read_byte(hexdigit_tab_P + ((j>>12)&0xF));
		errstr[12+3] = pgm_read_byte(hexdigit_tab_P + ((j>> 8)&0xF));
		errstr[13+3] = pgm_read_byte(hexdigit_tab_P + ((j>> 4)&0xF));
		errstr[14+3] = pgm_read_byte(hexdigit_tab_P + ((j>> 0)&0xF));
		DSs(errstr);
		return invalid_cred;
	}
	ticketdb_getUserTicketMac(ab->uid, &refhmac);
	if(memcmp(hmac, refhmac, 32)){
		/* wrong ticket, maybe a replay attack */
		DS("\r\nTicket does not match");
		return invalid_cred;
	}
	
	/* decrypt RID */
	load_ridkey(key);
	shabea256(ab->rid, key, 256, 0, 16); /* shabea256 with 16 rounds in decrypt mode */
	delete_key(key,32);
	shabea256(ab->rid, ab->ticket, 256, 0, 16); /* shabea256 with 16 rounds in decrypt mode */
	
	/* decrypt PINMAC */
	load_ticketkeydiv2(key2);
	hmac_sha256(key, key2, 256, ab->ticket, 256);
	delete_key(key2, 32);
	ticketdb_getUserPinMac(ab->uid,pinhmac);
	shabea256(pinhmac, key, 256, 0, 16); /* shabea256 with 16 rounds in decrypt mode */
	delete_key(key,32);
	
	/* search in flag-modify-DB & apply flag modifications */
	flmdb_process(ab->rid, ab->uid, &flags);
		
	if(flags.locked || !flags.exist){
		DS("\r\nUser locked");
		return invalid_cred;
	}	
		
	/* free old user entry in ticketDB */
	ticketdb_deluser(ab->uid);	
	/* generate new uid */
	uint8_t t=0;
	uint16_t i;
	do{
		i = fairrnd(ticketdb_getstatMaxUsers()-1);
		++t;
	} while (ticketdb_userexists(i) && (t<5));
	if(t==5){
		do{
			i = (i+1)%ticketdb_getstatMaxUsers();
		}while(ticketdb_userexists(i));
	}
	ab->uid = i;
	
	
	/* check timestamp */
	load_timestampkey(key);
	shabea256(ab->ticket, key,256, 0, 16);
	delete_key(key, 32);
	{
		timestamp_t t;
		memcpy(&t, ab->ticket+32-sizeof(timestamp_t), sizeof(timestamp_t)); 
		if(flags.anonymous && (t+TICKET_TIMEOUT < gettimestamp())){
			/* setlock bit instead of directly returning invalid_cred 
			 * for making it possible to use the unlock_feature */
			 flags.locked = 1;
			 ticketdb_setUserFlags(ab->uid, &flags);
		}
	}
	
	/* generate new ticket */
	entropium_fillBlockRandom(ab->ticket, 32-sizeof(timestamp_t));
	{
		timestamp_t t;
		t = gettimestamp();
		memcpy(ab->ticket+32-sizeof(timestamp_t), &t, sizeof(timestamp_t));
	}
	load_timestampkey(key);
	shabea256(ab->ticket, key, 256, 1, 16);
	delete_key(key, 32);
	/* store new ticket */
	load_ticketkey(key);
	hmac_sha256(hmac, key, 256, ab->ticket, 32*8);
	delete_key(key, 32);
	ticketdb_newuser(&hmac, &(ab->uid), ab->uid);
	ticketdb_setUserFlags(ab->uid, &flags);
	/* make new RID & Co */
//	entropium_fillBlockRandom(ab->rkey, 32);
//	memset(ab->rkey, 0, 32);
	/* make new RID & Co */
	load_ticketkeydiv1(key2);
	hmac_sha256(key, key2, 256, ab->ticket, 256);
	delete_key(key2,32);
	shabea256(ab->rid, key, 256, 1, 16); /* shabea256 with 16 rounds in encrypt mode */
	delete_key(key, 32);
	load_ridkey(key);
	shabea256(ab->rid, key, 256, 1, 16); /* shabea256 with 16 rounds in encrypt mode */
	delete_key(key,32);
	/* encrypt PINMAC */
	load_ticketkeydiv1(key2);
	hmac_sha256(key, key2, 256, ab->ticket, 256);
	delete_key(key2,32);
	shabea256(pinhmac, key, 256, 1, 16); /* shabea256 with 16 rounds in encrypt mode */
	delete_key(key, 32);
	ticketdb_setUserPinMac(ab->uid, pinhmac);


	/* fix hmac */
	load_absignkey(key);
	hmac_sha256(ab->hmac, key, 256, ab, 8*(sizeof(authblock_t)-32));
	delete_key(key, 32);

	if(flags.locked==1){ // this happens when user got timeout
		DS("\r\nTimeout");
		return invalidtimeout_cred;
	}
	return flags.admin?valid_admin:(flags.notify_lostadmin?valid_user_lostadm:valid_user);
}

uint8_t pin_required(uid_t uid, uint8_t admin){
	userflags_t flags;
	ticketdb_getUserFlags(uid, &flags);
	if(admin){
		if(flags.force_admin_pin)
			return 1;
		#ifdef AAP_ANON_ADMIN_REQ_PIN
		if(flags.anonymous)
			return 1;
		#endif
		#ifdef AAP_ADMIN_REQ_PIN
			return 1;
		#else
			return 0;
		#endif
	} else {
		if(flags.force_normal_pin)
			return 1;
		#ifdef AAP_ANON_USER_REQ_PIN
		if(flags.anonymous)
			return 1;
		#endif
		#ifdef AAP_USER_REQ_PIN
			return 1;
		#else
			return 0;
		#endif	
	}
}


void new_account(authblock_t * ab, char* nickname, sha256_hash_t pinhash,uint8_t anon, uint8_t pinflags){
	userflags_t flags;
	uint8_t key[32], key2[32];
	uint8_t hmac[32];
	
	anon = anon?1:0;
	/* set initial userflags */
	flags.admin = 0;
	flags.exist = 1;
	flags.locked = 0;
	flags.notify_lostadmin = 0;
	flags.anonymous = anon?true:false;
	flags.force_admin_pin = (pinflags&1)?true:false;
	flags.force_normal_pin = (pinflags&2)?true:false;
	flags.lock_nick = 0;
	/* generate new uid */
	uint8_t t=0;
	uint16_t i=0;
	send_str(TERMINALUNIT_ID, PSTR("(1/n)"), STR_CLASS_DBG_P);
	do{
		i = fairrnd(ticketdb_getstatMaxUsers()-1);
		++t;
		send_str(TERMINALUNIT_ID, PSTR("(1.1/n)"), STR_CLASS_DBG_P);
	} while (ticketdb_userexists(i) && (t<5));
	send_str(TERMINALUNIT_ID, PSTR("(2/n)"), STR_CLASS_DBG_P);
	if(t==5){
		do{
			i = (i+1)%ticketdb_getstatMaxUsers();
			send_str(TERMINALUNIT_ID, PSTR("(2.1/n)"), STR_CLASS_DBG_P);
		}while(ticketdb_userexists(i));
	}
	
	send_str(TERMINALUNIT_ID, PSTR("(3/n)"), STR_CLASS_DBG_P);
	ab->uid = i;
	
	/* generate new ticket */
	entropium_fillBlockRandom(ab->ticket, 32-sizeof(timestamp_t));
	{
		timestamp_t t;
		t = gettimestamp();
		memcpy(ab->ticket+32-sizeof(timestamp_t), &t, sizeof(timestamp_t));
	}
	
	send_str(TERMINALUNIT_ID, PSTR("(4/n)"), STR_CLASS_DBG_P);
	load_timestampkey(key);
	send_str(TERMINALUNIT_ID, PSTR("(4.0/n)"), STR_CLASS_DBG_P);
	shabea256(ab->ticket, key, 256, 1, 16); /* encrypt ticket */
	send_str(TERMINALUNIT_ID, PSTR("(4.0.1/n)"), STR_CLASS_DBG_P);
	delete_key(key, 32);
	send_str(TERMINALUNIT_ID, PSTR("(4.1/n)"), STR_CLASS_DBG_P);
	/* store new ticket */
	load_ticketkey(key);
	hmac_sha256(hmac, key, 256, ab->ticket, 32*8);
	delete_key(key, 32);
	send_str(TERMINALUNIT_ID, PSTR("(4.2/n)"), STR_CLASS_DBG_P);
	
	ticketdb_newuser(&hmac, &(ab->uid), ab->uid);
	send_str(TERMINALUNIT_ID, PSTR("(4.3/n)"), STR_CLASS_DBG_P);
	if(!anon){
		ticketdb_addname(ab->uid, nickname);
	}
	
	send_str(TERMINALUNIT_ID, PSTR("(5/n)"), STR_CLASS_DBG_P);
//	ticketdb_setUserFlags(ab->uid, &flags);

	/* make new RID & Co */
	load_nickkey(key);
	hmac_sha256(ab->rid, key, 256, nickname, 8*strlen(nickname));
	delete_key(key, 32);	
//	DS("\r\n hnick: ");
//	DD(ab->rid, 32);
//	entropium_fillBlockRandom(ab->rkey, 32);
//	memset(ab->rkey, 0, 32);

	send_str(TERMINALUNIT_ID, PSTR("(6/n)"), STR_CLASS_DBG_P);
	load_ticketkeydiv1(key);
	hmac_sha256(key2, key, 256, ab->ticket, 256);
	delete_key(key, 32);
	shabea256(ab->rid, key2, 256, 1, 16); /* shabea256 with 16 rounds in encrypt mode */
	delete_key(key2, 32);
	load_ridkey(key);
	shabea256(ab->rid, key, 256, 1, 16); /* shabea256 with 16 rounds in encrypt mode */
	delete_key(key,32);
	
	send_str(TERMINALUNIT_ID, PSTR("(7/n)"), STR_CLASS_DBG_P);
	/* process pinhash */
//	memset(ab->pinhmac, 0, 32);
	load_ticketkeydiv2(key);
	hmac_sha256(key2, key, 256, ab->ticket, 256);
	delete_key(key, 32);
	memcpy(hmac, pinhash, 32);
	shabea256(hmac, key2, 256, 1, 16); /* shabea256 with 16 rounds in encrypt mode */
	ticketdb_setUserPinMac(ab->uid, hmac);
	
	send_str(TERMINALUNIT_ID, PSTR("(8/n)"), STR_CLASS_DBG_P);
	/* fix hmac */
	load_absignkey(key);
	hmac_sha256(ab->hmac, key, 256, ab, 8*(sizeof(authblock_t)-32));
	delete_key(key, 32);
	
	return;
}

void modify_account(char * nickname, userflags_t setflags, userflags_t clearflags){
	uint8_t hmac[32];
	uint8_t key[32];
	
	load_nickkey(key);
	hmac_sha256(hmac, key, 256, nickname, 8*strlen(nickname));
	delete_key(key, 32);
//	DS("\r\n hnick: ");
//	DD(hmac, 32);
	flmdb_makeentry(hmac, setflags, clearflags, 0);
}


void modify_account_byuid(uid_t uid, userflags_t setflags, userflags_t clearflags){
	userflags_t flags;
	
	ticketdb_getUserFlags(uid, &flags);
/*	flags |= setflags;
	flags &= ~clearflags; */
	flags.admin            |= setflags.admin;
	flags.exist            |= setflags.exist;
	flags.locked           |= setflags.locked;
	flags.notify_lostadmin |= setflags.notify_lostadmin;
	flags.force_admin_pin  |= setflags.force_admin_pin;
	flags.force_normal_pin |= setflags.force_normal_pin;
	flags.lock_nick        |= setflags.lock_nick;
	flags.admin            &= ~clearflags.admin;
	flags.exist            &= ~clearflags.exist;
	flags.locked           &= ~clearflags.locked;
	flags.notify_lostadmin &= ~clearflags.notify_lostadmin;
	flags.force_admin_pin  &= ~clearflags.force_admin_pin;
	flags.force_normal_pin &= ~clearflags.force_normal_pin;
	flags.lock_nick        &= ~clearflags.lock_nick;
	ticketdb_setUserFlags(uid, &flags);	
}
