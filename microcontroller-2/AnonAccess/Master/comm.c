/* comm.c */
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


/**
 * 
 * 
 * 
 * 
 * 
 * 
 */
 
#include "config.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "lop.h"
#include "lop_debug.h"
#include "types.h"
#include "sha256.h"
#include "rtc.h"
#include "ticketDB.h"
#include "flmDB.h"
#include "reqvalidator.h"
#include "action.h"
#include "system_stats.h"
#include "comm.h"


#define SESSION_MAX_PARTICIPANTS 5
#define SESSION_MAX_IDLE (10LL * 60LL * 1000LL)     /* 10 minutes */
#define SESSION_MAX_DURATION (45LL * 60LL * 1000LL) /* 45 minutes */

#define DS(a) (send_str(TERMINALUNIT_ID, PSTR(a), STR_CLASS_DBG_P))	


#ifndef MAX
 #define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef MIN
 #define MIN(a,b) (((a)<(b))?(a):(b))
#endif

extern lop_ctx_t lop1;
volatile uint8_t msg_wait;
uint16_t msg_length;
void* msg_data;

typedef struct {
	uint8_t in_session;
	uint8_t terminal;
	uint8_t users;
	uint8_t admins;
	uint16_t participants[SESSION_MAX_PARTICIPANTS];
	uint8_t admin_status[(SESSION_MAX_PARTICIPANTS+7)/8];
	timestamp_t starttime, lasttime;
} master_state_t;

master_state_t master_state;

uint8_t session_reset(uint16_t length, uint8_t* msg, master_state_t* mstate){
	uint8_t i;
	mstate->terminal = 1;
	mstate->users = 0; /* also include the admins*/
	mstate->admins = 0;
	mstate->starttime = msg?gettimestamp():0; /* so we can securely init the box */
	mstate->lasttime = mstate->starttime;
	mstate->in_session = msg?1:0;
	for(i=0; i<SESSION_MAX_PARTICIPANTS; ++i){
		mstate->participants[i] = NO_USER;
	}
	for(i=0; i<(SESSION_MAX_PARTICIPANTS+7)/8; ++i){
		mstate->admin_status[i]=0;
	}
	return 0;
}

uint8_t session_reset_validate(uint16_t length, uint8_t* msg){
	return (length==3)?0:1;
}

void session_error_reply(uint8_t msg_id, uint8_t reason, master_state_t* mstate){
	uint8_t msg[4];
	msg[0] = mstate->terminal;
	msg[1] = MASTERUNIT_ID;
	msg[2] = msg_id;
	msg[3] = reason;
	lop_sendmessage(&lop1, 4, msg);
}

