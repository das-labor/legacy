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
#include "entropium.h"
#include "hmac-sha256.h"
#include "sha256.h"
#include "action.h"
#include "types.h"
#include "keys.h"
#include "reqvalidator.h"
#include "shabea.h"
#include "rtc.h"
#include "reset_counter.h"
#include "enc2E24C.h"
#include "hwrnd.h"
#include "lop.h"
#include "lop_debug.h"
#include "qport.h"
#include "debug_tools.h"
#include "selfdestruct.h"
#include "i2c_printer.h"
#include "iDP3540.h"
#include "comm.h"

/*
#define DS(a)   uart_putstr_P(PSTR(a))
#define DC(a)	uart_putc(a)
#define DD(a,b) uart_hexdump((a),(b))
*/

/*
#define DS(a) {uart_putstr_P(PSTR(a)); printer_str_P(PSTR(a));} 
#define DC(a) {uart_putc(a); printer_char(a);} 
#define DD(a,b) {uart_hexdump((a),(b)); printer_hexdump((a),(b));} 
*/

/*
#define DS(a) {uart_putstr_P(PSTR(a)); terminal_print_P(&lop0, STR_CLASS_INFO, PSTR(a));} 
#define DC(a) {uart_putc(a); printer_char(a);} 
#define DD(a,b) {uart_hexdump((a),(b)); printer_hexdump((a),(b));} 
*/

#define DS(a) {terminal_print_P(&lop1, STR_CLASS_INFO, PSTR(a));} 
//#define DS(a) {;} 
#define DC(a) {;} 
#define DD(a,b) {;} 


#define MASTERUNIT_ID 0
#define TERMINALUNIT_ID 1

#define MSGID_SESSION_INIT  0
#define MSGID_ADD_AB        1
#define MSGID_AB_REPLY      2
#define MSGID_AB_ERROR      3
#define MSGID_ACTION        4
#define MSGID_ACTION_REPLY  5
#define MSGID_PRINT         6

#define MSGID_ADD_BOOTSTRAP 7

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


#define STR_CLASS_NO    0x00
#define STR_CLASS_INFO  0x01
#define STR_CLASS_WARN  0x02
#define STR_CLASS_ERROR 0x03
#define STR_CLASS_SEC   0x04
#define STR_CLASS_MAX   0x04


#define DONE    1
#define NOTDONE 0
#define ANON    1
#define NOTANON 0

#define SESSION_MAX_PARTICIPANTS 5
#define SESSION_MAX_DURATION (5LL * 60LL * 1000LL) /* 5 minutes */

/******************************************************************************/
/******************************************************************************/

#define HELP_STR "\r\n ==AnonAccess== " \
				 "\r\n  (debug console)" \
				 "\r\n h: print this help" \
				 "\r\n a: autodetect I2C-Devices" \
				 "\r\n t: print timestamp" \
				 "\r\n T: print number of resets done" \
				 "\r\n s: print session info" \
				 "\r\n r: read serial eeprom in crypto mode (1KiB)" \
				 "\r\n R: read serial eeprom in crypto mode (8KiB)" \
				 "\r\n d: read serial eeprom in plain mode (1KiB)" \
				 "\r\n D: read serial eeprom in plain mode (8KiB)" \
				 "\r\n k: initiate keymigration" \
				 "\r\n f: format databases" \
				 "\r\n i: initialise databases" \
				 "\r\n o: execute main_open()" \
				 "\r\n c: execute main_close()" \
				 "\r\n u: cut paper" \
				 "\r\n"

/******************************************************************************/
/******************************************************************************/

uint8_t setupdone_flag EEMEM = 0;
uint8_t bootstrap_accounts EEMEM = BOOTSTRAP_ACCOUNTS;

struct {
	uint8_t users;
	uint8_t admins;
	timestamp_t timestamp;
	uid_t participants[SESSION_MAX_PARTICIPANTS];
} session;

/* keys from /dev/random */
uint8_t qp0_key_rxa[16] EEMEM = {0x3c, 0x71, 0x68, 0xa9, 0x6b, 0xf1, 0x63, 0xac,
                                 0xc4, 0x7f, 0x61, 0x00, 0xcb, 0x96, 0x3d, 0x74};
