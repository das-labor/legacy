/**
 * \file	main.c
 * \author	Daniel Otte
 * \date	2007-12-06
 * \par license
 *  GPLv3
 * \brief mainpart of AnonAccess
 *
 */





#include "config.h"
#include "debug.h"
#include "uart.h"
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h> // itoa()
#include <avr/eeprom.h>
#include <stdlib.h>
#include "24Cxxx.h"
#include "i2c_tools.h"
#include "ticketDB.h"
#include "flmDB.h"
#include "prng.h"
#include "hmac-sha256.h"
#include "sha256.h"
#include "action.h"
#include "types.h"
#include "keys.h"
#include "reqvalidator.h"
#include "shabea.h"
#include "rtc.h"
#include "enc2E24C.h"
#include "hwrnd.h"
#include "lop.h"
#include "lop_debug.h"
#include "debug_tools.h"

#define MASTERUNIT_ID 0
#define TERMINALUNIT_ID 1

#define MSGID_SESSION_INIT  0
#define MSGID_ADD_AB        1
#define MSGID_AB_REPLY      2
#define MSGID_AB_ERROR      3
#define MSGID_ACTION        4
#define MSGID_ACTION_REPLY  5
#define MSGID_PRINT         6

#define AB_ERROR_WONTTELL   0
#define AB_ERROR_HMAC       1
#define AB_ERROR_EXPIRED    2
#define AB_ERROR_TICKET     3
#define AB_ERROR_LOCKED     4

#define ACTION_MAINOPEN    0x00
#define ACTION_MAINCLOSE   0x01
#define ACTION_ADDUSER     0x10
#define ACTION_REMUSER     0x11
#define ACTION_LOCKUSER    0x12
#define ACTION_UNLOCKUSER  0x13
#define ACTION_ADDADMIN    0x14
#define ACTION_REMADMIN    0x15
#define ACTION_KEYMIGRATION 0x016

#define DONE    1
#define NOTDONE 0
#define ANON    1
#define NOTANON 0


#define SESSION_MAX_DURATION (5LL * 60LL * 1000LL) /* 5 minutes */

/******************************************************************************/
/******************************************************************************/

#define HELP_STR "\r\n ==AnonAccess== " \
				 "\r\n  (debug console)" \
				 "\r\n h: print this help" \
				 "\r\n a: autodetect I2C-Devices" \
				 "\r\n t: print timestamp" \
				 "\r\n s: print session info" \
				 "\r\n r: read serial eeprom in crypto mode (1KiB)" \
				 "\r\n R: read serial eeprom in crypto mode (8KiB)" \
				 "\r\n d: read serial eeprom in plain mode (1KiB)" \
				 "\r\n D: read serial eeprom in plain mode (8KiB)" \
				 "\r\n k: initiate keymigration" \
				 "\r\n f: format databases" \
				 "\r\n i: initialise databases" \
				 "\r\n"

/******************************************************************************/
/******************************************************************************/

uint8_t setupdone_flag EEMEM = 0;

struct {
	uint8_t users;
	uint8_t admins;
	timestamp_t timestamp;
} session;

lop_ctx_t lop0={
	idle, idle, idle, 0, 0, NULL, 0, 
	NULL, NULL, NULL, NULL};
	
enum{
	mainidle,
	insession
} masterstate;

/******************************************************************************/
/******************************************************************************/

void lop0_sendrawbyte(uint8_t b){
	uart_putc(b);
}

/******************************************************************************/

// this handler is called from the uart_hook, i.e. when the Uart receives
// a new byte.
void onuartrx(uint8_t b){
	//let lop handle the received byte.
	lop_recieve_byte(&lop0,b);
}

/******************************************************************************/

bool setup_done(void){
	return eeprom_read_byte(&setupdone_flag);
}

/******************************************************************************/

void setup_system(void){
}

/******************************************************************************/

void session_reset(void){
	session.users = 0;
	session.admins = 0;
	session.timestamp = 0;
}

/******************************************************************************/

void streamrx(uint8_t b){
	i2c_detect_t dev_table;
	
	switch (b){
		case 'h': lop_dbg_str_P(&lop0, PSTR(HELP_STR)); 
			break;
		case 'a': i2c_detect(dev_table);
			break;
		case 't': console_dumptimestamp(); 
			break;
		case 'r': crypto_eeprom_dump(0, 1024);
			break;
		case 'R': crypto_eeprom_dump(0, 8*1024);
			break;
		case 'd': eeprom_dump_page(0xA0, 0, 1024);
			break;
		case 'D': eeprom_dump_page(0xA0, 0, 8*1024);
			break;
		case 's': lop_dbg_str_P(&lop0, PSTR("\r\nsession info:"));
			lop_dbg_str_P(&lop0, PSTR("\r\n\tusers: "));
			lop_dbg_hexdump(&lop0, &session.users, 1);
			lop_dbg_str_P(&lop0, PSTR("\r\n\tadmins: "));
			lop_dbg_hexdump(&lop0, &session.admins, 1);
			lop_dbg_str_P(&lop0, PSTR("\r\n\ttimestamp: "));
			lop_dbg_hexdump(&lop0, &session.timestamp, sizeof(timestamp_t));
			break;
		case 'k': do_keymigrate(); 
			break;
    	case 'f': ticketdb_format(TICKETDB_SIZE); 
    		flmdb_format();/* break;*/
    	case 'i': ticketdb_init();
    		dump_dbstats();
    		break;
    		
		default: lop_dbg_str_P(&lop0, PSTR("\r\n unknown command "));
				 lop_sendstream(&lop0, b);
				 break;
	}
	lop_dbg_str_P(&lop0, PSTR("\r\n done\r\n"));
}

/******************************************************************************/

