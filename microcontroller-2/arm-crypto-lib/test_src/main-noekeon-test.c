/* main-noekeon-test.c */
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
 * noekeon test-suit
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

#include <noekeon/noekeon.h>
#include "nessie_bc_test.h"
#include "performance_test.h"
#include "bcal-performance.h"
#include "bcal_noekeon.h"

const char* algo_name = "Noekeon";

void uart0_putc(char byte){
	uart_putc(UART_0, byte);
}

char uart0_getc(void){
	return uart_getc(UART_0);
}

const bcdesc_t* algolist[] = {
	(bcdesc_t*)&noekeon_direct_desc,
	(bcdesc_t*)&noekeon_indirect_desc,
	NULL
};
/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/
void noekeon_genctx_dummy(uint8_t* key, uint16_t keysize, void* ctx){
	noekeon_init(key, ctx);
}

void testrun_nessie_noekeon_indirect(void){
	char str[strlen(algo_name)+10];
	strcpy(str, algo_name);
	strcat(str, "-indirect");
	
	nessie_bc_ctx.blocksize_B =  16;
	nessie_bc_ctx.keysize_b   = 128;
	nessie_bc_ctx.name        = str;
	nessie_bc_ctx.ctx_size_B  = sizeof(noekeon_ctx_t);
	nessie_bc_ctx.cipher_enc  = (nessie_bc_enc_fpt)noekeon_enc;
	nessie_bc_ctx.cipher_dec  = (nessie_bc_dec_fpt)noekeon_dec;
	nessie_bc_ctx.cipher_genctx  = (nessie_bc_gen_fpt)noekeon_genctx_dummy;
	
	nessie_bc_run();
}

void noekeon_genctx_dummy_direct(uint8_t* key, uint16_t keysize, void* ctx){
	memcpy(ctx, key, 16);
}

void testrun_nessie_noekeon_direct(void){
	char str[strlen(algo_name)+10];
	strcpy(str, algo_name);
	strcat(str, "-Direct");
	
	nessie_bc_ctx.blocksize_B =  16;
	nessie_bc_ctx.keysize_b   = 128;
	nessie_bc_ctx.name        = str;
	nessie_bc_ctx.ctx_size_B  = sizeof(noekeon_ctx_t);
	nessie_bc_ctx.cipher_enc  = (nessie_bc_enc_fpt)noekeon_enc;
	nessie_bc_ctx.cipher_dec  = (nessie_bc_dec_fpt)noekeon_dec;
	nessie_bc_ctx.cipher_genctx  = (nessie_bc_gen_fpt)noekeon_genctx_dummy_direct;
	
	nessie_bc_run();
}

void testrun_nessie_noekeon(void){
	testrun_nessie_noekeon_direct();
	testrun_nessie_noekeon_indirect();
}


void testrun_stdtest_rundirect(void* data, void* key){
	cli_putstr("\r\n                     ");
	cli_putstr("k = ");
	cli_hexdump(key,16);
	
	cli_putstr("\r\n                     ");
	cli_putstr("a = ");
	cli_hexdump(data,16);
	
	noekeon_enc(data, key);
	cli_putstr("\r\nafter NESSIEencrypt, b = ");
	cli_hexdump(data,16);
	
	noekeon_dec(data, key);
	cli_putstr("\r\nafter NESSIEdecrypt, a?= ");
	cli_hexdump(data,16);
	cli_putstr("\r\n");
}

void testrun_stdtest_runindirect(void* data, void* key){
	noekeon_ctx_t ctx;
	cli_putstr("\r\n                     ");
	cli_putstr("k = ");
	cli_hexdump(key,16);
	
	cli_putstr("\r\n                     ");
	cli_putstr("a = ");
	cli_hexdump(data,16);
	noekeon_init(key, &ctx);
	noekeon_enc(data, &ctx);
	cli_putstr("\r\nafter NESSIEencrypt, b = ");
	cli_hexdump(data,16);
	
	noekeon_dec(data, &ctx);
	cli_putstr("\r\nafter NESSIEdecrypt, a?= ");
	cli_hexdump(data,16);
	cli_putstr("\r\n");
}

void testrun_stdtest_noekeon(void){
	uint8_t key[16], data[16];
	uint8_t key3[16];
	noekeon_ctx_t ctx;
	
	cli_putstr("\r\nTest vectors for block cipher Noekeon in Indirect-Key Mode:\r\n");
	
	memset(key,  0, 16);
	memset(data, 0, 16);
	testrun_stdtest_runindirect(data, key);
	
	memset(key,  0xFF, 16);
	memset(data, 0xFF, 16);
	testrun_stdtest_runindirect(data, key);
	
	memset(key,  0, 16);
	memset(data, 0, 16);
	noekeon_init(key, &ctx);
	noekeon_enc(data, &ctx);
	memcpy(key3, data, 16);
	memset(key,  0xFF, 16);
	memset(data, 0xFF, 16);
	noekeon_init(key, &ctx);
	noekeon_enc(data, &ctx);
	testrun_stdtest_runindirect(data, key3);
	
	cli_putstr("\r\nTest vectors for block cipher Noekeon in Direct-Key Mode:\r\n");
	
	memset(key,  0, 16);
	memset(data, 0, 16);
	testrun_stdtest_rundirect(data, key);
	
	memset(key,  0xFF, 16);
	memset(data, 0xFF, 16);
	testrun_stdtest_rundirect(data, key);
	
	memset(key,  0, 16);
	memset(data, 0, 16);
	noekeon_enc(data, key);
	memcpy(key3, data, 16);
	memset(key,  0xFF, 16);
	memset(data, 0xFF, 16);
	noekeon_enc(data, key);
	testrun_stdtest_rundirect(data, key3);
	
}

void testrun_performance_noekeon(void){
	bcal_performance_multiple(algolist);
}
/*****************************************************************************
 *  main																	 *
 *****************************************************************************/

const char nessie_str[]      = "nessie";
const char test_str[]        = "test";
const char direct_str[]      = "direct";
const char indirect_str[]    = "indirect";
const char performance_str[] = "performance";
const char echo_str[]        = "echo";

cmdlist_entry_t cmdlist[] = {
	{ nessie_str,      NULL, testrun_nessie_noekeon},
	{ test_str,        NULL, testrun_stdtest_noekeon},
	{ direct_str,      NULL, testrun_nessie_noekeon_direct},
	{ indirect_str,    NULL, testrun_nessie_noekeon_indirect},
	{ performance_str, NULL, testrun_performance_noekeon},
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
