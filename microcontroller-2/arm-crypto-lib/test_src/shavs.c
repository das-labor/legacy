/* shavs.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2010  Daniel Otte (daniel.otte@rub.de)

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
/**
 * \file	shavs.c
 * \author  Daniel Otte
 * \date    2006-05-16
 * \license	GPLv3 or later
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hashfunction_descriptor.h"
#include "hfal-basic.h"
#include "shavs.h"
#include "string-extras.h"
#include "cli.h"


#ifdef DEBUG
#  undef DEBUG
#endif

#define DEBUG 0

#if DEBUG
#include "uart_lowlevel.h"
//#  include "config.h"
//#  include <util/delay.h>
#endif

hfdesc_t*  shavs_algo=NULL;
hfdesc_t** shavs_algolist=NULL;

void shavs_listalgos(void){
	char option = 'a';

	hfdesc_t* t;
	uint8_t i=0;
	cli_putstr("\r\nthe following algorithms are available:\r\n");
	while(option<='z' && (t=shavs_algolist[i])){
		cli_putc('\t');
		cli_putc((t==shavs_algo)?'*':' ');
		cli_putc(option++);
		cli_putstr(":\t");
		cli_putstr(t->name);
		cli_putstr("\r\n");
		i++;
	}
}

void shavs_setalgo(char* param){
	param = strstrip(param);
	if(param[1]=='\0'){ /* single letter specified */
		uint8_t i,option = param[0]-'a';

		if(!shavs_algolist){
			cli_putstr("\r\nERROR: shavs_algolist not set!");
			return;
		}
		for(i=0; i<=option; ++i){
			if((shavs_algolist[i])==NULL){
				cli_putstr("\r\nERROR: invalid selection!");
				return;
			}
		}
		shavs_algo=(hfdesc_t*)(shavs_algolist[option]);
	} else { /* name specified */
		hfdesc_t* t=NULL;
		uint8_t i=0;
		while((t=shavs_algolist[i]) && strcasecmp(param, t->name)){
			++i;
		}
		if(t){
			shavs_algo=t;
		}else{
			cli_putstr("\r\nERROR: could not find \"");
			cli_putstr(param);
			cli_putstr("\"!");
		}
	}
}

typedef struct {
	uint16_t buffer_idx;
	uint16_t buffersize_B;
	uint32_t blocks;
	hfgen_ctx_t ctx;
	uint8_t* buffer;
	uint8_t  in_byte;
} shavs_ctx_t;

static shavs_ctx_t shavs_ctx;

static
uint8_t buffer_add(char c){
	uint8_t v,t;
	if(shavs_ctx.buffer_idx==shavs_ctx.buffersize_B){
		hfal_hash_nextBlock(&(shavs_ctx.ctx), shavs_ctx.buffer);
		++shavs_ctx.blocks;
		shavs_ctx.buffer_idx=0;
		cli_putc('.');
	}
	if(c>='0' && c<='9'){
		v=c-'0';
	}else{
		c &= (uint8_t)~('a' ^ 'A');
		if(c>='A' && c<='F'){
			v=c-'A'+10;
		}else{
			return 1;
		}
	}
	t=shavs_ctx.buffer[shavs_ctx.buffer_idx];
	if(shavs_ctx.in_byte){
		t |= v;
		shavs_ctx.buffer[shavs_ctx.buffer_idx] = t;
		shavs_ctx.buffer_idx++;
		shavs_ctx.in_byte = 0;
	}else{
		t = v<<4;
		shavs_ctx.buffer[shavs_ctx.buffer_idx] = t;
		shavs_ctx.in_byte = 1;
	}
	return 0;
}

static
uint32_t my_strtoul(const char* str){
	uint32_t r=0;
	while(*str && (*str<'0' || *str>'9')){
		str++;
	}
	if(!*str){
		return 0;
	}
	while(*str && (*str>='0' && *str<='9')){
		r *= 10;
		r += *str-'0';
		str++;
	}
	return r;
}