uint8_t qp0_key_rxb[16] EEMEM = {0xd4, 0x82, 0x18, 0x04, 0xd2, 0x29, 0xe2, 0x7e,
                                 0x7b, 0x41, 0x2a, 0x2e, 0x30, 0xc6, 0x5c, 0xbd};
uint8_t qp0_key_txa[16] EEMEM = {0x15, 0x36, 0xb4, 0x47, 0x62, 0xc2, 0xc8, 0x35,
                                 0x78, 0xb5, 0x4c, 0xa3, 0xcb, 0x10, 0x8b, 0x34};
uint8_t qp0_key_txb[16] EEMEM = {0xec, 0x78, 0x0c, 0xf0, 0x44, 0xf9, 0x6f, 0x4a,
                                 0xd5, 0xf6, 0x3a, 0x87, 0xde, 0x3d, 0x8f, 0x18};

uint8_t qp0_key_hmac[32] EEMEM = {0x8c, 0x40, 0xc8, 0x3c, 0x84, 0x11, 0x8a, 0xec,
                                  0x66, 0x66, 0x5d, 0x9f, 0x3b, 0x79, 0x55, 0x63,
                                  0x68, 0x3e, 0xc3, 0xb2, 0x20, 0xc6, 0xa7, 0x7e,
                                  0x13, 0x54, 0x88, 0x77, 0x02, 0x9b, 0xb9, 0x8a};

lop_ctx_t lop0={
	idle, idle, idle, 0, 0, NULL, 0, 
	NULL, NULL, NULL, NULL};

qport_ctx_t qp0;

lop_ctx_t lop1={
	idle, idle, idle, 0, 0, NULL, 0, 
	NULL, NULL, NULL, NULL};
	
enum{
	mainidle,
	insession
} masterstate;

/******************************************************************************/
/******************************************************************************/

void streamrx(uint8_t b);
void messagerx_old(uint16_t length, uint8_t* msg);


// this handler is called from the uart_hook, i.e. when the Uart receives
// a new byte.
void onuartrx(uint8_t b){
	lop_recieve_byte(&lop0,b);
}

void lop0_sendrawbyte(uint8_t b){
	uart_putc((char)b);
}

void lop0_streamrx(uint8_t b){
	qport_recieve_byte(&qp0, b);
}

void lop0_streamsync(void){	
}

void lop0_messagerx(uint16_t length, uint8_t * msg){
	qport_incomming_msg(&qp0, length, msg);
}

/******************************************************************************/

void qp0_streamrx(uint8_t b){
	lop_recieve_byte(&lop1, b);
}

/******************************************************************************/

void lop1_sendrawbyte(uint8_t b){
	qport_streamsend(&qp0, b);
}

void lop1_streamrx(uint8_t b){
	streamrx(b);
}

void lop1_messagerx(uint16_t length, uint8_t* msg){
	messagerx(length, msg);
}

/******************************************************************************/

bool setup_done(void){
	return eeprom_read_byte(&setupdone_flag);
}

/******************************************************************************/

void setup_system(void){
	uint8_t a,b;
	uint8_t buffer[32];
	/* eeprom keyspace is 8*32 byte */
	DS("\r\nstarting SETUP");
	DS("\r\ngenerating keys:");
	for(a=0; a<EEPROM_SEC_INIT_RWS; ++a){
		for(b=0; b<KEY_NUM; ++b){
			entropium_getRandomBlock((uint32_t*)buffer);
			eeprom_write_block(buffer, (void*)(b*32), 32);
		}
		if((a&(31))==0){
			DS("\r\n");
		}
		DC('.');
	}
	load_eeprom_crypt_key(eeprom_key);
	DS("\r\ninitialising Ticket-DB: ...");
	ticketdb_format(TICKETDB_SIZE);
	DS("\r\ninitialising FLM-DB: ...");
	flmdb_format();
	eeprom_write_byte(&setupdone_flag, 1);
	DS("\r\nSETUP DONE\r\n");
}

/******************************************************************************/

void session_reset_old(void){
	uint8_t i;
	session.users = 0;
	session.admins = 0;
	session.timestamp = 0;
	for(i=0; i<SESSION_MAX_PARTICIPANTS; ++i){
		session.participants[i] = NO_USER;
	}
}

/******************************************************************************/

