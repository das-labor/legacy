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
#include "hexdigit_tab.h"

void lop_dbg_str(lop_ctx_t * ctx, const char * str){
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

void lop_dbg_hexdump(lop_ctx_t * ctx, const void * buf, uint16_t len){
	while(len--){
		lop_sendstream(ctx, pgm_read_byte(hexdigit_tab_P+(*((uint8_t*)buf)>>4)));
		lop_sendstream(ctx, pgm_read_byte(hexdigit_tab_P+(*((uint8_t*)buf)&0xf)));
		lop_sendstream(ctx, ' ');
		buf=(uint8_t*)buf+1;
	}
}


