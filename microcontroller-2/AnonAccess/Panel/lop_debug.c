/* lop_debug.c */
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


