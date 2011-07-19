/* main-trivium-test.c */
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

#include "main-test-common.h"

#include "trivium.h"
#include "scal_trivium.h"
#include "scal-basic.h"
#include "scal-nessie.h"
#include "performance_test.h"

char* algo_name = "Trivium";

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/

void testrun_nessie_trivium(void){
	scal_nessie_set_estream(1);
	scal_nessie_run(&trivium_desc);
}

void testrun_trivium(void){
	uint8_t key[10];
	uint8_t iv[4];
	uint8_t buffer[64];
	scgen_ctx_t ctx;
	memset(key, 0, 10);
	memset(iv, 0, 4);
	key[0] = 0x80;
	scal_cipher_init(&trivium_desc, key, 80, iv, 32, &ctx);
	scal_cipher_gen_fillblock(buffer, 64, &ctx);
	cli_putstr("\r\nTest:\r\n  Key     = ");
	cli_hexdump(key, 10);
	cli_putstr("\r\n  IV      = ");
	cli_hexdump(iv, 4);
	cli_putstr("\r\n  Cipher  = ");
	cli_hexdump_block(buffer, 64, 4, 16);
	scal_cipher_free(&ctx);
	key[0] = 0x40;
	scal_cipher_init(&trivium_desc, key, 80, iv, 32, &ctx);
	scal_cipher_gen_fillblock(buffer, 64, &ctx);
	cli_putstr("\r\nTest:\r\n  Key     = ");
	cli_hexdump(key, 10);
	cli_putstr("\r\n  IV      = ");
	cli_hexdump(iv, 4);
	cli_putstr("\r\n  Cipher  = ");
	cli_hexdump_block(buffer, 64, 4, 16);
	scal_cipher_free(&ctx);
	key[0] = 0x20;
	scal_cipher_init(&trivium_desc, key, 80, iv, 32, &ctx);
	scal_cipher_gen_fillblock(buffer, 64, &ctx);
	cli_putstr("\r\nTest:\r\n  Key     = ");
	cli_hexdump(key, 10);
	cli_putstr("\r\n  IV      = ");
	cli_hexdump(iv, 4);
	cli_putstr("\r\n  Cipher  = ");
	cli_hexdump_block(buffer, 64, 4, 16);
	scal_cipher_free(&ctx);
	key[0] = 0x10;
	scal_cipher_init(&trivium_desc, key, 80, iv, 32, &ctx);
	scal_cipher_gen_fillblock(buffer, 64, &ctx);
	cli_putstr("\r\nTest:\r\n  Key     = ");
	cli_hexdump(key, 10);
	cli_putstr("\r\n  IV      = ");
	cli_hexdump(iv, 4);
	cli_putstr("\r\n  Cipher  = ");
	cli_hexdump_block(buffer, 64, 4, 16);
	scal_cipher_free(&ctx);
}

void testrun_performance_trivium(void){
	uint64_t t;
	char str[16];
	uint8_t key[10], iv[10];
	trivium_ctx_t ctx;
	
	calibrateTimer();
	print_overhead();
	
	memset(key,  0, 10);
	memset(iv,  0, 10);
	
	startTimer(1);
	trivium_init(key, 80, iv, 80, &ctx);
	t = stopTimer();
	cli_putstr("\r\n\tctx-gen time: ");
	ultoa((unsigned long)t, str, 10);
	cli_putstr(str);	
	
	startTimer(1);
	trivium_enc(&ctx);
	t = stopTimer();
	cli_putstr("\r\n\tencrypt time: ");
	ultoa((unsigned long)t, str, 10);
	cli_putstr(str);	
	
	cli_putstr("\r\n");
}

/*****************************************************************************
 *  main																	 *
 *****************************************************************************/

const char nessie_str[] = "nessie";
const char test_str[] = "test";
const char performance_str[] = "performance";
const char echo_str[] = "echo";

cmdlist_entry_t cmdlist[] = {
	{ nessie_str,      NULL, testrun_nessie_trivium},
	{ test_str,        NULL, testrun_trivium},
	{ performance_str, NULL, testrun_performance_trivium},
	{ echo_str,    (void*)1, (void_fpt)echo_ctrl},
	{ NULL,            NULL, NULL}
};

int main (void){
	main_setup();
	
	for(;;){
		welcome_msg(algo_name);
		cmd_interface(cmdlist);
	}
}
