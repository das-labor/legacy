/* nessie_common.c */
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
 * 
 * author: Daniel Otte
 * email:  daniel.otte@rub.de
 * license: GPLv3
 * 
 * common function for nessie-tests
 * 
 * */

#include <string.h>
#include <stdint.h>
#include <stdlib.h> /* utoa() */
#include "nessie_common.h"
#include "hexdigit_tab.h"
#include "cli.h"
#include "string-extras.h"


#ifdef NESSIE_ALIVE
void nessie_send_alive(void){
	cli_putc(NESSIE_ALIVE_CHAR);
}

void nessie_send_alive_a(uint16_t i){
	if((i&31)==0)
		cli_putc(NESSIE_ALIVE_CHAR);
}
#endif

void nessie_print_block(uint8_t* block, uint16_t blocksize_bit){
	cli_hexdump(block, (blocksize_bit+7)/8);
}

#define SPACES 31
#define BYTESPERLINE 16

void nessie_print_item(const char* name, uint8_t* buffer, uint16_t size_B){
	uint8_t name_len;
	uint8_t i;
	name_len=strlen(name);
	if(name_len>SPACES-1){
		cli_putstr("\r\n!!! formatting error !!!\r\n");
		return;
	}
	cli_putstr("\r\n");
	for(i=0; i<SPACES-name_len-1; ++i){
		cli_putc(' ');
	}
	cli_putstr(name);
	cli_putc('=');
	/* now the data printing begins */
	if(size_B<=BYTESPERLINE){
		/* one line seems sufficient */
		nessie_print_block(buffer, size_B*8);
	} else {
		/* we need more lines */
		nessie_print_block(buffer, BYTESPERLINE*8); /* first line */
		int16_t toprint = size_B - BYTESPERLINE;
		buffer += BYTESPERLINE;
		while(toprint > 0){
			cli_putstr("\r\n");
			for(i=0; i<SPACES; ++i){
				cli_putc(' ');
			}
			nessie_print_block(buffer, ((toprint>BYTESPERLINE)?BYTESPERLINE:toprint)*8);
			buffer  += BYTESPERLINE;
			toprint -= BYTESPERLINE;
		}
	}
} 


void nessie_print_set_vector(uint8_t set, uint16_t vector){
	cli_putstr("\r\n\r\nSet ");
	cli_putc('0'+set%10);
	cli_putstr(", vector#");
	cli_putc((vector<1000)?' ':'0'+vector/1000);
	cli_putc((vector<100)?' ':'0'+(vector/100)%10);
	cli_putc((vector<10 )?' ':'0'+(vector/10)%10);
	cli_putc('0'+vector%10);
	cli_putc(':');
}

/* example:
Test vectors -- set 3
=====================
 */ 
void nessie_print_setheader(uint8_t set){
	cli_putstr("\r\n\r\nTest vectors -- set ");
	cli_putc('0'+set%10);
	cli_putstr("\r\n=====================");
}

/* example:
********************************************************************************
*Project NESSIE - New European Schemes for Signature, Integrity, and Encryption*
********************************************************************************

Primitive Name: Serpent
=======================
Key size: 256 bits
Block size: 128 bits
*/

void nessie_print_header(const char* name,
                         uint16_t keysize_b, 
                         uint16_t blocksize_b,
                         uint16_t hashsize_b, 
                         uint16_t macsize_b,
                         uint16_t ivsize_b ){
	uint16_t i;
	cli_putstr("\r\n\r\n"
	"********************************************************************************\r\n"
	"* ARM-Crypto-Lib - crypto primitives for ARM microcontrollers by Daniel Otte   *\r\n"
	"********************************************************************************\r\n"
	"\r\n");

	cli_putstr("Primitive Name: ");
	cli_putstr(name);
	cli_putstr("\r\n");
	/* underline */	
	for(i=0; i<16+strlen(name); ++i){
		cli_putc('=');
	}
	char str[6]; /* must catch numbers up to 65535 + terminating \0 */
	if(keysize_b){
		cli_putstr("\r\nKey size: ");
		ustoa(keysize_b, str, 10);
		cli_putstr(str);
		cli_putstr(" bits");
	}
	if(blocksize_b){
		cli_putstr("\r\nBlock size: ");
		ustoa(blocksize_b, str, 10);
		cli_putstr(str);
		cli_putstr(" bits");
	}
	if(hashsize_b){
		cli_putstr("\r\nHash size: ");
		ustoa(hashsize_b, str, 10);
		cli_putstr(str);
		cli_putstr(" bits");
	}

	if(macsize_b){
		cli_putstr("\r\nMac size: ");
		ustoa(macsize_b, str, 10);
		cli_putstr(str);
		cli_putstr(" bits");
	}
	if(ivsize_b){
		if(ivsize_b==(uint16_t)-1){
			cli_putstr("\r\nNo initial value (IV) mode");
		}else{
			cli_putstr("\r\nIV size: ");
			ustoa(ivsize_b, str, 10);
			cli_putstr(str);
			cli_putstr(" bits");
		}
	}
	cli_putstr("\r\n");
}

void nessie_print_footer(void){
	cli_putstr("\r\n\r\n\r\n\r\nEnd of test vectors\r\n\r\n");
}