int32_t getLength(void){
	uint32_t len=0;
	char lenstr[25];
	char* len2;
	for(;;){
		memset(lenstr, 0, 21);
		cli_getsn(lenstr, 20);
		len2 = strstrip(lenstr);
		if(!strncasecmp(len2, "LEN", 3)){
			while(*len2 && *len2!='=')
				len2++;
			if(*len2=='='){
				do{
					len2++;
				}while(*len2 && !isdigit((uint8_t)*len2));
				len = my_strtoul(len2);
				//len=(uint32_t)strtoul(len2, NULL, 10);
				return len;
			}
		} else {
			if(!strncasecmp(len2, "EXIT", 4)){
				return -1;
			}
		}
	}
}

void shavs_test1(void){ /* KAT tests */
	uint32_t length=0;
	int32_t expect_input=0;

	if(!shavs_algo){
			cli_putstr("\r\nERROR: select algorithm first!");
		return;
	}
	char c;
	uint8_t diggest[shavs_algo->hashsize_b/8];
	shavs_ctx.buffersize_B=shavs_algo->blocksize_b/8;
	uint8_t buffer[shavs_ctx.buffersize_B+5];
	shavs_ctx.buffer = buffer;
	cli_putstr("\r\nbuffer_size = 0x");
	cli_hexdump_rev(&(shavs_ctx.buffersize_B), 2);
	cli_putstr(" bytes");
	for(;;){
		shavs_ctx.blocks = 0;
		memset(buffer, 0, shavs_ctx.buffersize_B);
		length = getLength();
		if((int32_t)length<0){
#if DEBUG
			cli_putstr("\r\n(x) Len == ");
			cli_hexdump_rev(&length, 4);
			uart_flush(0);
#endif
			return;
		}

#if DEBUG
		cli_putstr("\r\nLen == ");
		cli_hexdump_rev(&length, 4);
		uart_flush(0);
#endif
		if(length==0){
			expect_input=2;
		}else{
			expect_input=((length + 7) >> 2) & (~1L);
		}
#if DEBUG
		cli_putstr("\r\nexpected_input == ");
		cli_hexdump_rev(&expect_input, 4);
		if(expect_input==0)
			cli_putstr("\r\nexpected_input == 0 !!!");
#endif
		shavs_ctx.buffer_idx = 0;
		shavs_ctx.in_byte    = 0;
		shavs_ctx.blocks     = 0;
		uint8_t ret;
#if DEBUG
		cli_putstr("\r\n HFAL init");
		cli_putstr("\r\n (2) expected_input == ");
		cli_hexdump_rev(&expect_input, 4);
#endif
		ret = hfal_hash_init(shavs_algo, &(shavs_ctx.ctx));
		if(ret){
			cli_putstr("\r\n HFAL init returned with: ");
			cli_hexdump(&ret, 1);
			return;
		}
#if DEBUG
		cli_putstr("\r\n (3) expected_input == ");
		cli_hexdump_rev(&expect_input, 4);
		cli_putstr("\r\n");
#endif
		while((c=cli_getc_cecho())!='M' && c!='m'){
			if(!isblank((uint8_t)c)){
				cli_putstr("\r\nERROR: wrong input (1) [0x");
				cli_hexdump(&c, 1);
				cli_putstr("]!\r\n");
				hfal_hash_free(&(shavs_ctx.ctx));
				return;
			}
		}
		if((c=cli_getc_cecho())!='s' && c!='S'){
				cli_putstr("\r\nERROR: wrong input (2)!\r\n");
				hfal_hash_free(&(shavs_ctx.ctx));
				return;
		}
		if((c=cli_getc_cecho())!='g' && c!='G'){
				cli_putstr("\r\nERROR: wrong input (3)!\r\n");
				hfal_hash_free(&(shavs_ctx.ctx));
				return;
		}
		while((c=cli_getc_cecho())!='='){
			if(!isblank((uint8_t)c)){
				cli_putstr("\r\nERROR: wrong input (4)!\r\n");
				hfal_hash_free(&(shavs_ctx.ctx));
				return;
			}
		}
#if DEBUG
		cli_putstr("\r\nparsing started");
#endif
		shavs_ctx.buffer_idx = 0;
		shavs_ctx.in_byte    = 0;
		shavs_ctx.blocks     = 0;
		while(expect_input>0){
			c=cli_getc_cecho();
#if DEBUG
			cli_putstr("\r\n\t(");
			cli_hexdump_rev(&expect_input, 4);
			cli_putstr(") ");
#endif
			if(buffer_add(c)==0){
				--expect_input;
			}else{
				if(!isblank((uint16_t)c)){
					cli_putstr("\r\nERROR: wrong input (5) (");
					cli_putc(c);
					cli_putstr(")!\r\n");
					hfal_hash_free(&(shavs_ctx.ctx));
					return;
				}
			}
		}
#if DEBUG
		cli_putstr("\r\nBuffer-A:");
		cli_hexdump_block(buffer, shavs_ctx.buffersize_B, 5, 8);

		cli_putstr("\r\n starting finalization");
		cli_putstr("\r\n\tblocks     == ");
		cli_hexdump_rev(&(shavs_ctx.blocks),4);
		cli_putstr("\r\n\tbuffer_idx == ");
		cli_hexdump_rev(&(shavs_ctx.buffer_idx),2);
		cli_putstr("\r\n\tin_byte    == ");
		cli_hexdump_rev(&(shavs_ctx.in_byte),1);

		cli_putstr("\r\n starting last block");
		cli_putstr("\r\n\tlength       == ");
		cli_hexdump_rev(&length,4);
		cli_putstr("\r\n\tbuffersize_B == ");
		cli_hexdump_rev(&(shavs_ctx.buffersize_B),2);
		uint16_t temp=length-(shavs_ctx.blocks)*((shavs_ctx.buffersize_B)*8);
		cli_putstr("\r\n\t (temp)      == ");
		cli_hexdump_rev(&temp,2);
#else
		uint16_t temp=length-(shavs_ctx.blocks)*((shavs_ctx.buffersize_B)*8);
#endif
		hfal_hash_lastBlock( &(shavs_ctx.ctx), buffer, /* be aware of freaking compilers!!! */
		                    temp );
#if DEBUG
		cli_putstr("\r\n starting ctx2hash");
#endif
		hfal_hash_ctx2hash(diggest, &(shavs_ctx.ctx));
#if DEBUG
		cli_putstr("\r\n starting hash free");
#endif
		hfal_hash_free(&(shavs_ctx.ctx));
		cli_putstr("\r\n MD = ");
		cli_hexdump(diggest, shavs_algo->hashsize_b/8);

	}
}

