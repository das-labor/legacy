/* main-prf_tls12-test.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2010 Daniel Otte (daniel.otte@rub.de)

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
 * DSA test-suit
 *
*/

#include "main-test-common.h"
#include "prf_tls12.h"
#include <crypto/hashfunction_descriptor.h>
#include <crypto/hfal_sha1.h>
#include <crypto/hfal_sha256.h>
const char* algo_name = "PRF-TLS1.2";

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/

/*
# Generating 100 bytes of pseudo-randomness using TLS1.2PRF-SHA256
Secret (16 bytes):
0000    9b be 43 6b a9 40 f0 17    ..Ck....
0008    b1 76 52 84 9a 71 db 35    .vR..q.5

Seed (16 bytes):
0000    a0 ba 9f 93 6c da 31 18    ....l.1.
0008    27 a6 f7 96 ff d5 19 8c    ........

Label (10 bytes):
0000    74 65 73 74 20 6c 61 62    test lab
0008    65 6c                      el

Output (100 bytes):
0000    e3 f2 29 ba 72 7b e1 7b    ....r...
0008    8d 12 26 20 55 7c d4 53    ... U..S
0010    c2 aa b2 1d 07 c3 d4 95    ........
0018    32 9b 52 d4 e6 1e db 5a    2.R....Z
0020    6b 30 17 91 e9 0d 35 c9    k0....5.
0028    c9 a4 6b 4e 14 ba f9 af    ..kN....
0030    0f a0 22 f7 07 7d ef 17    ........
0038    ab fd 37 97 c0 56 4b ab    ..7..VK.
0040    4f bc 91 66 6e 9d ef 9b    O..fn...
0048    97 fc e3 4f 79 67 89 ba    ...Oyg..
0050    a4 80 82 d1 22 ee 42 c5    ......B.
0058    a7 2e 5a 51 10 ff f7 01    ..ZQ....
0060    87 34 7b 66                .4.f
 */

const uint8_t test_secret[] = {
	0x9b, 0xbe, 0x43, 0x6b, 0xa9, 0x40, 0xf0, 0x17,
	0xb1, 0x76, 0x52, 0x84, 0x9a, 0x71, 0xdb, 0x35
};

const uint8_t test_seed[] = {
	0xa0, 0xba, 0x9f, 0x93, 0x6c, 0xda, 0x31, 0x18,
	0x27, 0xa6, 0xf7, 0x96, 0xff, 0xd5, 0x19, 0x8c
};

const char test_label[] = "test label";

void test_prf(const hfdesc_t* hash){
	prf_tls12_ctx_t ctx;
	uint8_t buffer[100];
	prf_tls12_init_w_label(&ctx, hash, test_secret, 16*8, test_label, strlen(test_label), test_seed, 16*8);
	cli_putstr("\r\n== Testing PRF-TLSv1.2 with ");
	cli_putstr(hash->name);
	cli_putstr(" ==\r\n");
	prf_tls12_fill(buffer, 100, &ctx);
	cli_hexdump_block(buffer, 100, 4, 8);
	prf_tls12_free(&ctx);
}

void test_sha256(void){
	test_prf(&sha256_desc);
}

void test_sha1(void){
	test_prf(&sha1_desc);
}
/*****************************************************************************
 *  main																	 *
 *****************************************************************************/

const char echo_test_str[]     = "echo-test";
const char test_sha256_str[]   = "test-sha256";
const char test_sha1_str[]     = "test-sha1";
//const char performance_str[]   = "performance";
const char echo_str[]          = "echo";

cmdlist_entry_t cmdlist[] = {
	{ test_sha256_str,      NULL, test_sha256                   },
	{ test_sha1_str,        NULL, test_sha1                     },
//	{ performance_str,      NULL, testrun_performance_bigint    },
	{ echo_str,         (void*)1, (void_fpt)echo_ctrl           },
	{ NULL,                 NULL, NULL                          }
};

int main (void){
	main_setup();

	for(;;){
		welcome_msg(algo_name);
		cmd_interface(cmdlist);
	}
}
