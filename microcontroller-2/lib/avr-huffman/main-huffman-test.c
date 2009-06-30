/* main-huffman-test.c */
/*
    This file is part of the AVR-Huffman.
    Copyright (C) 2009  Daniel Otte (daniel.otte@rub.de)

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
#include "dump.h"
#include "avr-huffman-decode.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>


extern uint8_t* _binary_data_hacker_manifesto_borg_txt_hfm_start PROGMEM;
extern uint8_t* _binary_data_test_txt_hfm_start PROGMEM;
extern uint8_t* _binary_data_COPYING_gpl3_hfm_start PROGMEM;


uint16_t read_byte_pgm(uint16_t addr){
	return pgm_read_byte((PGM_VOID_P)addr);
}

void decompress(PGM_VOID_P addr){
	huffman_dec_ctx_t ctx;
	uint16_t c;
	huffman_dec_init(&ctx, read_byte_pgm);
	huffman_dec_set_addr(&ctx, (uint16_t)addr);
	cli_putstr_P(PSTR("\r\ndecompressing data at "));
	cli_hexdump_rev(&addr, 2);
	cli_putstr_P(PSTR("\r\n"));
	for(;;){
		c=huffman_dec_byte(&ctx);
		if(c>0xff){
			cli_putstr_P(PSTR("\r\n"));
			return;
		}
		c&=0xff;
		if(c=='\n'){
			cli_putc('\r');
		}
		cli_putc(c);
	}
}

void decompress_test(void){
	decompress(&_binary_data_test_txt_hfm_start);
}

void decompress_hacker(void){
	decompress(&_binary_data_hacker_manifesto_borg_txt_hfm_start);
}

void decompress_GPL(void){
	decompress(&_binary_data_COPYING_gpl3_hfm_start);
}

/******************************************************************************/

const char hacker_str[]  PROGMEM = "hacker_manifesto";
const char test_str[]    PROGMEM = "test";
const char GPL_str[]     PROGMEM = "gpl_license";
const char dump_str[]    PROGMEM = "dump";
const char echo_str[]    PROGMEM = "echo";

cmdlist_entry_t cmdlist[] PROGMEM = {
	{ test_str,        NULL, decompress_test},
	{ hacker_str,      NULL, decompress_hacker },
	{ GPL_str,         NULL, decompress_GPL}, 
	{ dump_str,    (void*)1, (void_fpt)dump},
	{ echo_str,    (void*)1, (void_fpt)echo_ctrl},
	{ NULL,            NULL, NULL}
};

int main (void){
	uart_init();
		
	cli_rx = uart_getc;
	cli_tx = uart_putc;	 	
	for(;;){
		cli_putstr_P(PSTR("\r\n\r\nAVR-Huffman testing system"));
		cli_putstr_P(PSTR("\r\n  *** loaded and running*** \r\n"));
		cmd_interface(cmdlist);
	}
}

