/* main-serpent-test.c */
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
 * serpent test-suit
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


#include "serpent.h"
#include "nessie_bc_test.h"
#include "performance_test.h"
#include "bcal-performance.h"
#include "bcal_serpent.h"

char* algo_name = "Serpent";

void uart0_putc(char byte){
	uart_putc(UART_0, byte);
}

char uart0_getc(void){
	return uart_getc(UART_0);
}

const bcdesc_t* algolist[] = {
	(bcdesc_t*)&serpent_desc,
	NULL
};

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/

void testrun_nessie_serpent(void){
	nessie_bc_ctx.blocksize_B =  16;
	nessie_bc_ctx.keysize_b   = 128;
	nessie_bc_ctx.name        = algo_name;
	nessie_bc_ctx.ctx_size_B  = sizeof(serpent_ctx_t);
	nessie_bc_ctx.cipher_enc  = (nessie_bc_enc_fpt)serpent_enc;
	nessie_bc_ctx.cipher_dec  = (nessie_bc_dec_fpt)serpent_dec;
	nessie_bc_ctx.cipher_genctx  = (nessie_bc_gen_fpt)serpent_init;
	
	nessie_bc_run();
	
	nessie_bc_ctx.keysize_b   = 192;
	nessie_bc_run();
	
	nessie_bc_ctx.keysize_b   = 256;
	nessie_bc_run();
}

void testrun_test_serpent(void){
	uint8_t key[32];
	serpent_ctx_t ctx;
	uint8_t i;
	memset(key, 0, 16);
	serpent_init(key, 128, &ctx);
	for(i=0; i<33; ++i){
		cli_putstr("\r\n subkekey ");
		cli_hexdump(&i, 1);
		cli_putstr(" : ");
		cli_hexdump(ctx.k[i], 16);
	}
}

void testrun_serpent256(void){
	uint8_t key[32];
	uint8_t data[16];
	serpent_ctx_t ctx;
	memset(key, 0, 32);
	memset(data, 0, 16);
	key[0] = 0x80;
	cli_putstr("\r\n== small test Serpent-256 ==");
	cli_putstr("\r\n  key    = ");
	cli_hexdump(key, 32);
	cli_putstr("\r\n  plain  = ");
	cli_hexdump(data, 16);
	serpent_init(key, 256, &ctx);
	serpent_enc(data, &ctx);
	cli_putstr("\r\n  cipher = ");
	cli_hexdump(data, 16);
}

void testrun_performance_serpent(void){
	bcal_performance_multiple(algolist);
}
/*****************************************************************************
 *  main																	 *
 *****************************************************************************/

const char nessie_str[]      = "nessie";
const char test_str[]        = "test";
const char test256_str[]     = "test256";
const char performance_str[] = "performance";
const char echo_str[]        = "echo";

const cmdlist_entry_t cmdlist[] = {
	{ nessie_str,      NULL, testrun_nessie_serpent},
	{ test_str,        NULL, testrun_test_serpent},
	{ test256_str,     NULL, testrun_serpent256},
	{ performance_str, NULL, testrun_performance_serpent},
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
