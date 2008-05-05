/*
 * 
 * 
 * 
 * 
 * 
 */
#include <stdint.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "lop.h"
#include "lop_debug.h"

void lop_dbg_str(lop_ctx_t * ctx,char * str){
	while(*str){
		lop_sendstream(ctx, *str++);
	}
}

void lop_dbg_str_P(lop_ctx_t * ctx,PGM_P str){
	char tmp;
	while((tmp = pgm_read_byte(str))) {
		lop_sendstream(ctx, tmp);
		str++;
	}
}

void lop_dbg_hexdump(lop_ctx_t * ctx, void * buf, uint16_t len){
	unsigned char table[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};	
	while(len--){
		lop_sendstream(ctx, table[((*((char*)buf))>>4)&0xf]);
		lop_sendstream(ctx, table[ (*((char*)buf))    &0xf]);
		lop_sendstream(ctx, ' ');
		buf=(uint8_t*)buf+1;
	}
}

void terminal_print(lop_ctx_t * ctx, uint8_t class, char* str){
	uint8_t len=strlen(str)+1;
	uint8_t msg[5+len];
	if(class>0x04)
		class=0x00;
	msg[0]=1;
	msg[1]=0;
	msg[2]=6; // MSGID_PRINT 
	msg[3]=class;
	msg[4]=len;
	memcpy(msg+5, str, len);
	lop_sendmessage(ctx, len+5, msg);
}

void terminal_print_P(lop_ctx_t * ctx, uint8_t class, PGM_P str){
	uint8_t len=strlen_P(str)+1;
	uint8_t msg[5+len];
	if(class>0x04)
		class=0x00;
	msg[0]=1;
	msg[1]=0;
	msg[2]=6; // MSGID_PRINT 
	msg[3]=class;
	msg[4]=len;
	memcpy_P(msg+5, str, len);
	lop_sendmessage(ctx, len+5, msg);
}
