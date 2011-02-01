/* main-jh-test.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2010 Daniel Otte (daniel.otte@rub.de)

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
 * JH test-suit
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

#include "shavs.h"
#include "nessie_hash_test.h"
#include "performance_test.h"
#include "hfal-nessie.h"
#include "hfal-performance.h"
#include "hfal-test.h"

#include "jh_simple.h"
#include "hfal_jh.h"

char* algo_name = "JH";

void uart0_putc(char byte){
	uart_putc(UART_0, byte);
}

char uart0_getc(void){
	return uart_getc(UART_0);
}

const hfdesc_t* algolist[] = {
	(hfdesc_t*)&jh224_desc,
	(hfdesc_t*)&jh256_desc,
	(hfdesc_t*)&jh384_desc,
	(hfdesc_t*)&jh512_desc,
	NULL
};

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/

void performance_jh(void){
	hfal_performance_multiple(algolist);
}

void testrun_nessie_jh(void){
	hfal_nessie_multiple(algolist);
}

void test256Null(void){
	jh_ctx_t ctx;
	uint8_t hash[32];
	jh256_init(&ctx);
	jh_lastBlock(&ctx, NULL, 0);
	jh256_ctx2hash(hash, &ctx);
	cli_putstr("\r\nresult:\r\n");
	cli_hexdump_block(hash, 32, 4, 8);
}

/*****************************************************************************
 *  main																	 *
 *****************************************************************************/


const char nessie_str[]        = "nessie";
const char test_str[]          = "test";
const char test256_str[]       = "test256";
const char performance_str[]   = "performance";
const char singleround_str[]   = "singleround";
const char echo_str[]          = "echo";
const char shavs_list_str[]    = "shavs_list";
const char shavs_set_str[]     = "shavs_set";
const char shavs_test1_str[]   = "shavs_test1";

cmdlist_entry_t cmdlist[]  = {
	{ nessie_str,          NULL, testrun_nessie_jh},
	{ performance_str,     NULL, performance_jh},
	{ test256_str,         NULL, test256Null},
//	{ singleround_str,     NULL, singleround_jh},
	{ shavs_list_str,      NULL, shavs_listalgos},
	{ shavs_set_str,   (void*)1, (void_fpt)shavs_setalgo},
	{ shavs_test1_str,     NULL, shavs_test1},
	{ echo_str,        (void*)1, (void_fpt)echo_ctrl},
	{ NULL,                NULL, NULL}
};

int main (void){
	sysclk_set_freq(SYS_FREQ);
	sysclk_mosc_verify_enable();
    uart_init(UART_0, 115200, 8, UART_PARATY_NONE, UART_STOPBITS_ONE);
    gptm_set_timer_32periodic(TIMER0);

	cli_rx = uart0_getc;
    cli_tx = uart0_putc;

	shavs_algolist=(hfdesc_t**)algolist;
	shavs_algo=(hfdesc_t*)&jh256_desc;

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
