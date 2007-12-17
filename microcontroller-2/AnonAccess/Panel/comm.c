/**
 * 
 * 
 * 
 * 
 * 
 * 
 */
 
#include <stdint.h>
#include <string.h>
#include "lop.h"
#include "lop_debug.h"
#include "types.h"
#include "comm.h"

extern lop_ctx_t lop0;

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

void req_bootab(char * name, uint8_t anon){
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



