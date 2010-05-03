/* dump.c */
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
 * \file     dump.c
 * \email    daniel.otte@rub.de
 * \author   Daniel Otte 
 * \date     2009-02-04
 * \license  GPLv3 or later
 * 
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>					
#include <ctype.h>
#include "cli.h" 
#include "string-extras.h" 
 
#define DUMP_WIDTH 16

static
void dump_chars(uint8_t* buffer, uint8_t len){
	uint8_t i;
	cli_putc('|');
	for(i=0; i<len; ++i){
		if(isprint(buffer[i])){
			cli_putc(buffer[i]);
		}else{
			cli_putc('.');
		}
	}
	for(;len<DUMP_WIDTH; ++len){
		cli_putc(' ');
	}
	cli_putc('|');
}

static 
void print_aligned(unsigned long value, uint8_t align){
	char str[10];
	uint8_t i;
	ultoa(value, str, 16);
	for(i=strlen(str);i<align;++i)
		cli_putc(' ');
	cli_putstr(str);
}


void dump(char* s){
	uint8_t readlen;
	uint32_t addr=0;
	uint32_t size=32;
	uint8_t buffer[DUMP_WIDTH];
	char tstr[9];
	if(s){
		s=strstrip(s);
		if(isalpha(*s)){
			while(isalpha(*s))
				++s;
		}
		char* eptr;
		if(*s)
			addr = strtoul(s, &eptr, 0);
		if(eptr)
			size = strtoul(eptr, NULL, 0);
		if(!size)
			size = 32;
	}
	cli_putstr("\r\ndumping ");
	ultoa(size, tstr, 10);
	cli_putstr(tstr);
	cli_putstr(" bytes, beginning at 0x");
	ultoa(addr, tstr, 16);
	cli_putstr(tstr);
	cli_putstr(":\r\n");
	uint8_t t;
	while(size){
		readlen = (size>DUMP_WIDTH)?DUMP_WIDTH:size;
		memcpy(buffer, (void*)addr, readlen);
		print_aligned(addr, 6);
		cli_putstr(": ");
		cli_hexdump2(buffer, readlen);
		t=(DUMP_WIDTH-readlen)*3;
		while(t--){
			cli_putc(' ');
		}
		cli_putc('\t');
		dump_chars(buffer,readlen);
		addr+=readlen;
		size-=readlen;
		cli_putstr("\r\n");
	}
}