void shavs_test2(void){ /* Monte Carlo tests for SHA-1 & SHA-2 */
	uint16_t expected_input;
	uint16_t count;
	uint8_t v;
	uint8_t index=0;
	char c;
	if(!shavs_algo){
			cli_putstr("\r\nERROR: select algorithm first!");
		return;
	}
	uint8_t ml=shavs_algo->hashsize_b/8;
	uint8_t m[ml*4+8];
	for(;;){
		while((c=cli_getc_cecho())!='S' && c!='s'){
			if(!isblank((uint8_t)c)){
				cli_putstr("\r\nERROR: wrong input (1) [0x");
				cli_hexdump(&c, 1);
				cli_putstr("]!\r\n");
				return;
			}
		}
		if((c=cli_getc_cecho())!='e' && c!='E'){
				cli_putstr("\r\nERROR: wrong input (2)!\r\n");
				return;
		}
		if((c=cli_getc_cecho())!='e' && c!='E'){
				cli_putstr("\r\nERROR: wrong input (3)!\r\n");
				return;
		}
		if((c=cli_getc_cecho())!='d' && c!='D'){
				cli_putstr("\r\nERROR: wrong input (4)!\r\n");
				return;
		}
		while((c=cli_getc_cecho())!='='){
			if(!isblank((uint8_t)c)){
				cli_putstr("\r\nERROR: wrong input (5)!\r\n");
				return;
			}
		}
		expected_input = ml*2;
		memset(m+2*ml, 0, ml);
		do{
			v=0xff;
			c=cli_getc_cecho();
			if(c>='0' && c<='9'){
				v = c - '0';
			}else{
				c |= 'A'^'a';
				if(c>='a' && c<='f'){
					v = c - 'a' +10;
				}
			}
			if(v<0x10){
				c=m[ml*2+index/2];
				if(index&1){
					c |= v;
				}else{
					c |=v<<4;
				}
				m[ml*2+index/2]=c;
				index++;
				expected_input--;
			}
		}while(expected_input);
		/* so we have the seed */
		cli_putstr("\r\nstarting processing");
		uint16_t j;
		for(count=0; count<100; ++count){
			memcpy(m, m+ml*2, ml);
			memcpy(m+ml, m+ml*2, ml);
			for(j=0; j<1000; ++j){
				hfal_hash_mem(shavs_algo, m+ml*3, m, ml*3*8);
				memmove(m, m+ml, 3*ml);
			}
			cli_putstr("\r\n\r\nCOUNT = ");
			if(count>=10){
				cli_putc(count/10+'0');
			}
			cli_putc(count%10+'0');
			cli_putstr("\r\nMD = ");
			cli_hexdump(m+ml*2, ml);
		}
	}
}