uint8_t session_addAB(uint16_t length, uint8_t* msg, master_state_t* mstate){
	authblock_t* ab;
	uint8_t req_admin=0;
	uint8_t i;

	DS("add AB");	
	ab=(authblock_t*)(msg+3);
//	if(length==3+sizeof(authblock_t)+1)
		req_admin=(msg[length-1])?1:0;
	if(!mstate->in_session)
		return 1; /* DROP */
	DS("(1/n)");	

	if(gettimestamp()-mstate->starttime > SESSION_MAX_DURATION){
		session_error_reply(MSGID_AB_ERROR, AB_ERROR_TIMEOUT, mstate);
		mstate->in_session = 0;
		return 4;
	}
	DS("(2/n)");	

	if(gettimestamp()-mstate->lasttime > SESSION_MAX_IDLE){
		session_error_reply(MSGID_AB_ERROR, AB_ERROR_TIMEOUT, mstate);
		mstate->in_session = 0;
		return 5;
	}	
	DS("(3/n)");	
	mstate->lasttime=gettimestamp();
	if(mstate->users==SESSION_MAX_PARTICIPANTS){
		session_error_reply(MSGID_AB_ERROR, AB_ERROR_TOO_MANY_PARTICIPANTS, mstate);
		return 2;
	}
	DS("(4/n)");	
	
	/* check if user is already participating */
	for(i=0; i<(mstate->users); ++i){
		if(mstate->participants[i] == ab->uid){
			session_error_reply(MSGID_AB_ERROR, AB_ERROR_ALLREADY_PARTICIPATING, mstate);
			return 3;
		}
	}
	uint16_t tmp_i;
	DS("(5/n)");	
	for(tmp_i=0; tmp_i<3000; ++tmp_i){
		_delay_ms(1);
	}
	authcredvalid_state_t avst;
	avst = check_authblock(ab);
	DS("(5.1/n)");	
	switch(avst){
		case invalidtimeout_cred:
			DS("(5.2/n)");	
			session_error_reply(MSGID_AB_ERROR, AB_ERROR_AUTHBLOCK_TIMEOUT, mstate);
			return 6;
		case invalid_cred:
			DS("(5.2/n)");	
			session_error_reply(MSGID_AB_ERROR, AB_ERROR_AUTHBLOCK, mstate);
			return 5;
		default:
			break;
	}
	DS("(6/n)");		
	/* send back new AB */
	msg[0] = mstate->terminal;
	msg[1] = MASTERUNIT_ID;
	msg[2] = MSGID_AB_REPLY;
	msg[3+sizeof(authblock_t)] = (avst==valid_user_lostadm)?1:0; 
	lop_sendmessage(&lop1, 3+sizeof(authblock_t)+1, msg);
	DS("(7/n)");	
	if(pin_required(ab->uid, req_admin)){
		msg[2] = MSGID_AB_PINREQ;
		lop_sendmessage(&lop1, 3, msg);
		DS("(7.1/n)");	
		if(waitformessage(10L*60L*1000L)){ /* 10 minute to enter the pin */
			/* pin not given, timeout occured */
			session_error_reply(MSGID_AB_ERROR, AB_ERROR_PINTIMEOUT, mstate);
			return 6;
		}
		DS("(7.2/n)");	
		if(msg_length!=3+sizeof(sha256_hash_t)){
			session_error_reply(MSGID_AB_ERROR, AB_ERROR_WONTTELL, mstate);
			freemsg();
			return 7;
		}
		DS("(7.3/n)");	
		if(((uint8_t*)msg_data)[2]!=MSGID_AB_PINREPLY){
			session_error_reply(MSGID_AB_ERROR, AB_ERROR_WONTTELL, mstate);
			freemsg();
			return 8;
		}
		DS("(7.4/n)");	
		mstate->lasttime=gettimestamp();
		if(!check_pin(ab, (uint8_t*)msg_data+3)){
			session_error_reply(MSGID_AB_ERROR, AB_ERROR_PIN, mstate);
			freemsg();
			return 9;
		}
		DS("(7.5/n)");	
		freemsg();	
	}
	DS("(8/n)");	
	mstate->participants[mstate->users++]=ab->uid;
	if(req_admin && (avst==valid_admin)){
		DS("(8.1/n)");	
		mstate->admins++;
		mstate->admin_status[(mstate->users-1)/8] |= 1<<((mstate->users-1)%8);
	}
	DS("(9/n)");	
	
	return 0;
}

uint8_t session_addAB_validate(uint16_t length, uint8_t* msg){
	DS("AB preval");
	if(length==3+sizeof(authblock_t)+1)
		return 0;
	return 1;	
}

