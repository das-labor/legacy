/* main-dsa-test.c */
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "cli.h"
#include "dump.h"
#include "uart_lowlevel.h"
#include "sysclock.h"
#include "hw_gptm.h"

#include "noekeon.h"
#include "noekeon_prng.h"
#include "bigint.h"
#include "bigint_io.h"
#include "dsa.h"
#include "dsa_key_blob.h"

#include "performance_test.h"
#include "hfal_sha1.h"
#include "base64_enc.h"
#include "base64_dec.h"

void uart0_putc(char byte){
	uart_putc(UART_0, byte);
}

char uart0_getc(void){
	return uart_getc(UART_0);
}

const char* algo_name = "DSA";

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/

dsa_ctx_t dsa_ctx;

void load_fix_dsa(void){
	load_dsa_key_blob(&dsa_ctx);
}

void dsa_print_item(bigint_t* a, const char* pstr){
	uint8_t *p;
	cli_putstr("\r\n");
	cli_putstr(pstr);
	cli_putstr(": ");
	uint16_t i;
	p = (uint8_t*)a->wordv + a->length_B*sizeof(bigint_word_t) -1;
	for(i=0; i<a->length_B*sizeof(bigint_word_t)-1; ++i){
		if(i%16==0){
			cli_putstr("\r\n    ");
		}
		cli_hexdump(p, 1);
		cli_putc(':');
		--p;
	}
	if(i%16==0){
		cli_putstr("\r\n    ");
	}
	cli_hexdump(p, 1);
}

void dsa_print_signature_b64(dsa_signature_t* s){
	uint16_t size_r, size_s, size_o, i,j;
	size_r = s->r.length_B*sizeof(bigint_word_t) +2;
	size_s = s->s.length_B*sizeof(bigint_word_t) +2;
	size_o = size_r + size_s +2;
	uint8_t bin_b[size_o];
	bin_b[0] = 0x30;
	bin_b[1] = size_o -2;
	bin_b[2] = 0x02;
	bin_b[3] = size_r-2;
	j=4;
	for(i=s->r.length_B*sizeof(bigint_word_t); i>0;  --i){
		bin_b[j++] = ((uint8_t*)s->r.wordv)[i-1];
	}
	bin_b[j++] = 0x02;
	bin_b[j++] = size_s -2;
	for(i=s->s.length_B*sizeof(bigint_word_t); i>0;  --i){
		bin_b[j++] = ((uint8_t*)s->s.wordv)[i-1];
	}
	char b64_b[size_o*4/3+5];
	base64enc(b64_b, bin_b, size_o);
	cli_putstr(b64_b);
}

void dsa_print_ctx(dsa_ctx_t* ctx){
	dsa_print_item(&(ctx->priv), "private");
	dsa_print_item(&(ctx->pub), "public");
	dsa_print_item(&(ctx->domain.p), "P");
	dsa_print_item(&(ctx->domain.q), "Q");
	dsa_print_item(&(ctx->domain.g), "G");
}

void dsa_print_signature(const dsa_signature_t* sig){
	cli_putstr("\r\nDSA-Signature:\r\n r:");
	bigint_print_hex(&(sig->r));
	cli_putstr("\r\n s:");
	bigint_print_hex(&(sig->s));
}

void quick_test(void){
	dsa_signature_t dsa_sig;
	uint8_t i, t=0, message[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
	load_fix_dsa();
	bigint_word_t dsa_sig_s_b[dsa_ctx.domain.q.length_B],
	        dsa_sig_r_b[dsa_ctx.domain.q.length_B];
	dsa_print_ctx(&dsa_ctx);
	dsa_sig.r.wordv = dsa_sig_r_b;
	dsa_sig.s.wordv = dsa_sig_s_b;
	cli_putstr("\r\n\r\n=== DSA QUICK TEST ===");
	for(i=0; i<9; ++i){
		cli_putstr("\r\n");
		cli_putc('1'+i);
		cli_putstr(": message: ");
		if (i){
			cli_hexdump(message, i);
		}else{
			cli_putstr("<empty>");
		}
		cli_putstr("\r\n computing signature ... ");
		dsa_sign_message(&dsa_sig, message, i*8, &sha1_desc, &dsa_ctx, random8);
		dsa_print_signature(&dsa_sig);
		cli_putstr("\r\n base64:\r\n--- SIGNATURE ---\r\n ");
		dsa_print_signature_b64(&dsa_sig);
		cli_putstr("\r\n verifying signature ... ");
		t = dsa_verify_message(&dsa_sig, message, i*8, &sha1_desc, &dsa_ctx);
		cli_putstr("\r\n verification: ");
		if(t==DSA_SIGNATURE_OK){
			cli_putstr("[PASS]");
		}else{
			cli_putstr("[FAIL]");
		}
	}
}

void reset_prng(void){
	uint8_t buf[16];
	memset(buf, 0, 16);
	random_seed(buf);
	cli_putstr("\r\nPRNG reset");
}

void testrun_performance_bigint(void){

}
/*****************************************************************************
 *  main																	 *
 *****************************************************************************/

const char echo_test_str[]     = "echo-test";
const char reset_prng_str[]    = "reset-prng";
const char quick_test_str[]    = "quick-test";
const char performance_str[]   = "performance";
const char echo_str[]          = "echo";

cmdlist_entry_t cmdlist[] = {
	{ reset_prng_str,       NULL, reset_prng                    },
	{ quick_test_str,       NULL, quick_test                    },
	{ performance_str,      NULL, testrun_performance_bigint    },
	{ echo_str,         (void*)1, (void_fpt)echo_ctrl           },
	{ NULL,                 NULL, NULL                          }
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
