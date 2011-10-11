/* main-echo-test.c */
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
 * Echo test-suit
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

#include "echo.h"
#include "hfal_echo.h"

char* algo_name = "Echo";


const hfdesc_t* algolist[] = {
	(hfdesc_t*)&echo224_desc,
	(hfdesc_t*)&echo256_desc,
	(hfdesc_t*)&echo384_desc,
	(hfdesc_t*)&echo512_desc,
	NULL
};

void uart0_putc(char byte){
	uart_putc(UART_0, byte);
}

char uart0_getc(void){
	return uart_getc(UART_0);
}

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/
/* IntermediateKAT1_256.txt */
static const
uint8_t intermediate_data[] = {
	0xDB, 0x11, 0xF6, 0x09, 0xBA, 0xBA, 0x7B, 0x0C,
	0xA6, 0x34, 0x92, 0x6B, 0x1D, 0xD5, 0x39, 0xC8,
	0xCB, 0xAD, 0xA2, 0x49, 0x67, 0xD7, 0xAD, 0xD4,
	0xD9, 0x87, 0x6F, 0x77, 0xC2, 0xD8, 0x0C, 0x0F,
	0x4D, 0xCE, 0xFB, 0xD7, 0x12, 0x15, 0x48, 0x37,
	0x35, 0x82, 0x70, 0x5C, 0xCA, 0x24, 0x95, 0xBD,
	0x2A, 0x43, 0x71, 0x6F, 0xE6, 0x4E, 0xD2, 0x6D,
	0x05, 0x9C, 0xFB, 0x56, 0x6B, 0x33, 0x64, 0xBD,
	0x49, 0xEE, 0x07, 0x17, 0xBD, 0xD9, 0x81, 0x0D,
	0xD1, 0x4D, 0x8F, 0xAD, 0x80, 0xDB, 0xBD, 0xC4,
	0xCA, 0xFB, 0x37, 0xCC, 0x60, 0xFB, 0x0F, 0xE2,
	0xA8, 0x0F, 0xB4, 0x54, 0x1B, 0x8C, 0xA9, 0xD5,
	0x9D, 0xCE, 0x45, 0x77, 0x38, 0xA9, 0xD3, 0xD8,
	0xF6, 0x41, 0xAF, 0x8C, 0x3F, 0xD6, 0xDA, 0x16,
	0x2D, 0xC1, 0x6F, 0xC0, 0x1A, 0xAC, 0x52, 0x7A,
	0x4A, 0x02, 0x55, 0xB4, 0xD2, 0x31, 0xC0, 0xBE,
	0x50, 0xF4, 0x4F, 0x0D, 0xB0, 0xB7, 0x13, 0xAF,
	0x03, 0xD9, 0x68, 0xFE, 0x7F, 0x0F, 0x61, 0xED,
	0x08, 0x24, 0xC5, 0x5C, 0x4B, 0x52, 0x65, 0x54,
	0x8F, 0xEB, 0xD6, 0xAA, 0xD5, 0xC5, 0xEE, 0xDF,
	0x63, 0xEF, 0xE7, 0x93, 0x48, 0x9C, 0x39, 0xB8,
	0xFD, 0x29, 0xD1, 0x04, 0xCE
	};

static const
uint8_t intermediate_data2[] = {
	0x75, 0x8E, 0xA3, 0xFE, 0xA7, 0x38, 0x97, 0x3D,
	0xB0, 0xB8, 0xBE, 0x7E, 0x59, 0x9B, 0xBE, 0xF4,
	0x51, 0x93, 0x73, 0xD6, 0xE6, 0xDC, 0xD7, 0x19,
	0x5E, 0xA8, 0x85, 0xFC, 0x99, 0x1D, 0x89, 0x67,
	0x62, 0x99, 0x27, 0x59, 0xC2, 0xA0, 0x90, 0x02,
	0x91, 0x2F, 0xB0, 0x8E, 0x0C, 0xB5, 0xB7, 0x6F,
	0x49, 0x16, 0x2A, 0xEB, 0x8C, 0xF8, 0x7B, 0x17,
	0x2C, 0xF3, 0xAD, 0x19, 0x02, 0x53, 0xDF, 0x61,
	0x2F, 0x77, 0xB1, 0xF0, 0xC5, 0x32, 0xE3, 0xB5,
	0xFC, 0x99, 0xC2, 0xD3, 0x1F, 0x8F, 0x65, 0x01,
	0x16, 0x95, 0xA0, 0x87, 0xA3, 0x5E, 0xE4, 0xEE,
	0xE5, 0xE3, 0x34, 0xC3, 0x69, 0xD8, 0xEE, 0x5D,
	0x29, 0xF6, 0x95, 0x81, 0x5D, 0x86, 0x6D, 0xA9,
	0x9D, 0xF3, 0xF7, 0x94, 0x03
};

