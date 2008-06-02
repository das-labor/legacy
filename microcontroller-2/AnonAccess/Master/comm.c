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
#include "reqvalidator.h"
#include "action.h"
#include "comm.h"


#define SESSION_MAX_PARTICIPANTS 5
#define SESSION_MAX_IDLE (10LL * 60LL * 1000LL)     /* 10 minutes */
#define SESSION_MAX_DURATION (45LL * 60LL * 1000LL) /* 45 minutes */


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
	msg[4] = reason;
	lop_sendmessage(&lop1, 4, msg);
}

uint8_t session_addAB(uint16_t length, uint8_t* msg, master_state_t* mstate){
	authblock_t* ab;
	uint8_t req_admin=0;
	uint8_t i;
	
	ab=(authblock_t*)(msg+3);
	if(length==3+sizeof(authblock_t)+1)
		req_admin=(msg[length-1])?1:0;
	if(!mstate->in_session)
		return 1; /* DROP */
	if(gettimestamp()-mstate->starttime > SESSION_MAX_DURATION){
		session_error_reply(MSGID_AB_ERROR, AB_ERROR_TIMEOUT, mstate);
		mstate->in_session = 0;
		return 4;
	}
	if(gettimestamp()-mstate->lasttime > SESSION_MAX_IDLE){
		session_error_reply(MSGID_AB_ERROR, AB_ERROR_TIMEOUT, mstate);
		mstate->in_session = 0;
		return 5;
	}	
	mstate->lasttime=gettimestamp();
	if(mstate->users==SESSION_MAX_PARTICIPANTS){
		session_error_reply(MSGID_AB_ERROR, AB_ERROR_TOO_MANY_PARTICIPANTS, mstate);
		return 2;
	}
	/* check if user is already participating */
	for(i=0; i<mstate->users; ++i){
		if(mstate->participants[i] == ab->uid){
			session_error_reply(MSGID_AB_ERROR, AB_ERROR_ALLREADY_PARTICIPATING, mstate);
			return 3;
		}
	}
	authcredvalid_state_t avst;
	switch(avst=check_authblock(ab)){
		case invalidtimeout_cred:
		case invalid_cred:
			session_error_reply(MSGID_AB_ERROR, AB_ERROR_AUTHBLOCK, mstate);
			return 5;
		default:
			break;
	}
	/* send back new AB */
	msg[0] = mstate->terminal;
	msg[1] = MASTERUNIT_ID;
	msg[2] = MSGID_AB_REPLY;
	lop_sendmessage(&lop1, 3+sizeof(authblock_t), msg);
	if(pin_required(ab->uid, req_admin)){
		msg[2] = MSGID_AB_PINREQ;
		lop_sendmessage(&lop1, 3, msg);
		if(waitformessage(10L*60L*1000L)){ /* 10 minute to enter the pin */
			/* pin not given, timeout occured */
			session_error_reply(MSGID_AB_ERROR, AB_ERROR_PINTIMEOUT, mstate);
			return 6;
		}
		if(msg_length!=3+sizeof(sha256_hash_t)){
			session_error_reply(MSGID_AB_ERROR, AB_ERROR_WONTTELL, mstate);
			freemsg();
			return 7;
		}
		if(((uint8_t*)msg_data)[2]!=MSGID_AB_PINREPLY){
			session_error_reply(MSGID_AB_ERROR, AB_ERROR_WONTTELL, mstate);
			freemsg();
			return 8;
		}
		mstate->lasttime=gettimestamp();
		if(!check_pin(ab, (uint8_t*)msg_data+3)){
			session_error_reply(MSGID_AB_ERROR, AB_ERROR_PIN, mstate);
			freemsg();
			return 9;
		}
		freemsg();	
	}
	/* may implement "admin lost notification" here */
	mstate->participants[mstate->users++]=ab->uid;
	if(req_admin && (avst==valid_admin)){
		mstate->admins++;
		mstate->admin_status[(mstate->users-1)/8] |= 1<<((mstate->users-1)%8);
	}
	return 0;
}

uint8_t session_addAB_validate(uint16_t length, uint8_t* msg){
	/* if no <admin/user> flag is given, maybe removed later */
	if(length==3+sizeof(authblock_t))
		return 0;
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
	msg[2]=MSGID_ACTION_REPLY;
	if(check_permissions(mstate->users, mstate->admins, action)==false){
		msg[4]=NOTDONE;
		lop_sendmessage(&lop1, 5, msg);
	}
	/* now we can perform the action */
	if(action!=adduser){
		switch (action){
			case mainopen:   main_open(); break;
			case mainclose:  main_close(); break;
			case keymigrate: keymigration(); break;
			case remuser:    rem_user(msg+4); break;
			case lockuser:   lock_user(msg+4); break;
			case unlockuser: unlock_user(msg+4); break;
			case addadmin:   add_admin(msg+4); break;
			case remadmin:   rem_admin(msg+4); break;
			default: /* ERROR */ break;
		}
		msg[4]=DONE;
		lop_sendmessage(&lop1, 5, msg);
		return 0;
	} else {
		char name[msg[4]+1]; 
		uint8_t addreply[5+sizeof(authblock_t)]={
			mstate->terminal,
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
	return 3;
}

uint8_t session_action_validate(uint16_t length, uint8_t* msg){
	/* if no <admin/user> flag is given, maybe removed later */
	if(length<4)
		return 1;
	if(msg[3] == ACTION_MAINOPEN || 
	   msg[3] == ACTION_MAINCLOSE || 
	   msg[3] == ACTION_KEYMIGRATION){
		return (length==4)?0:2;
	}
	if(msg[3] < 0x10 || msg[3] > 0x16)
		return 3;
	/* actions taking only uid as parameter */
	if(msg[3] >= 0x11 && msg[3] <= 0x15){
		if(length<6)
			return 4;
		if(msg[4] == 0){
			return (length==7)?0:5;
		} else {
			if(5+msg[4]!=length)
				return 6;
			uint8_t i;
			for(i=0; i<msg[4]; ++i){
				if(msg[5+i]=='\0')
					return 7;
			}
			return 0;
		}		
	}
	if(msg[3]==ACTION_ADDUSER){
		if(length<7)
			return 8;
		if(5+msg[4]+sizeof(sha256_hash_t)+1+1!=length)
			return 9;
		uint8_t i;
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
	}
	return 1;	
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

command_func_pt msg_command_table[MSGID_CNT] PROGMEM = {
	session_reset,
	session_addAB,
	NULL
};

prevalid_func_pt msg_prevalid_table[MSGID_CNT] PROGMEM = {
	session_reset_validate,
	session_addAB_validate,
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
	if(!command_func)
		return; /* DROP */
	if(prevalid_func){
		if(prevalid_func(length, msg)!=0)
			return; /* DROP */
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

#ifndef MAX
 #define MAX(a,b) (((a)>(b))?(a):(b))
#endif

void send_str(uint8_t terminal_id, char* str, uint8_t str_class){
	uint16_t slen = strlen(str);
	slen = MAX(slen,255);
	uint8_t msg[3+1+1+slen];
	msg[0] = terminal_id;
	msg[1] = MASTERUNIT_ID;
	msg[3] = MSGID_PRINT;
	msg[4] = (str_class>STR_CLASS_MAX)?STR_CLASS_NO:str_class;
	memcpy(msg+5, str, slen);
	lop_sendmessage(&lop1, sizeof(msg), msg);
}

