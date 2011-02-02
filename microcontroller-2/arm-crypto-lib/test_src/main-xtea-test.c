/* main-xtea-test.c */
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
 * XTEA test-suit
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

#include "xtea.h"
#include "nessie_bc_test.h"
#include "performance_test.h"
#include "bcal-performance.h"
#include "bcal_xtea.h"

char* algo_name = "XTEA";

void uart0_putc(char byte){
	uart_putc(UART_0, byte);
}

char uart0_getc(void){
	return uart_getc(UART_0);
}

const bcdesc_t* algolist[] = {
	(bcdesc_t*)&xtea_desc,
	NULL
};

/******************************************************************************/

void xtea_genctx_dummy(uint8_t* key, uint16_t keysize, void* ctx){
	memcpy(ctx, key, (keysize+7)/8);
}

void xtea_enc_dummy(uint8_t* buffer, void* ctx){
	xtea_enc((uint32_t*)buffer, (uint32_t*)buffer, ctx);
}

void xtea_dec_dummy(uint8_t* buffer, void* ctx){
	xtea_dec((uint32_t*)buffer, (uint32_t*)buffer, ctx);
}

void testrun_nessie_xtea(void){
	nessie_bc_ctx.blocksize_B =   8;
	nessie_bc_ctx.keysize_b   = 128;
	nessie_bc_ctx.name        = algo_name;
	nessie_bc_ctx.ctx_size_B  = 128/8;
	nessie_bc_ctx.cipher_enc  = (nessie_bc_enc_fpt)xtea_enc_dummy;
	nessie_bc_ctx.cipher_dec  = (nessie_bc_dec_fpt)xtea_dec_dummy;
	nessie_bc_ctx.cipher_genctx  = (nessie_bc_gen_fpt)xtea_genctx_dummy;
	
	nessie_bc_run();	
}

void testrun_performance_xtea(void){
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
	{ nessie_str,      NULL, testrun_nessie_xtea},
	{ test_str,        NULL, testrun_nessie_xtea},
	{ performance_str, NULL, testrun_performance_xtea},
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