void streamrx(uint8_t b){
	return;
/*	
	i2c_detect_t dev_table;
	
	switch (b){
		case 'h': lop_dbg_str_P(&lop0, PSTR(HELP_STR)); 
			break;
		case 'a': i2c_detect(dev_table);
			break;
		case 't': console_dumptimestamp(); 
			break;
		case 'T': console_dumpresets(); 
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
    		flmdb_format();/ * break;* /
    	case 'i': ticketdb_init();
    		dump_dbstats();
    		break;
    	case 'o': main_open();
    		break;
    	case 'c': main_close();
    		break;
    	case 'u': cutpaper();
    		break;

		default: lop_dbg_str_P(&lop0, PSTR("\r\n unknown command "));
				 lop_sendstream(&lop0, b);
				 break;
	}
	lop_dbg_str_P(&lop0, PSTR("\r\n done\r\n"));
	*/
}

/******************************************************************************/

bool msg_check(uint16_t len, uint8_t * msg){
	if(len<3){
		session_reset_old();
		masterstate = mainidle;
		busy &= ~1;
		/* message to short - DROP */
		return 0;
	}
	if(msg[0] != MASTERUNIT_ID){
		/* packet not for me - DROP */
		return 0;
	}
	if(msg[1] != TERMINALUNIT_ID){
		/* packet not from terminal - DROP */
		return 0;
	}
	if(msg[2]>7) /* no valid MSGID */
		return 0;
	/* check packages length */
	switch(msg[2]){
		case MSGID_SESSION_INIT:
			return len==3;
		case MSGID_ADD_AB:
			return len==3+sizeof(authblock_t);
		case MSGID_ADD_BOOTSTRAP:
			if(len<3+2)
				return 0;
			if(len!=4+msg[3]+sizeof(sha256_hash_t)+1+1)
				return 0;
			return 1;	
		case MSGID_ACTION:
			if(len<4)
				return 0;
			break;
		/* the following packets should not be recieved by the maser */
		case MSGID_AB_REPLY:
		case MSGID_AB_ERROR:
		case MSGID_ACTION_REPLY:
		case MSGID_PRINT:
		default:
			return 0;	
	}
	/* now only the action stuff is left */
	switch(msg[3]){
		case ACTION_MAINOPEN:
		case ACTION_MAINCLOSE:
		case ACTION_KEYMIGRATION:
			return len==4;
		case ACTION_ADDUSER:
			if(len<6)
				return 0;
			return len==5+msg[4]+1;
		case ACTION_REMUSER:
		case ACTION_LOCKUSER:
		case ACTION_UNLOCKUSER:
		case ACTION_ADDADMIN:
		case ACTION_REMADMIN:
			if(len<6)
				return 0;
			if(msg[4]){
				/* nickname option choosen */
				return len==5+msg[4];
			} else {
				/* uid option choosen */
				return len==7;
			}
		default:
			return 0;
	}
}

/******************************************************************************/

