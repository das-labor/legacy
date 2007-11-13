
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

#define DS(a) uart_putstr_P(PSTR(a))
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

/*******************************************************************************
 * 
 * 
 * 
 * 
 * 
 */
bool valid_authreq(action_t action, uint8_t n, authblock_t * authblock){
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
	uint8_t users=0, admins=0;  
	uint8_t i;
	authcredvalid_state_t t;
	
	for(i=1; i<n; ++i){ /* check for multiple authcreds */
		uint8_t j;
		for(j=0; j<i; ++j){
			if(authblock[i].uid == authblock[j].uid){
				uart_putstr_P(PSTR(" double authcreds!"));
				return false;
			}
		}
	}
	
	for(i=0;i<n;++i){
		t =  check_authblock(&(authblock[i]));
		users  += (t==valid_user || t==valid_admin)?1:0;
		admins += (t==valid_admin)?1:0;
	}
	uart_putstr_P(PSTR("\r\n users: "));
	uart_hexdump((char*)&users,1);
	uart_putstr_P(PSTR("\r\n admins: "));
	uart_hexdump((char*)&admins,1);
	
	for(i=0;i<n;++i){
		t =  check_authblock(&(authblock[i]));
		users  += (t==valid_user || t==valid_admin)?1:0;
		admins += (t==valid_admin)?1:0;
	}
	
	if((requirement_table[action].users_req  <= users)
	 &&(requirement_table[action].admins_req <= admins)){
		return true;
	}
	
	return false;
}


/*******************************************************************************
 * 
 * 
 * 
 * 
 * 
 */
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
		uart_putstr_P(PSTR("\r\nHMAC failed"));
		return invalid_cred;
	}
	
	/* check ticket */
	load_ticketkey(key);
	hmac_sha256(hmac,key,256,ab->ticket,256);
	delete_key(key, 32);
	
	if(!ticketdb_userexists(ab->uid)){
		uart_putstr_P(PSTR("\r\nUser does not exist"));
		return invalid_cred;
	}
	ticketdb_getUserTicketMac(ab->uid, &refhmac);
	if(memcmp(hmac, refhmac, 32)){
		/* wrong ticket, maybe a replay attack */
		uart_putstr_P(PSTR("\r\nTicket does not match"));
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
		uart_putstr_P(PSTR("\r\nUser locked"));
		return invalid_cred;
	}	
		
	/* free old user entry in ticketDB */
	ticketdb_deluser(ab->uid);	
	/* generate new uid */
	uint16_t i;
	i = fairrnd(ticketdb_getstatMaxUsers()-ticketdb_getstatUsers());	
	ab->uid = -1;
	while(i){
		ab->uid++;
		if(!ticketdb_userexists(ab->uid)){
			--i;
		}
	}	
	
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
		uart_putstr_P(PSTR("\r\nTimeout"));
		return invalid_cred;
	}
	return flags.admin?valid_admin:valid_user;
}

void new_account(authblock_t * ab, char* nickname){
	userflags_t flags;
	uint8_t key[32];
	uint8_t hmac[32];
	/* set initial userflags */
	flags.admin = 0;
	flags.exist = 1;
	flags.locked = 0;
	flags.notify_lostadmin = 0;
	flags.reserved = 0;
	/* generate new uid */
	uint16_t i;
	i = fairrnd(ticketdb_getstatMaxUsers()-ticketdb_getstatUsers());	
	ab->uid = -1;
	while(i){
		ab->uid++;
		if(!ticketdb_userexists(ab->uid)){
			--i;
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
	load_nickkey(key);
	hmac_sha256(ab->rid, key, 256, nickname, 8*strlen(nickname));
	delete_key(key, 32);
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
	flmdb_makeentry(hmac, setflags, clearflags);
}