uint8_t session_action(uint16_t length, uint8_t* msg, master_state_t* mstate){
	if(!mstate->in_session)
		return 1;
	if((gettimestamp() > mstate->starttime + SESSION_MAX_DURATION) ||
	   (gettimestamp() > mstate->lasttime + SESSION_MAX_IDLE)){
		mstate->in_session = 0;
		return 2;
	}
	action_t action;
	action = (msg[3]&0xf0)?(msg[3]-0x10+2):msg[3]; /* transform ACTION_* in action_t */
	if(final_action(action)){
		mstate->in_session = 0;
	}
//	msg[2]=MSGID_ACTION_REPLY;
	if(check_permissions(mstate->users, mstate->admins, action)==false){
		lop_sendmessage(&lop1, 5, (uint8_t[5])
		                {msg[1], MASTERUNIT_ID, MSGID_ACTION_REPLY, msg[3], NOTDONE});
	}
	/* now we can perform the action */
	if(action!=adduser && action<getinfo){
		uint8_t reply[5];
		uint8_t n=0;
		reply[0] = msg[1];
		reply[1] = MASTERUNIT_ID;
		reply[2] = MSGID_ACTION_REPLY;
		reply[3] = msg[3];
	
		switch (action){
			case mainopen:   n=main_open(); break;
			case mainclose:  n=main_close(); break;
			case keymigrate: n=keymigration(); break;
			case remuser:    n=rem_user(msg+4); break;
			case lockuser:   n=lock_user(msg+4); break;
			case unlockuser: n=unlock_user(msg+4); break;
			case addadmin:   n=add_admin(msg+4); break;
			case remadmin:   n=rem_admin(msg+4); break;
			case locknick:   n=lock_nick(msg+4); break;
			case opennick:   n=open_nick(msg+4); break;
			
			default: /* ERROR */ break;
		}
		msg[4]=(n==0)?DONE:NOTDONE;
		lop_sendmessage(&lop1, 5, reply);
		return 0;
	} else {
		if(action==adduser){
			char name[msg[4]+1]; 
			uint8_t addreply[5+sizeof(authblock_t)]={
				msg[1],
				MASTERUNIT_ID,
				MSGID_ACTION_REPLY,
				ACTION_ADDUSER,
				DONE, 0};
			memcpy(name, msg+5, msg[4]);
			name[msg[4]] = '\0';
			add_user(name, msg+4+msg[4]+1, msg[length-2], msg[length-1], (authblock_t*)&(addreply[5]));
			lop_sendmessage(&lop1, 5+sizeof(authblock_t), addreply);
			return 0;
		}
		if(action==getinfo){
			/* currently not implemented */
			lop_sendmessage(&lop1, 5, (uint8_t[5])
		           {msg[1], MASTERUNIT_ID, MSGID_ACTION_REPLY, msg[3], NOTDONE});
			return 0;
		}
		if(action==search || action==searchcont){
			/* currently not implemented */
			lop_sendmessage(&lop1, 5, (uint8_t[5])
		           {msg[1], MASTERUNIT_ID, MSGID_ACTION_REPLY, msg[3], NOTDONE});
			return 0;
		}
		if(action==getstats){
			uint8_t reply[5+54];
			reply[0]=msg[1];
			reply[1]=MASTERUNIT_ID;
			reply[2]=MSGID_ACTION_REPLY;
			reply[3]=ACTION_GETSTATS;
			reply[4]=DONE;
			*((uint16_t*)(reply+ 5))=ticketdb_getstatMaxUsers();
			*((uint16_t*)(reply+ 7))=ticketdb_getstatUsers();
			*((uint16_t*)(reply+ 9))=ticketdb_getstatAdmins();
			*((uint16_t*)(reply+11))=ticketdb_getstatLockedUsers();
			*((uint16_t*)(reply+12))=ticketdb_getstatLockedAdmins();
			*((uint16_t*)(reply+13))=0;
			*((uint16_t*)(reply+15))=FLMDB_SIZE/sizeof(flmdb_entry_t);
			*((uint64_t*)(reply+17))=gettimestamp();
			system_hash(reply+19);
			lop_sendmessage(&lop1, 54, reply);
			return 0;
		}
	}
	return 3;
}