void messagerx_old(uint16_t len, uint8_t * msg){
/*	
	lop_dbg_str_P(&lop0, PSTR("\r\nmessage rx:"));
	lop_dbg_hexdump(&lop0, msg, len);
*/	
	if(session.users > SESSION_MAX_PARTICIPANTS || session.admins > SESSION_MAX_PARTICIPANTS){
		/* someone seems to be pretty fast wiht card changing, but we won't allow this */
		session_reset_old();
		masterstate = mainidle;
		busy &= ~1;
	}
	
	
	if(!msg_check(len, msg)){
		session_reset_old();
		masterstate = mainidle;
		busy &= ~1;
		/* unknown message type - DROP */
		DS("\r\nERROR:  MESSAGE DROPED\r\n\r\n\r\n\r\n\r\n")
		return;
	}
	
	if(msg[2] == MSGID_SESSION_INIT){ /* reset/initiate session*/
		session_reset_old();
		session.timestamp = gettimestamp();
		busy |= 1;
		masterstate = insession;
		return;
	}
	
	if(masterstate != insession){
		session_reset_old();
		masterstate = mainidle;
		busy &= ~1;
		/* not "in session" - DROP */
		return;
	}

	if(session.timestamp + SESSION_MAX_DURATION < gettimestamp()){
		session_reset_old();
		masterstate = mainidle;
		busy &= ~1;
		/* session expired - DROP */
	}
	
	if(msg[2] == MSGID_ADD_AB){ /* add AuthBlock */
		/* check if user allready participates */
		uint8_t i;
		for(i=0; i<session.users; ++i){
			if(((authblock_t*)&(msg[3]))->uid == session.participants[i]){
				/* user already participates */
				uint8_t reply[] = {
					TERMINALUNIT_ID,
					MASTERUNIT_ID,
					MSGID_AB_ERROR, /* AuthBlock error reply */
				1};//	AB_ERROR_WONTTELL }; /* won't tell */
				lop_sendmessage(&lop1, 4, reply);
				return;
			}
		}
		switch(check_authblock((authblock_t*)&(msg[3]))){
			case invalid_cred:
				{
					uint8_t reply[] = {
						TERMINALUNIT_ID,
						MASTERUNIT_ID,
						MSGID_AB_ERROR, /* AuthBlock error reply */
					2};//	AB_ERROR_WONTTELL }; /* won't tell */
					lop_sendmessage(&lop1, 4, reply);	
				}
				return;	break;
			case valid_admin:
				session.admins += 1;
			case valid_user:
				session.users += 1;
			case invalidtimeout_cred:  /* on timeout user gets new AB but no user increment is done */	
				msg[0] = TERMINALUNIT_ID;
				msg[1] = MASTERUNIT_ID;
				msg[2] = MSGID_AB_REPLY; /* AuthBlock reply */
				
				lop_sendmessage(&lop1, 3+sizeof(authblock_t), msg);
				return; break;
			default:
				/* GNAHHH, this should NEVER happen */
				session_reset_old();
				masterstate = mainidle;
				busy &= ~1;
				return; break;
		} /* switch(check_authblock(&(msg[3]))) */
	} /* if add AuthBlock */
	if(msg[2] == MSGID_ACTION){ /* let's do some action */
		
		uint8_t reply[]= {
			TERMINALUNIT_ID,
			MASTERUNIT_ID,
			MSGID_ACTION_REPLY,  /* action reply */
			msg[3],   /* reply to what requested */
			NOTDONE}; /* default to "not done" */
		
		/* length checked, now we may try to do something */
		action_t action;
		action = (msg[3]&0xf0)?(msg[3]-0x10+2):msg[3]; /* transform ACTION_* in action_t */
		if(!check_permissions(session.users, session.admins, action)){
			/* not sufficient permissions */
			session_reset_old();
			masterstate = mainidle;
			busy &= ~1;
			lop_sendmessage(&lop1, 5, reply);
			return;
		}else{
			/* length ok, permissions ok, now do the real stuff */
			/* first the parameter free ones */
			if(action==mainopen || action==mainclose || action==keymigrate){
				switch (action){
					case mainopen:   main_open(); break;
					case mainclose:  main_close(); break;
					case keymigrate: keymigration(); break;
					default: /* ERROR */ break;
				}
				reply[4]=DONE;
				lop_sendmessage(&lop1, 5, reply);
			} else {
			/*	
				char name[msg[4]+1];
				strncpy(name,(char*)&(msg[5]), msg[4]);
				name[msg[4]] = '\0';
				switch (action){
					case remuser:    rem_user(name); break;
					case lockuser:   lock_user(name); break;
					case unlockuser: unlock_user(name); break;
					case addadmin:   add_admin(name); break;
					case remadmin:   rem_admin(name); break;
					default: / * ERROR * / break;
				}
				if(action==adduser){
					uint8_t addreply[5+sizeof(authblock_t)]={
						TERMINALUNIT_ID,
						MASTERUNIT_ID,
						MSGID_ACTION_REPLY,
						ACTION_ADDUSER,
						DONE, 0};
					add_user(name, msg+len-sizeof(sha256_hash_t), msg[len-2], msg[len-1], (authblock_t*)&(addreply[5]));
//					add_user(name, msg[len-1]?true:false, (authblock_t*)&(addreply[5])); 
					lop_sendmessage(&lop1, 5+sizeof(authblock_t), addreply);
				}else{
					reply[4]=DONE;
					lop_sendmessage(&lop1, 5, reply);
				}
			*/	
			}
			return;
		}/* if(!check_permissions) ... else ... */
	} /* if(msg[2] == MSGID_ACTION) */
	
	if(msg[2] == MSGID_ADD_BOOTSTRAP){
		uint8_t t; /* for bootstrap_accounts */
		/* check form and length */
		if(NO_ANON_ADMINS && ((msg[len-2])?1:0)){
			session_reset_old();
			masterstate = mainidle;
			busy &= ~1;
			return;
		}
		if((t=eeprom_read_byte(&bootstrap_accounts))!=0){
			/* yeah, let's bootstrap the system */
	//		lop_dbg_str_P(&lop0,PSTR("brapX"));
			eeprom_write_byte(&bootstrap_accounts, t-1);
			char name[msg[3]+1];
			strncpy(name,(char*)&(msg[4]), msg[3]);
			name[msg[3]] = '\0';
			uint8_t addreply[5+sizeof(authblock_t)]={
						TERMINALUNIT_ID,
						MASTERUNIT_ID,
						MSGID_ACTION_REPLY,
						ACTION_ADDUSER,
						DONE, 0};
			add_user(name, msg+len-2-sizeof(sha256_hash_t), msg[len-2]?true:false, msg[len-1], (authblock_t*)&(addreply[5])); 
			/* make user admin */
			userflags_t uf;
			ticketdb_getUserFlags(((authblock_t*)&(addreply[5]))->uid ,&uf);
			uf.admin = true;
			ticketdb_setUserFlags(((authblock_t*)&(addreply[5]))->uid ,&uf);
			lop_sendmessage(&lop1, 5+sizeof(authblock_t), addreply);
		} else {
			/* won't give a bootstrap account */
			session_reset_old();
			masterstate = mainidle;
			busy &= ~1;
			return;
		}
	}
} /* void messagerx */

