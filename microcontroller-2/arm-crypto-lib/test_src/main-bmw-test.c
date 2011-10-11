/* main-bmw-test.c */
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
 * BlueMidnightWish test-suit
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

#include "hfal_bmw_small.h"
#include "hfal_bmw_large.h"
#include "bmw_small.h"
#include "bmw_large.h"

void uart0_putc(char byte){
	uart_putc(UART_0, byte);
}

char uart0_getc(void){
	return uart_getc(UART_0);
}


const char* algo_name = "BlueMidnightWish";


const hfdesc_t* algolist[] = {
	(hfdesc_t*)&bmw224_desc,
	(hfdesc_t*)&bmw256_desc,
	(hfdesc_t*)&bmw384_desc,
	(hfdesc_t*)&bmw512_desc,
	NULL
};

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/

void performance_bmw(void){
	hfal_performance_multiple(algolist);
}

void testrun_nessie_bmw(void){
	hfal_nessie_multiple(algolist);
}

void bmw224_test(void* msg, uint32_t length_b){
	hfal_test(&bmw224_desc, msg, length_b);
}

void bmw256_test(void* msg, uint32_t length_b){
	hfal_test(&bmw256_desc, msg, length_b);
}
void bmw384_test(void* msg, uint32_t length_b){
	hfal_test(&bmw384_desc, msg, length_b);
}

void bmw512_test(void* msg, uint32_t length_b){
	hfal_test(&bmw512_desc, msg, length_b);
}

void test506(void){
	/* Testvector of length = 506 from short KAT */
	uint8_t v[] = {
		0xB9, 0xE3, 0xE2, 0x75, 0x5E, 0xD1, 0x21, 0x53,
		0x81, 0xB2, 0x45, 0x57, 0xEE, 0x14, 0xF8, 0xCD,
		0x26, 0x87, 0xA0, 0x71, 0xAE, 0xB3, 0xC7, 0x4F,
		0x42, 0x1C, 0xFE, 0xA3, 0xCF, 0xF8, 0xA6, 0xEA,
		0x0D, 0x4B, 0xDA, 0x2A, 0xD4, 0xBD, 0x82, 0x43,
		0xCB, 0xA7, 0x2B, 0x48, 0x1C, 0xBD, 0x52, 0x6E,
		0x1E, 0xFA, 0x1D, 0x9F, 0xD4, 0x32, 0xB9, 0x87,
		0xE8, 0x12, 0x65, 0x82, 0x44, 0x29, 0xBA, 0xC0 };
	bmw512_test(v, 506);
}

void test507(void){
	/* Testvector of length = 506 from short KAT */
	uint8_t v[] = {
		0xE8, 0x33, 0x25, 0x35, 0xE9, 0xA2, 0x05, 0x24,
		0x67, 0xA1, 0x02, 0x05, 0xC8, 0x70, 0x00, 0xBF,
		0xC6, 0xA6, 0x8D, 0x73, 0x96, 0x2E, 0x69, 0xE4,
		0xE5, 0x6F, 0x10, 0xBC, 0x79, 0xC6, 0x33, 0xC5,
		0x2F, 0x4D, 0x00, 0x74, 0xD6, 0x07, 0x75, 0x95,
		0xB6, 0x60, 0x07, 0x2E, 0x10, 0x74, 0x0D, 0xFA,
		0x66, 0xBC, 0x13, 0x20, 0x46, 0x9A, 0x31, 0x96,
		0xE0, 0x21, 0xE1, 0x32, 0x7C, 0xC0, 0x3A, 0xE0 };
	bmw512_test(v, 507);
}

void testrun_stdtest_bmw(void){
	char* msg0 = "abc";
	char* msg1 = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	bmw224_test(msg0, strlen(msg0)*8);
	bmw224_test(msg1, strlen(msg1)*8);
	bmw256_test(msg0, strlen(msg0)*8);
	bmw256_test(msg1, strlen(msg1)*8);
	bmw384_test(msg0, strlen(msg0)*8);
	bmw384_test(msg1, strlen(msg1)*8);
	bmw512_test(msg0, strlen(msg0)*8);
	bmw512_test(msg1, strlen(msg1)*8);
}

void bmw256_short_test(void* msg, uint32_t length_b){
	bmw256_test("abc", 3*8);
}


void testshort(void){
	char* msg0 = "abc";
	bmw224_test(msg0, strlen(msg0)*8);
}

void testlshort(void){
	char* msg0 = "abc";
	bmw384_test(msg0, strlen(msg0)*8);
}

/*****************************************************************************
 *  additional test-functions											     *
 *****************************************************************************/


static const
cmdlist_entry_t cmdlist[] = {
	{ "nessie",                  NULL, testrun_nessie_bmw            },
	{ "test",                    NULL, testrun_stdtest_bmw           },
	{ "short",                   NULL, testshort                     },
	{ "lshort",                  NULL, testlshort                    },
	{ "test506",                 NULL, test506                       },
	{ "test507",                 NULL, test507                       },
	{ "performance",             NULL, performance_bmw               },
	{ "shavs_list",              NULL, shavs_listalgos               },
	{ "shavs_set",           (void*)1, (void_fpt)shavs_setalgo       },
	{ "shavs_test1",             NULL, shavs_test1                   },
	{ "shavs_test3",             NULL, shavs_test3                   },
	{ "dump",                (void*)1, (void_fpt)dump                },
	{ "echo",                (void*)1, (void_fpt)echo_ctrl           },
	{ NULL,                      NULL, NULL                          }
};

int main(void) {
	sysclk_set_freq(SYS_FREQ);
	sysclk_mosc_verify_enable();
    uart_init(UART_0, 115200, 8, UART_PARATY_NONE, UART_STOPBITS_ONE);
    gptm_set_timer_32periodic(TIMER0);

	cli_rx = uart0_getc;
    cli_tx = uart0_putc;

	shavs_algolist=(hfdesc_t**)algolist;
	shavs_algo=(hfdesc_t*)&bmw256_desc;

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
