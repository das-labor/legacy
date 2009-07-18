/* dump.c */
/*
    This file is part of the AVR-Crypto-Lib.
    Copyright (C) 2008  Daniel Otte (daniel.otte@rub.de)

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
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "cli.h" 
#include "string-extras.h" 
 
#define DUMP_WIDTH 16

void pgm_read_block(void* buffer, uint32_t addr, uint8_t length){
	while(length--){
#ifdef pgm_read_byte_far				
		*((uint8_t*)buffer) = (uint8_t)pgm_read_byte_far(addr);
#else
		*((uint8_t*)buffer) = (uint8_t)pgm_read_byte((uint16_t)addr);
#endif
		buffer = (uint8_t*)buffer +1;
		++addr;
	}
}

void ee_read_block(void* buffer, uint32_t addr, uint8_t length){
	eeprom_read_block(buffer, (void*)((uint16_t)addr), length);
}

void ram_read_block(void* buffer, uint32_t addr, uint8_t length){
	memcpy(buffer, (void*)((uint16_t)addr), length);
} 

const char flash_trigger[]  PROGMEM = "fF";
const char ram_trigger[]    PROGMEM = "sSrRmM";
const char eeprom_trigger[] PROGMEM = "eE";

const char flash_desc[]  PROGMEM = "flash";
const char ram_desc[]    PROGMEM = "ram";
const char eeprom_desc[] PROGMEM = "eeprom";

typedef struct {
	void (*fpt)(void*, uint32_t, uint8_t);
	PGM_P trigger;
	PGM_P desc;
} memtype_desc_t;

memtype_desc_t memtype_desc[] PROGMEM = {
	{ pgm_read_block,  flash_trigger,   flash_desc  },
	{ ee_read_block,   eeprom_trigger,  eeprom_desc },
	{ ram_read_block,  ram_trigger,     ram_desc    },
	{ NULL,            NULL,            NULL        }
};

uint8_t charisinstr_P(char c, PGM_P str){
	char t;
	do{
		t=pgm_read_byte(str++);
		if(t==c)
			return 1;
	}while(t);
	return 0;
} 

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
	uint8_t mem_idx=0;
	uint8_t readlen;
	uint32_t addr=0;
	uint32_t size=128;
	uint8_t buffer[DUMP_WIDTH];
	PGM_P ptemp;
	char tstr[9];
	s=strstrip(s);
	do{
		ptemp = (void*)pgm_read_word(&(memtype_desc[mem_idx].trigger));
		if(charisinstr_P(*s, ptemp))
			break;
		++mem_idx;
	}while(ptemp);
	if(ptemp==NULL)
		mem_idx=0;	
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
	ptemp = (void*)pgm_read_word(&(memtype_desc[mem_idx].desc));
	cli_putstr_P(PSTR("\r\ndumping "));	
	ultoa(size, tstr, 10);
	cli_putstr(tstr);
	cli_putstr_P(PSTR(" bytes of "));
	cli_putstr_P(ptemp);
	cli_putstr_P(PSTR(", beginning at 0x"));	
	ultoa(addr, tstr, 16);
	cli_putstr(tstr);
	cli_putstr_P(PSTR(":\r\n"));
	ptemp = (void*)pgm_read_word(&(memtype_desc[mem_idx].fpt));
	uint8_t t;
	while(size){
		readlen = (size>DUMP_WIDTH)?DUMP_WIDTH:size;
		((void(*)(void*,uint32_t,uint8_t))((uint16_t)ptemp))(buffer, addr, readlen);
		print_aligned(addr, 6);
		cli_putstr_P(PSTR(": "));	
		cli_hexdump2(buffer, readlen);
		t=(DUMP_WIDTH-readlen)*3;
		while(t--){
			cli_putc(' ');
		}
		cli_putc('\t');
		dump_chars(buffer,readlen);
		addr+=readlen;
		size-=readlen;
		cli_putstr_P(PSTR("\r\n"));
	}
}