/******************************************************************************/

void init_system(void){
	door_init();
	session_reset_old();
	masterstate = idle;
	uart_init();
//	uart_putstr_P(PSTR("\r\nuart works (a)\r\n"));


	lop0.msgbuffer = NULL;
	lop0.msgidx = 0;
	lop0.on_streamrx = lop0_streamrx;
	lop0.sendrawbyte = lop0_sendrawbyte;
	lop0.on_msgrx = lop0_messagerx;
	
	qp0.keystate = unkeyed;
	qp0.master_enc_key_rxa = qp0_key_rxa;
	qp0.master_enc_key_rxb = qp0_key_rxb;
	qp0.master_enc_key_txa = qp0_key_txa;
	qp0.master_enc_key_txb = qp0_key_txb;
	qp0.master_mac_key = qp0_key_hmac;
	qp0.on_byterx = qp0_streamrx;
	qp0.lop = &lop0;
	qp0.keyingdata = 0;

	lop1.on_streamrx = lop1_streamrx;
	lop1.sendrawbyte = lop1_sendrawbyte;
	lop1.on_msgrx =    lop1_messagerx;
	lop_sendreset(&lop0);
	uart_hook = onuartrx;
/*
	lop0.sendrawbyte = lop0_sendrawbyte;
	lop0.on_streamrx = streamrx;
	lop0.on_msgrx = messagerx;
	uart_hook = onuartrx;
	lop_recieve_byte(&lop0, LOP_RESET_CODE);
*/	
	uart_putc(XON);
//	uart_putstr_P(PSTR("\r\nuart works\r\n"));
	
	if(qp0.keystate == unkeyed)
		qport_rekey(&qp0);
//	while(1);
    i2c_init();
    E24C_init();
	rtc_init();
    resetcnt_inc();
	DS("\r\nprinter init ... ");
	DS("\r\n2nd msg ");
    printer_init();
	DS(" finished\r\n");
    prng_init();
	#ifdef TAMPER_DETECTION
	tamperdetect_init();
	#endif
	DS("-_-_-_-");
	DS("\r\ninit finished\r\n");
}

/******************************************************************************/

int main(void){
	init_system();	
	if (!setup_done()){
		setup_system();
	} else {
		DS("\r\nOPERATION CONTINUED\r\n");
	}
    load_eeprom_crypt_key(eeprom_key);
	ticketdb_init();
	
	setred();
	DS("time: ");
	{
		timestamp_t t;
		t=gettimestamp();
		DD(&t, sizeof(timestamp_t));
	//	DS("\r\n\n\n\n");
	}
	while(1){
		_delay_ms(50);
	}
	
	return 0;
}

/******************************************************************************/