void messagerx(uint16_t len, uint8_t * msg){
	if(session.users == 255 || session.admins==255){
		/* someone seems to be pretty fast wiht card changing, but we won't allow this */
		session_reset();
		masterstate = mainidle;
		busy &= ~1;
	}
	
	if(len<3){
		session_reset();
		masterstate = mainidle;
		busy &= ~1;
		/* message to short - DROP */
		return;
	}
	if(msg[0] != MASTERUNIT_ID){
		/* packet not for me - DROP */
		return;
	}
	if(msg[1] != TERMINALUNIT_ID){
		/* packet not from terminal - DROP */
		return;
	}
	if((  msg[2] != MSGID_SESSION_INIT) 
	  && (msg[2] != MSGID_ADD_AB) 
	  && (msg[2] != MSGID_ACTION)){
		session_reset();
		masterstate = mainidle;
		busy &= ~1;
		/* unknown message type - DROP */
		return;
	}
	if(msg[2] == MSGID_SESSION_INIT){ /* reset/initiate session*/
		if(len!=3){
			session_reset();
			masterstate = mainidle;
			busy &= ~1;
			/* too long packet - DROP */
			return;
		}
		session_reset();
		session.timestamp = gettimestamp();
		busy |= 1;
		masterstate = insession;
	}
	if(msg[2] == MSGID_ADD_AB){ /* add AuthBlock */
		if(len!=3+130){
			session_reset();
			masterstate = mainidle;
			busy &= ~1;
			/* packet length does not match - DROP */
			return;
		}
		if(masterstate != insession){
			session_reset();
			masterstate = mainidle;
			busy &= ~1;
			/* not "in session" - DROP */
			return;
		}
		if(session.timestamp + SESSION_MAX_DURATION < gettimestamp()){
			session_reset();
			masterstate = mainidle;
			busy &= ~1;
			/* session expired - DROP */
		}
		switch(check_authblock((authblock_t*)&(msg[3]))){
			case invalid_cred:
				{
					uint8_t reply[] = {
						TERMINALUNIT_ID,
						MASTERUNIT_ID,
						MSGID_AB_ERROR, /* AuthBlock error reply */
						AB_ERROR_WONTTELL }; /* won't tell */
					lop_sendmessage(&lop0, 4, reply);	
				}
				return;	break;
			case valid_admin:
				session.admins += 1;
			case valid_user:
				session.users += 1;
				msg[0] = TERMINALUNIT_ID;
				msg[1] = MASTERUNIT_ID;
				msg[2] = MSGID_AB_REPLY; /* AuthBlock reply */
				lop_sendmessage(&lop0, 133, msg);
				return; break;
			default:
				/* GNAHHH, this should NEVER happen */
				session_reset();
				masterstate = mainidle;
				busy &= ~1;
				return; break;
		} /* switch(check_authblock(&(msg[3]))) */
	} /* if add AuthBlock */
	if(msg[2] == MSGID_ACTION){ /* let's do some action */
		if(len<=4){
			/* message too short */
			session_reset();
			masterstate = mainidle;
			busy &= ~1;
			return;
		}
		uint8_t reply[]= {
			TERMINALUNIT_ID,
			MASTERUNIT_ID,
			MSGID_ACTION_REPLY,  /* action reply */
			msg[3],
			NOTDONE}; /* default to "not done" */
		if(masterstate!=insession){
			/* session must be initialised before - DROP */
			lop_sendmessage(&lop0, 5, reply);
			return;
		}
		if(session.timestamp + SESSION_MAX_DURATION < gettimestamp()){
			/* session expired */
			session_reset();
			masterstate = mainidle;
			busy &= ~1;
			lop_sendmessage(&lop0, 5, reply);
			return;
		}
		/* now check the length */
		 /* length of actions without parameter */
		if((msg[3]==ACTION_MAINOPEN || msg[3]==ACTION_MAINCLOSE || msg[3]==ACTION_KEYMIGRATION)
		  && len!=4){
		  	/* wrong length */
			session_reset();
			masterstate = mainidle;
			busy &= ~1;
			lop_sendmessage(&lop0, 5, reply);
			return;
		}
		 /* length of packets with "nickname" parameter */
		if(msg[3]==ACTION_REMUSER || msg[3]==ACTION_LOCKUSER || 
		   msg[3]==ACTION_UNLOCKUSER || msg[3]==ACTION_ADDADMIN || 
		   msg[3]==ACTION_REMADMIN || msg[3]==ACTION_ADDUSER){
			if(len<5){
				/* not even a length byte is given zzz - DROP */
				session_reset();
				masterstate = mainidle;
				busy &= ~1;
				lop_sendmessage(&lop0, 5, reply);
				return;
			}
			if(len!=4+msg[4]+(msg[3]==ACTION_ADDUSER)?1:0){
				/* length is wrong - DROP */
				session_reset();
				masterstate = mainidle;
				busy &= ~1;
				lop_sendmessage(&lop0, 5, reply);
				return;
			}
		}
		/* length checked, now we may try to do something */
		
	}
}

/******************************************************************************/

void init_system(void){
	session.users = 0;
	session.admins = 0;
	session.timestamp = 0;
	uart_init();
	lop0.sendrawbyte = lop0_sendrawbyte;
	lop0.on_streamrx = streamrx;
	lop0.on_msgrx = messagerx;
	uart_hook = onuartrx;
    i2c_init();
    load_eeprom_crypt_key(eeprom_key);
    E24C_init();
    rtc_init();
    prng_init();
    ticketdb_init();
}

/******************************************************************************/

int main(void){
	init_system();
	
	if (!setup_done()){
		setup_system();
	}
	
	while (1){
		;
	}
	return 0;
}

/******************************************************************************/

