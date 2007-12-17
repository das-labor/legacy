
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
#include "prng.h"
#include "fairrnd.h"
#include "uart.h"
#include "rtc.h"
#include "main_test_tools.h"
#include "i2c_printer.h"
/*
#define DS(a) uart_putstr_P(PSTR(a))
#define DD(a,b) uart_hexdump((a),(b))

#define DS(a) {uart_putstr_P(PSTR(a)); printer_str_P(PSTR(a));} 
#define DC(a) {uart_putc(a); printer_char(a);} 
#define DD(a,b) {uart_hexdump((a),(b)); printer_hexdump((a),(b));} 
*/
#define DS(a) printer_str_P(PSTR(a))
#define DC(a) printer_char(a)
#define DD(a,b) printer_hexdump((a),(b)) 


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
	  {0, 2},   // addadmin
	  {0, 2},   // remadmin
	  {0, 3} }; // keymigration
	return (requirement_table[action].users_req<=users 
	     && requirement_table[action].admins_req<=admins);
}
/*******************************************************************************
 * 
 * 
 * 
 * 
 * 
 */

bool check_pin(authblock_t * ab, char* pin){
	if(!ticketdb_userexists(ab->uid))
		return false;
	uint16_t l=strlen(pin);	
	uint8_t refhmac[32],key[32];
	uint8_t msg[32+l];
	
	ticketdb_getUserPinMacSeed(ab->uid, msg); /* load seed in the first 32 byte */
	memcpy(msg+32, pin, l); /* load pin */
	load_pinmac_key(key);
	hmac_sha256(refhmac,key,256,msg,8*(32+l));
	delete_key(key, 32);
	 /* now we have the reference hmac */
	memcpy(msg, ab->pinhmac, 32);
	load_pinenc_key(key);
	shabea256(msg, key, 256, 0, 16);
	delete_key(key, 32);
	shabea256(msg, ab->rkey, 256, 0, 16);
	
	if(memcmp(refhmac, msg, 32)){
		return false;
	} else {
		return true;
	}
} 
 
void update_pin(authblock_t * ab, char* pin){
	if(!ticketdb_userexists(ab->uid))
		return;
	uint16_t l=strlen(pin);	
	uint8_t refhmac[32],key[32];
	uint8_t msg[32+l];
	
	fillBlockRandom(msg,32);
	ticketdb_setUserPinMacSeed(ab->uid, msg); /* load seed in the first 32 byte */
	memcpy(msg+32, pin, l); /* load pin */
	load_pinmac_key(key);
	hmac_sha256(refhmac,key,256,msg,8*(32+l));
	delete_key(key, 32);
		/* now we have to encrypt the hmac */
	shabea256(refhmac, ab->rkey, 256, 1, 16);
	load_pinenc_key(key);
	shabea256(refhmac, key, 256, 1, 16);
	delete_key(key, 32);
	memcpy(ab->pinhmac, refhmac, 32);
} 
 
authcredvalid_state_t check_authblock(authblock_t * ab){
	uint8_t key[32];
	uint8_t hmac[32];
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
		DS("\r\nUser does not exist");
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
	shabea256(ab->rid, ab->rkey, 256, 0, 16); /* shabea256 with 16 rounds in decrypt mode */
	
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
	fillBlockRandom(ab->ticket, 32-sizeof(timestamp_t));
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
	fillBlockRandom(ab->rkey, 32);
	shabea256(ab->rid, ab->rkey, 256, 1, 16); /* shabea256 with 16 rounds in decrypt mode */
	load_ridkey(key);
	shabea256(ab->rid, key, 256, 1, 16); /* shabea256 with 16 rounds in decrypt mode */
	delete_key(key,32);

	/* fix hmac */
	load_absignkey(key);
	hmac_sha256(ab->hmac, key, 256, ab, 8*(sizeof(authblock_t)-32));
	delete_key(key, 32);

	if(flags.locked==1){ // this happens when user got timeout
		DS("\r\nTimeout");
		return invalidtimeout_cred;
	}
	return flags.admin?valid_admin:valid_user;
}

void new_account(authblock_t * ab, char* nickname, uint8_t anon){
	userflags_t flags;
	uint8_t key[32];
	uint8_t hmac[32];
	
	anon = anon?1:0;
	/* set initial userflags */
	flags.admin = 0;
	flags.exist = 1;
	flags.locked = 0;
	flags.notify_lostadmin = 0;
	flags.anonymous = anon?true:false;
	flags.reserved = 0;
	/* generate new uid */
	uint8_t t=0;
	uint16_t i=0;
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
	
	/* generate new ticket */
	fillBlockRandom(ab->ticket, 32-sizeof(timestamp_t));
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
	if(!anon){
		ticketdb_addname(ab->uid, nickname);
	}
	ticketdb_setUserFlags(ab->uid, &flags);

	/* make new RID & Co */
	load_nickkey(key);
	hmac_sha256(ab->rid, key, 256, nickname, 8*strlen(nickname));
	delete_key(key, 32);	
//	DS("\r\n hnick: ");
//	DD(ab->rid, 32);
	fillBlockRandom(ab->rkey, 32);
	shabea256(ab->rid, ab->rkey, 256, 1, 16); /* shabea256 with 16 rounds in decrypt mode */
	load_ridkey(key);
	shabea256(ab->rid, key, 256, 1, 16); /* shabea256 with 16 rounds in decrypt mode */
	delete_key(key,32);
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


