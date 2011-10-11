/* main-keccak-test.c */
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
 * Keccak test-suit
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

#include "keccak.h"
#include "hfal_keccak.h"

void uart0_putc(char byte){
	uart_putc(UART_0, byte);
}

char uart0_getc(void){
	return uart_getc(UART_0);
}

const char* algo_name = "Keccak";


const hfdesc_t* algolist[] = {
	(hfdesc_t*)&keccak224_desc,
	(hfdesc_t*)&keccak256_desc,
	(hfdesc_t*)&keccak384_desc,
	(hfdesc_t*)&keccak512_desc,
	NULL
};

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/
void test_256(void){
	uint8_t data[] = {0x53, 0x58, 0x7B,  0xC8 };
	uint8_t hash[32];
	uint8_t null[KECCAK256_BLOCKSIZE_B];
	memset(null, 0, KECCAK256_BLOCKSIZE_B);
	keccak_ctx_t ctx;
	keccak256_init(&ctx);
	keccak_lastBlock(&ctx, data, 29);
	keccak256_ctx2hash(hash, &ctx);
	cli_putstr("\r\n testhash: ");
	cli_hexdump(hash, 32);
}


void performance_keccak(void){
	hfal_performance_multiple(algolist);
}

void testrun_nessie_keccak(void){
	hfal_nessie_multiple(algolist);
}
/*****************************************************************************
 *  main																	 *
 *****************************************************************************/

const char nessie_str[]       = "nessie";
const char test256_str[]      = "test256";
const char performance_str[]  = "performance";
const char echo_str[]         = "echo";
const char shavs_list_str[]   = "shavs_list";
const char shavs_set_str[]    = "shavs_set";
const char shavs_test1_str[]  = "shavs_test1";
const char shavs_test3_str[]  = "shavs_test3";

cmdlist_entry_t cmdlist[]  = {
	{ nessie_str,                NULL, testrun_nessie_keccak     },
	{ performance_str,           NULL, performance_keccak        },
	{ test256_str,               NULL, test_256                  },
	{ shavs_list_str,            NULL, shavs_listalgos           },
	{ shavs_set_str,         (void*)1, (void_fpt)shavs_setalgo   },
	{ shavs_test1_str,           NULL, shavs_test1               },
	{ shavs_test3_str,           NULL, shavs_test3               },
	{ echo_str,              (void*)1, (void_fpt)echo_ctrl       },
	{ NULL,                      NULL, NULL                      }
};

int main(void) {
	sysclk_set_freq(SYS_FREQ);
	sysclk_mosc_verify_enable();
    uart_init(UART_0, 115200, 8, UART_PARATY_NONE, UART_STOPBITS_ONE);
    gptm_set_timer_32periodic(TIMER0);

	cli_rx = uart0_getc;
    cli_tx = uart0_putc;

	shavs_algolist=(hfdesc_t**)algolist;
	shavs_algo=(hfdesc_t*)&keccak256_desc;

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
