/* bigint_io.c */
/*
    This file is part of the AVR-Crypto-Lib.
    Copyright (C) 2010 Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "cli.h"
#include "hexdigit_tab.h"
#include "bigint.h"
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

void bigint_print_hex(const bigint_t* a){
	if(a->length_B==0){
		cli_putc('0');
		return;
	}
	if(a->info&BIGINT_NEG_MASK){
		cli_putc('-');
	}
//	cli_putc((a->info&BIGINT_NEG_MASK)?'-':'+'); /* print sign */
	if(a->wordv[a->length_B-1]<0x10){
		cli_putc(pgm_read_byte(hexdigit_tab_uc_P+a->wordv[a->length_B-1]));
		cli_hexdump_rev(a->wordv, a->length_B-1);
	} else {
		cli_hexdump_rev(a->wordv, a->length_B);
	}
}

#define BLOCKSIZE 20

static uint8_t char2nibble(char c){
	if(c>='0' && c <='9'){
		return c-'0';
	}
	c |= 'A'^'a'; /* to lower case */
	if(c>='a' && c <='f'){
		return c-'a'+10;
	}
	return 0xff;
}

static uint16_t read_byte(void){
	uint8_t t1, t2;
	char c;
	c = cli_getc_cecho();
	if(c=='-'){
		return 0x0500;
	}
	t1 = char2nibble(c);
	if(t1 == 0xff){
		return 0x0100;
	}
	c = cli_getc_cecho();
	t2 = char2nibble(c);
	if(t2 == 0xff){
		return 0x0200|t1;
	}
	return (t1<<4)|t2;
}

uint8_t bigint_read_hex_echo(bigint_t* a){
	uint16_t allocated=0;
	uint8_t  shift4=0;
	uint16_t  t;
	a->length_B = 0;
	a->wordv = NULL;
	a->info = 0;
	for(;;){
		if(allocated-a->length_B < 1){
			uint8_t *p;
			p = realloc(a->wordv, allocated+=BLOCKSIZE);
			if(p==NULL){
				cli_putstr_P(PSTR("\r\nERROR: Out of memory!"));
				free(a->wordv);
				return 0xff;
			}
			a->wordv=p;
		}
		t = read_byte();
		if(a->length_B==0){
			if(t&0x0400){
				/* got minus */
				a->info |= BIGINT_NEG_MASK;
				continue;
			}else{
				if(t==0x0100){
					free(a->wordv);
					a->wordv=NULL;
					return 1;
				}
			}
		}
		if(t<=0x00ff){
			a->wordv[a->length_B++] = (uint8_t)t;
		}else{
			if(t&0x0200){
				shift4 = 1;
				a->wordv[a->length_B++] = (uint8_t)((t&0x0f)<<4);
			}
			break;
		}
	}
	/* we have to reverse the byte array */
	uint8_t tmp;
	uint8_t *p, *q;
	p = a->wordv;
	q = a->wordv+a->length_B-1;
	while(q>p){
		tmp = *p;
		*p = *q;
		*q = tmp;
		p++; q--;
	}
	if(shift4){
		bigint_adjust(a);
		bigint_shiftright(a, 4);
	}
	bigint_adjust(a);
	return 0;
}