void echo256_interm(void){
	echo_small_ctx_t ctx;
	uint8_t data[1384/8];
	uint8_t hash[32];
	echo256_init(&ctx);
	memcpy(data, intermediate_data, 173);
	cli_putstr("\r\ninit done ");
	echo_small_lastBlock(&ctx, data, 1384);
	cli_putstr("\r\nlastblock done ");
	echo256_ctx2hash(hash, &ctx);
	cli_putstr("\r\nhash = ");
	cli_hexdump(hash, 32);
}

void echo512_interm(void){
	echo_large_ctx_t ctx;
	uint8_t data[872/8];
	uint8_t hash[64];
	echo512_init(&ctx);
	memcpy(data, intermediate_data2, 872/8);
	cli_putstr("\r\ninit done ");
	echo_large_lastBlock(&ctx, data, 872);
	cli_putstr("\r\nlastblock done ");
	echo512_ctx2hash(hash, &ctx);
	cli_putstr("\r\nhash = ");
	cli_hexdump(hash, 64);
}

void echo256_test0(void){
	echo_small_ctx_t ctx;
	uint8_t hash[32];
	echo256_init(&ctx);
	cli_putstr("\r\ninit done ");
	echo_small_lastBlock(&ctx, NULL, 0);
	cli_putstr("\r\nlastblock done ");
	echo256_ctx2hash(hash, &ctx);
	cli_putstr("\r\nhash = ");
	cli_hexdump(hash, 32);
}

void performance_echo(void){
	hfal_performance_multiple(algolist);
}

void testrun_nessie_echo(void){
	hfal_nessie_multiple(algolist);
}
/*****************************************************************************
 *  main																	 *
 *****************************************************************************/

const char nessie_str[]       = "nessie";
const char test256_str[]      = "test256";
const char interm_str[]       = "interm";
const char interm2_str[]      = "interm2";
const char performance_str[]  = "performance";
const char echo_str[]         = "echo";
const char shavs_list_str[]   = "shavs_list";
const char shavs_set_str[]    = "shavs_set";
const char shavs_test1_str[]  = "shavs_test1";
const char shavs_test3_str[]  = "shavs_test3";

cmdlist_entry_t cmdlist[]  = {
	{ nessie_str,                NULL, testrun_nessie_echo         },
	{ interm_str,                NULL, echo256_interm              },
	{ interm2_str,               NULL, echo512_interm              },
	{ test256_str,               NULL, echo256_test0               },
	{ performance_str,           NULL, performance_echo            },
	{ shavs_list_str,            NULL, shavs_listalgos             },
	{ shavs_set_str,         (void*)1, (void_fpt)shavs_setalgo     },
	{ shavs_test1_str,           NULL, shavs_test1                 },
	{ shavs_test3_str,           NULL, shavs_test3                 },
	{ echo_str,              (void*)1, (void_fpt)echo_ctrl         },
	{ NULL,                      NULL, NULL                        }
};

int main(void) {
	sysclk_set_freq(SYS_FREQ);
	sysclk_mosc_verify_enable();
    uart_init(UART_0, 115200, 8, UART_PARATY_NONE, UART_STOPBITS_ONE);
    gptm_set_timer_32periodic(TIMER0);

	cli_rx = uart0_getc;
    cli_tx = uart0_putc;

	shavs_algolist=(hfdesc_t**)algolist;
	shavs_algo=(hfdesc_t*)&echo256_desc;

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
