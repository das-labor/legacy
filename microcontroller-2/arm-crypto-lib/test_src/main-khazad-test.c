/* main-khazad-test.c */
/*
    This file is part of the AVR-Crypto-Lib.
    Copyright (C) 2011  Daniel Otte (daniel.otte@rub.de)

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
/*
 * khazad test-suit
 * 
*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "cli.h"
#include "dump.h"
#include "uart_lowlevel.h"
#include "sysclock.h"
#include "hw_gptm.h"

#include "khazad.h"
#include "performance_test.h"
#include "bcal-performance.h"
#include "bcal-nessie.h"
#include "bcal_khazad.h"

char* algo_name = "Khazad";

void uart0_putc(char byte){
	uart_putc(UART_0, byte);
}

char uart0_getc(void){
	return uart_getc(UART_0);
}

const bcdesc_t* algolist[] = {
	(bcdesc_t*)&khazad_desc,
	NULL
};

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/

void testrun_nessie_khazad(void){
	bcal_nessie(&khazad_desc);
}

void testrun_performance_khazad(void){
	bcal_performance_multiple(algolist);
}

void test_khazad(void){
	uint8_t key[16];
	uint8_t data[8];
	khazad_ctx_t ctx;

	memset(key, 0, 16);
	memset(data, 0, 8);
	key[0] = 0x80;
	cli_putstr("\r\nkey:   ");
	cli_hexdump(key, 16);
	khazad_init(key, &ctx);
	cli_putstr("\r\nround keys:");
	cli_hexdump_block(&ctx, 8*8, 4, 8);
	cli_putstr("\r\nplain:  ");
	cli_hexdump(data, 8);
	khazad_enc(data, &ctx);
	cli_putstr("\r\nencrypt:");
	cli_hexdump(data, 8);
	khazad_dec(data, &ctx);
	cli_putstr("\r\ndecrypt:");
	cli_hexdump(data, 8);
}

void test_sbox(void){
	uint8_t i=0,x;
	cli_putstr("\r\nKhazad Sbox:\r\n\t");
	do{
		x = khazad_sbox(i);
		cli_hexdump_byte(x);
		cli_putc(' ');
		if(i%16==15){
			cli_putstr("\r\n\t");
		}
		++i;
	}while(i);
}

/*****************************************************************************
 *  main																	 *
 *****************************************************************************/

const char nessie_str[]      = "nessie";
const char test_str[]        = "test";
const char test_sbox_str[]   = "test_sbox";
const char performance_str[] = "performance";
const char echo_str[]        = "echo";

cmdlist_entry_t cmdlist[] = {
	{ nessie_str,      NULL, testrun_nessie_khazad},
	{ test_str,        NULL, test_khazad},
	{ test_sbox_str,   NULL, test_sbox},
	{ performance_str, NULL, testrun_performance_khazad},
	{ echo_str,    (void*)1, (void_fpt)echo_ctrl},
	{ NULL,            NULL, NULL}
};

int main (void){
	sysclk_set_freq(SYS_FREQ);
	sysclk_mosc_verify_enable();
	uart_init(UART_0, 115200, 8, UART_PARATY_NONE, UART_STOPBITS_ONE);
	gptm_set_timer_32periodic(TIMER0);

	cli_rx = uart0_getc;
	cli_tx = uart0_putc;
	
	for(;;){
		cli_putstr("\r\n\r\nARM-Crypto-Lib VS (");
		cli_putstr(algo_name);
		cli_putstr("; ");
		cli_putstr(__DATE__);
		cli_putc(' ');
		cli_putstr(__TIME__);
		cli_putstr(")\r\nloaded and running\r\n");
		cmd_interface(cmdlist);
	}
}
