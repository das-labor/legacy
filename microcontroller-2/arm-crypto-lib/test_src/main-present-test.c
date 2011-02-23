/* main-present-test.c */
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
/*
 * present test-suit
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
#include "config.h"

#include <present.h>
#include "nessie_bc_test.h"
#include "performance_test.h"
#include "bcal-performance.h"
#include "bcal_present.h"

const char* algo_name = "Present";

void uart0_putc(char byte){
	uart_putc(UART_0, byte);
}

char uart0_getc(void){
	return uart_getc(UART_0);
}

const bcdesc_t* algolist[] = {
	(bcdesc_t*)&present_desc,
	NULL
};

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/
void present_genctx_dummy(uint8_t* key, uint16_t keysize_b, present_ctx_t* ctx){
	present_init(key, keysize_b, ctx);
}

void testrun_nessie_present(void){
	nessie_bc_ctx.blocksize_B =   8;
	nessie_bc_ctx.keysize_b   =  80;
	nessie_bc_ctx.name        = algo_name;
	nessie_bc_ctx.ctx_size_B  = sizeof(present_ctx_t);
	nessie_bc_ctx.cipher_enc  = (nessie_bc_enc_fpt)present_enc;
	nessie_bc_ctx.cipher_dec  = (nessie_bc_dec_fpt)present_dec;
	nessie_bc_ctx.cipher_genctx  = (nessie_bc_gen_fpt)present_genctx_dummy;
	
	nessie_bc_run();	
}

void testrun_selfenc(uint8_t* key, uint8_t* buffer){
	present_ctx_t ctx;
	cli_putstr("\r\nkey   : ");
	cli_hexdump(key, 10);
	cli_putstr("\r\nplain : ");
	cli_hexdump(buffer, 8);
	present_init(key, 80, &ctx);
	present_enc(buffer, &ctx);
	cli_putstr("\r\ncipher: ");
	cli_hexdump(buffer, 8);
	present_dec(buffer, &ctx);
	cli_putstr("\r\nplain : ");
	cli_hexdump(buffer, 8);
	cli_putstr("\r\n");
}

void testrun_self_present(void){
	uint8_t buffer[8], key[10];
	cli_putstr("\r\n\r\n=== Testvectors from the paper ===\r\n");
	
	memset(buffer, 0, 8);
	memset(key, 0, 10);
	testrun_selfenc(key, buffer);
	
	memset(buffer, 0, 8);
	memset(key, 0xFF, 10);
	testrun_selfenc(key, buffer);
	
	memset(buffer, 0xFF, 8);
	memset(key, 0, 10);
	testrun_selfenc(key, buffer);
	
	memset(buffer, 0xFF, 8);
	memset(key, 0xFF, 10);
	testrun_selfenc(key, buffer);
}

void testrun_performance_present(void){
	bcal_performance_multiple(algolist);
}

/*****************************************************************************
 *  main																	 *
 *****************************************************************************/

const char nessie_str[]      = "nessie";
const char test_str[]        = "test";
const char performance_str[] = "performance";
const char echo_str[]        = "echo";

const cmdlist_entry_t cmdlist[] = {
	{ nessie_str,      NULL, testrun_nessie_present},
	{ test_str,        NULL, testrun_self_present},
	{ performance_str, NULL, testrun_performance_present},
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