uint8_t session_action_validate(uint16_t length, uint8_t* msg){
	DS("action preval");
	uint16_t tmp_i;
	for(tmp_i=0; tmp_i<3000; ++tmp_i){
		_delay_ms(1);
	}
	/* if no <admin/user> flag is given, maybe removed later */
	if(length<4)
		return 1;
	if(msg[3] == ACTION_MAINOPEN   || 
	   msg[3] == ACTION_MAINCLOSE  || 
	   msg[3] == ACTION_GETSTATS   || 
	   msg[3] == ACTION_SEARCHCONT || 
	   msg[3] == ACTION_KEYMIGRATION){
		return (length==4)?0:2;
	}
	if(msg[3] < ACTION_MIN_N || msg[3] > ACTION_MAX_N)
		return 3;
	uint8_t i;
	switch(msg[3]){
		/* actions taking only uid (num or nickname) as parameter */
		case ACTION_REMUSER:
		case ACTION_LOCKUSER:
		case ACTION_UNLOCKUSER:
		case ACTION_ADDADMIN:
		case ACTION_REMADMIN:
		case ACTION_GETINFO:
			if(length<6)
				return 4;
		/* check if nick is given correctly */		
		case ACTION_LOCKNICK:
		case ACTION_OPENNICK:
			if(length==5+msg[4]){
				for(i=0; i<msg[4]; ++i){
					if(msg[5+i]=='\0')
						return 5;
				}
				return 0;
			}
			return 6;
		case ACTION_ADDUSER:
				if(length<7)
					return 8;
				if(5+msg[4]+sizeof(sha256_hash_t)+1+1!=length)
					return 9;
				for(i=0; i<msg[4]; ++i){
					if(msg[5+i]=='\0')
					return 10;
				}
				/* check anon/nonanon */
				if(msg[length-2] > 1)
					return 11;
				/* check pinflags */
				if(msg[length-1] > 3)
					return 12;
				return 0;
		 case ACTION_SEARCH:
		 	if(length<8)
		 		return 13;
		 	if(msg[4]==0 || msg[5]>1)
		 		return 14;
		 	if(length!=6+msg[6]);
		 	for(i=0; i<msg[6]; ++i){
				if(msg[7+i]=='\0')
				return 15;
			}
			return 0;
		default:
			return 16;
	}	
}

uint8_t session_getbootstrap(uint16_t length, uint8_t* msg, master_state_t* mstate){
	uint8_t reply[5+sizeof(authblock_t)];
	char* nick;
	uint8_t nick_len;
	nick_len=msg[3];
	nick = malloc(nick_len+1);
	memcpy(nick, msg+4, nick_len);
	nick[nick_len]=0;
	send_str(TERMINALUNIT_ID, PSTR("started"), STR_CLASS_INFO_P);
	new_account((authblock_t*)(reply+5), nick, msg+3+nick_len,msg[3+nick_len+32], msg[3+nick_len+32+1]);
	send_str(TERMINALUNIT_ID, PSTR("done (1/n)"), STR_CLASS_INFO_P);
	reply[0] = TERMINALUNIT_ID;
	reply[1] = MASTERUNIT_ID;
	reply[2] = MSGID_ACTION_REPLY;
	reply[3] = ACTION_ADDUSER;
	reply[4] = DONE;
	send_str(TERMINALUNIT_ID, PSTR("done (2/n)"), STR_CLASS_INFO_P);
	lop_sendmessage(&lop1, 5+sizeof(authblock_t), reply);
	send_str(TERMINALUNIT_ID, PSTR("done (n/n)"), STR_CLASS_INFO_P);
	session_reset(0,NULL,mstate);
	return 0;
}

uint8_t session_getbootstrap_validate(uint16_t length, uint8_t* msg){
	if(length<6)
		return 1;
	if(4+msg[3]+sizeof(sha256_hash_t)+1+1!=length)
		return 2;
	uint8_t i;
	for(i=0; i<msg[3]; ++i){
		if(msg[4+i]=='\0')
		return 3;
	}
	/* check anon/nonanon */
	if(msg[length-2] > 1)
		return 4;
	/* check pinflags */
	if(msg[length-1] > 3)
		return 5;
		
	return 0;
}
	
