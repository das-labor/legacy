
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
#define DS(a)   {;} 
#define DC(a)   {;} 
#define DD(a,b) {;} 

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
 
void update_pin(authblock_t * ab, char* pin){//sha256_hash_t pinhash){
	if(!ticketdb_userexists(ab->uid))
		return;
	uint16_t l=strlen(pin);	
	uint8_t refhmac[32],key[32];
	uint8_t msg[32+l];
	
	entropium_fillBlockRandom(msg,32);
	ticketdb_setUserPinMac(ab->uid, msg); /* load seed in the first 32 byte */
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
		DS("\r\nUser does not exist: ");
		DD(&(ab->uid), 2);
		DS(" max: ");
		uint16_t j;
		j = ticketdb_getstatMaxUsers();
		DD(&j, 2);
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
	memset(ab->rkey, 0, 32);
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
	return flags.admin?valid_admin:valid_user;
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
	entropium_fillBlockRandom(ab->ticket, 32-sizeof(timestamp_t));
	{
		timestamp_t t;
		t = gettimestamp();
		memcpy(ab->ticket+32-sizeof(timestamp_t), &t, sizeof(timestamp_t));
	}
	load_timestampkey(key);
	shabea256(ab->ticket, key, 256, 1, 16); /* encrypt ticket */
	delete_key(key, 32);
	
	/* store new ticket */
	load_ticketkey(key);
	hmac_sha256(hmac, key, 256, ab->ticket, 32*8);
	delete_key(key, 32);
	ticketdb_newuser(&hmac, &(ab->uid), ab->uid);
	if(!anon){
		ticketdb_addname(ab->uid, nickname);
	}
//	ticketdb_setUserFlags(ab->uid, &flags);

	/* make new RID & Co */
	load_nickkey(key);
	hmac_sha256(ab->rid, key, 256, nickname, 8*strlen(nickname));
	delete_key(key, 32);	
//	DS("\r\n hnick: ");
//	DD(ab->rid, 32);
//	entropium_fillBlockRandom(ab->rkey, 32);
	memset(ab->rkey, 0, 32);
	load_ticketkeydiv1(key);
	hmac_sha256(key2, key, 256, ab->ticket, 256);
	delete_key(key, 32);
	shabea256(ab->rid, key2, 256, 1, 16); /* shabea256 with 16 rounds in encrypt mode */
	delete_key(key2, 32);
	load_ridkey(key);
	shabea256(ab->rid, key, 256, 1, 16); /* shabea256 with 16 rounds in encrypt mode */
	delete_key(key,32);
	
	/* process pinhash */
	memset(ab->pinhmac, 0, 32);
	load_ticketkeydiv2(key);
	hmac_sha256(key2, key, 256, ab->ticket, 256);
	delete_key(key, 32);
	memcpy(hmac, pinhash, 32);
	shabea256(hmac, key2, 256, 1, 16); /* shabea256 with 16 rounds in encrypt mode */
	ticketdb_setUserPinMac(ab->uid, hmac);
	
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


