/* main-shabal-test.c */
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
 * shabal test-suit
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

#include "shabal.h"
#include "hfal_shabal.h"

void uart0_putc(char byte){
	uart_putc(UART_0, byte);
}

char uart0_getc(void){
	return uart_getc(UART_0);
}

const char* algo_name = "Shabal";

const hfdesc_t* algolist[] = {
	(hfdesc_t*)&shabal192_desc,
	(hfdesc_t*)&shabal224_desc,
	(hfdesc_t*)&shabal256_desc,
	(hfdesc_t*)&shabal384_desc,
	(hfdesc_t*)&shabal512_desc,
	NULL
};

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/
void testrun_stdtest_shabal192(const void* msg, uint16_t size_b){
	hfal_test(&shabal192_desc, msg, size_b);
}

void testrun_stdtest_shabal224(const void* msg, uint16_t size_b){
	hfal_test(&shabal224_desc, msg, size_b);
}

void testrun_stdtest_shabal256(const void* msg, uint16_t size_b){
	hfal_test(&shabal256_desc, msg, size_b);
}

void testrun_stdtest_shabal384(const void* msg, uint16_t size_b){
	hfal_test(&shabal384_desc, msg, size_b);
}

void testrun_stdtest_shabal512(const void* msg, uint16_t size_b){
	hfal_test(&shabal512_desc, msg, size_b);
}

void testrun_stdtest_shabal(void){
	uint8_t ma[64];
	const char*   mb= "abcdefghijklmnopqrstuvwxyz-"
	                  "0123456789-"
			          "ABCDEFGHIJKLMNOPQRSTUVWXYZ-"
                      "0123456789-"
				      "abcdefghijklmnopqrstuvwxyz";

	memset(ma, 0, 64);
	testrun_stdtest_shabal192(ma, 64*8);
	testrun_stdtest_shabal192(mb, strlen(mb)*8);
	testrun_stdtest_shabal224(ma, 64*8);
	testrun_stdtest_shabal224(mb, strlen(mb)*8);
	testrun_stdtest_shabal256(ma, 64*8);
	testrun_stdtest_shabal256(mb, strlen(mb)*8);
	testrun_stdtest_shabal384(ma, 64*8);
	testrun_stdtest_shabal384(mb, strlen(mb)*8);
	testrun_stdtest_shabal512(ma, 64*8);
	testrun_stdtest_shabal512(mb, strlen(mb)*8);
}

void testshort(void){
	uint8_t ma[64];
	memset(ma, 0, 64);
	testrun_stdtest_shabal192(ma, 64*8);
}

void shabal_ctx_dump(shabal_ctx_t* ctx){
	uint8_t i;
	void* p;
	cli_putstr("\r\n=== shabal ctx dump ===\r\n  size = ");
	i=sizeof(shabal_ctx_t);
	if(i>=100)
		cli_putc('0'+i/100);
	if(i>=10)
		cli_putc('0'+(i/10)%10);
	cli_putc('0'+i%10);
	cli_putstr("\r\n  a = ");
	cli_hexdump_block(ctx->a, 12*4, 5, 4*8);
	cli_putstr("\r\n  b_buffer = ");
	cli_hexdump_block(ctx->b_buffer, 12*4, 5, 4*8);
	cli_putstr("\r\n  c_buffer = ");
	cli_hexdump_block(ctx->c_buffer, 12*4, 5, 4*8);
	if(ctx->b == &(ctx->b_buffer[0]))
		cli_putstr("\r\nb --> b_buffer");
	if(ctx->b == &(ctx->c_buffer[0]))
		cli_putstr("\r\nb --> c_buffer");
	if(ctx->c == &(ctx->b_buffer[0]))
		cli_putstr("\r\nc --> b_buffer");
	if(ctx->c == &(ctx->c_buffer[0]))
		cli_putstr("\r\nc --> c_buffer");
	cli_putstr("\r\n b = ");
	cli_hexdump(&(ctx->b), 2);
	p = ctx->b_buffer;
	cli_putstr("\r\n b (should) = ");
	cli_hexdump(&p, 2);
	cli_putstr("\r\n c = ");
	cli_hexdump(&(ctx->c), 2);
	p = ctx->c_buffer;
	cli_putstr("\r\n c (should) = ");
	cli_hexdump(&p, 2);
}


void testinit_192(void){
	shabal_ctx_t ctx;
	shabal192_init(&ctx);
	shabal_ctx_dump(&ctx);
}

void testinit_224(void){
	shabal_ctx_t ctx;
	shabal224_init(&ctx);
	shabal_ctx_dump(&ctx);
}

void testinit_256(void){
	shabal_ctx_t ctx;
	shabal256_init(&ctx);
	shabal_ctx_dump(&ctx);
}

void testinit_384(void){
	shabal_ctx_t ctx;
	shabal384_init(&ctx);
	shabal_ctx_dump(&ctx);
}

void testinit_512(void){
	shabal_ctx_t ctx;
	shabal512_init(&ctx);
	shabal_ctx_dump(&ctx);
}
void testinit(void){
	testinit_192();
	testinit_224();
	testinit_256();
	testinit_384();
	testinit_512();
}

void performance_shabal(void){
	hfal_performance_multiple(algolist);
}

void testrun_nessie_shabal(void){
	hfal_nessie_multiple(algolist);
}

/*****************************************************************************
 *  main																	 *
 *****************************************************************************/


const char nessie_str[]       = "nessie";
const char test_str[]         = "test";
const char testinit192_str[]  = "testinit192";
const char testinit_str[]     = "testinit";
const char testshort_str[]    = "short";
const char ztest_str[]        = "zerotest";
const char performance_str[]  = "performance";
const char echo_str[]         = "echo";
const char shavs_list_str[]   = "shavs_list";
const char shavs_set_str[]    = "shavs_set";
const char shavs_test1_str[]  = "shavs_test1";
const char shavs_test2_str[]  = "shavs_test2";
const char shavs_test3_str[]  = "shavs_test3";

cmdlist_entry_t cmdlist[]  = {
	{ nessie_str,          NULL, testrun_nessie_shabal          },
	{ test_str,            NULL, testrun_stdtest_shabal         },
	{ testinit192_str,     NULL, testinit_192                   },
	{ testinit_str,        NULL, testinit                       },
	{ testshort_str,       NULL, testshort                      },
	{ performance_str,     NULL, performance_shabal             },
	{ shavs_list_str,      NULL, shavs_listalgos                },
	{ shavs_set_str,   (void*)1, (void_fpt)shavs_setalgo        },
	{ shavs_test1_str,     NULL, shavs_test1                    },
	{ shavs_test2_str,     NULL, shavs_test2                    },
	{ shavs_test3_str,     NULL, shavs_test3                    },
	{ echo_str,        (void*)1, (void_fpt)echo_ctrl            },
	{ NULL,                NULL, NULL                           }
};

int main(void) {
	sysclk_set_freq(SYS_FREQ);
	sysclk_mosc_verify_enable();
    uart_init(UART_0, 115200, 8, UART_PARATY_NONE, UART_STOPBITS_ONE);
    gptm_set_timer_32periodic(TIMER0);

	cli_rx = uart0_getc;
    cli_tx = uart0_putc;

	shavs_algolist=(hfdesc_t**)algolist;
	shavs_algo=(hfdesc_t*)&shabal256_desc;

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