typedef uint8_t(*prevalid_func_pt)(uint16_t length, uint8_t *msg);
typedef uint8_t(*command_func_pt)(uint16_t length, uint8_t *msg, master_state_t* mstate);
/*
  0 - session initialisation/reset
  1 - add AuthBlock to session
  2 - AuthBlock reply
  3 - AuthBlock error
  4 - Action
  5 - Action reply
  6 - print string
  7 - get bootstrap account
*  8 - AuthBlock PIN request
*  9 - AuthBlock PIN reply
*/
command_func_pt msg_command_table[MSGID_CNT] PROGMEM = {
	session_reset,
	session_addAB,
	NULL,
	NULL,
	session_action,
	NULL,
	NULL,
	session_getbootstrap,
	NULL,
	NULL
};

prevalid_func_pt msg_prevalid_table[MSGID_CNT] PROGMEM = {
	session_reset_validate,
	session_addAB_validate,
	NULL,
	NULL,
	session_action_validate,
	NULL,
	NULL,
	session_getbootstrap_validate,
	NULL,
	NULL
};

void messagerx(uint16_t length, void* msg){
	if(length<3) 
		return;  /* DROP */
	if((((uint8_t*)(msg))[0] != MASTERUNIT_ID) || 
	   (((uint8_t*)(msg))[1] != TERMINALUNIT_ID))
		return;  /* DROP */
	if(msg_wait){	/* there is a blocking request for a message */
		/* a nicer solution if we have access to the lop struct */
		if((msg_data=lop1.msgbuffer)){
			lop1.msgbuffer=NULL;
			msg_length=length;
		}
		/*
		if((msg_data = malloc(length))){
			memcpy(msg_data, msg, length);
			msg_length = length;
		} else {
			msg_length=0;
		}
		*/
		msg_wait=0;
		return;
	}
	if(((uint8_t*)msg)[2] > MSGID_MAX)
		return; /* DROP */
	command_func_pt  command_func;
	prevalid_func_pt prevalid_func;
	command_func = (command_func_pt)(pgm_read_word(&(msg_command_table[((uint8_t*)msg)[2]])));
	prevalid_func = (prevalid_func_pt)(pgm_read_word(&(msg_prevalid_table[((uint8_t*)msg)[2]])));
	if(!command_func){
		send_str(TERMINALUNIT_ID, PSTR("function not defined"), STR_CLASS_ERROR_P);
		return; /* DROP */
	}
	if(prevalid_func){
		if(prevalid_func(length, msg)!=0){
			send_str(TERMINALUNIT_ID, PSTR("prevalidate failed"), STR_CLASS_ERROR_P);
			return; /* DROP */
		}
	}
	command_func(length, msg, &master_state);
}

uint8_t waitformessage(uint32_t timeout){
	msg_wait=1;
	while(msg_wait && timeout){
		--timeout;
		_delay_ms(1);
	}
	msg_wait=0;	
	if(timeout==0)
		return 1;	
	if(msg_data==0)
		return 2;
	return 0;
}

void freemsg(void){
	if(msg_data)
		free(msg_data);
	msg_length=0;
}

void send_str(uint8_t terminal_id, char* str, uint8_t str_class){
	uint16_t slen; 
	if(str_class&0x10){
		slen = strlen_P(str);
	} else {
	 	slen = strlen(str);
	}
	slen = MIN(slen,255);
	uint8_t msg[3+1+1+slen];
	msg[0] = terminal_id;
	msg[1] = MASTERUNIT_ID;
	msg[2] = MSGID_PRINT;
	msg[3] = (((str_class&0x0F)>STR_CLASS_MAX)?STR_CLASS_NO:str_class)&0x0F;
	msg[4] = slen;
	if(str_class&0x10){
		memcpy_P(msg+5, str, slen);
	} else {
		memcpy(msg+5, str, slen);
	}
	lop_sendmessage(&lop1, sizeof(msg), msg);
}

