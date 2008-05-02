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
#include <util/delay.h>
#include "lop.h"
#include "lop_debug.h"
#include "types.h"
#include "sha256.h"
#include "comm.h"

extern lop_ctx_t lop0;
uint8_t msg_wait;
uint16_t msg_length;
void* msg_data;


void init_session(void){
	uint8_t msg[3]={
		MASTERUNIT_ID,
		TERMINALUNIT_ID,
		MSGID_SESSION_INIT
	};
	lop_sendmessage(&lop0, 3, msg);
} 
 
void submit_ab(authblock_t * ab){
	uint8_t msg[3+sizeof(authblock_t)]={
		MASTERUNIT_ID,
		TERMINALUNIT_ID,
		MSGID_ADD_AB,
		0
	};
	memcpy(&(msg[3]), ab, sizeof(authblock_t));
	lop_sendmessage(&lop0, 3+sizeof(authblock_t), msg);
}

void submit_pin(char* pin, uint16_t length_B){
	uint8_t msg[3+sizeof(sha256_hash_t)]={
		MASTERUNIT_ID,
		TERMINALUNIT_ID,
		MSGID_AB_PINREPLY,
		0
	};
	sha256((sha256_hash_t*)((uint8_t*)msg+3), pin, length_B*8);
	lop_sendmessage(&lop0, 3+32, msg);
}


void send_mainopen(void){
	uint8_t msg[4]={
		MASTERUNIT_ID,
		TERMINALUNIT_ID,
		MSGID_ACTION,
		ACTION_MAINOPEN
	};
	lop_sendmessage(&lop0, 4, msg);
}

void send_mainclose(void){
	uint8_t msg[4]={
		MASTERUNIT_ID,
		TERMINALUNIT_ID,
		MSGID_ACTION,
		ACTION_MAINCLOSE
	};
	lop_sendmessage(&lop0, 4, msg);
} 

void req_bootab(char* name, char* pin, uint16_t pinlen_B, uint8_t anon, uint8_t pinflags){
	uint16_t len;
	len = strlen(name);
	if(len>=256){
		len=255;
	}
	uint8_t msg[3+1+len+1]; /*={
		MASTERUNIT_ID,
		TERMINALUNIT_ID,
		MSGID_ADD_BOOTSTRAP,
		0
	};*/
	msg[0]=MASTERUNIT_ID;
	msg[1]=TERMINALUNIT_ID;
	msg[2]=MSGID_ADD_BOOTSTRAP;
	msg[3]=(uint8_t)len;
	memcpy(&(msg[4]), name, len);
	msg[3+1+len]=anon?1:0;
	lop_sendmessage(&lop0, 4+1+len, msg);
}

uint8_t waitformessage(uint16_t timeout){
	msg_wait=1;
	while(msg_wait && timeout--)
		_delay_ms(1);
	msg_wait=0;	
	return !timeout;
}

void freemsg(void){
	free(msg_data);
	msg_length=0;
}

uint8_t getmsgid(void* msg){
	return ((uint8_t*)msg)[2];
}
