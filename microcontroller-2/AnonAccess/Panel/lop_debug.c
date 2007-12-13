/*
 * 
 * 
 * 
 * 
 * 
 */
#include <stdint.h>
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