void shavs_test3(void){ /* Monte Carlo tests for SHA-3 */
	uint16_t expected_input;
	uint16_t count;
	uint8_t v;
	uint8_t index=0;
	char c;
	if(!shavs_algo){
			cli_putstr("\r\nERROR: select algorithm first!");
		return;
	}
	uint8_t ml=shavs_algo->hashsize_b/8;
	uint8_t m[ml+128];
	for(;;){
		while((c=cli_getc_cecho())!='S' && c!='s'){
			if(!isblank((uint8_t)c)){
				cli_putstr("\r\nERROR: wrong input (1) [0x");
				cli_hexdump(&c, 1);
				cli_putstr("]!\r\n");
				return;
			}
		}
		if((c=cli_getc_cecho())!='e' && c!='E'){
				cli_putstr("\r\nERROR: wrong input (2)!\r\n");
				return;
		}
		if((c=cli_getc_cecho())!='e' && c!='E'){
				cli_putstr("\r\nERROR: wrong input (3)!\r\n");
				return;
		}
		if((c=cli_getc_cecho())!='d' && c!='D'){
				cli_putstr("\r\nERROR: wrong input (4)!\r\n");
				return;
		}
		while((c=cli_getc_cecho())!='='){
			if(!isblank((uint8_t)c)){
				cli_putstr("\r\nERROR: wrong input (5)!\r\n");
				return;
			}
		}
		expected_input = 1024/4;
		memset(m+ml, 0, 1024/8);
		do{
			v=0xff;
			c=cli_getc_cecho();
			if(c>='0' && c<='9'){
				v = c - '0';
			}else{
				c |= 'A'^'a';
				if(c>='a' && c<='f'){
					v = c - 'a' +10;
				}
			}
			if(v<0x10){
				c=m[ml+index/2];
				if(index&1){
					c |= v;
				}else{
					c |=v<<4;
				}
				m[ml+index/2]=c;
				index++;
				expected_input--;
			}
		}while(expected_input);
		/* so we have the seed */
		cli_putstr("\r\nstarting processing");
		uint16_t j;
		for(count=0; count<100; ++count){
			for(j=0; j<1000; ++j){
				hfal_hash_mem(shavs_algo, m, m+ml, 1024);
				memmove(m+ml, m, 1024/8);
			}
			cli_putstr("\r\n\r\nj = ");
			if(count>=10){
				cli_putc(count/10+'0');
			}
			cli_putc(count%10+'0');
			cli_putstr("\r\nMD = ");
			cli_hexdump(m+ml, ml);

		}
	}
}
