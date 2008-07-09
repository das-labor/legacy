/* main-tdes-test.c */
/*
    This file is part of the Crypto-avr-lib/microcrypt-lib.
    Copyright (C) 2008  Daniel Otte (daniel.otte@rub.de)

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
 * tdes test-suit
 * 
*/

#include "config.h"
#include "serial-tools.h"
#include "uart.h"
#include "debug.h"

#include "des.h"
#include "nessie_bc_test.h"
#include "cli.h"
#include "performance_test.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

char* cipher_name = "TDES";

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/
void tdes_init_dummy(const void* key, uint16_t keysize_b, void* ctx){
	memcpy(ctx, key, 8*3);
}

void tdes_enc_dummy(void* buffer, void* ctx){
	tdes_encrypt(buffer, buffer, ctx);
} 

void tdes_dec_dummy(void* buffer, void* ctx){
	tdes_decrypt(buffer, buffer, ctx);
} 

void testrun_nessie_tdes(void){
	nessie_bc_init();
	nessie_bc_ctx.blocksize_B =   8;
	nessie_bc_ctx.keysize_b   = 192;
	nessie_bc_ctx.name        = cipher_name;
	nessie_bc_ctx.ctx_size_B  = sizeof(8*3);
	nessie_bc_ctx.cipher_enc  = (nessie_bc_enc_fpt)tdes_enc_dummy;
	nessie_bc_ctx.cipher_dec  = (nessie_bc_dec_fpt)tdes_dec_dummy;
	nessie_bc_ctx.cipher_genctx  = (nessie_bc_gen_fpt)tdes_init_dummy;

	nessie_bc_run();
}


void testrun_performance_tdes(void){
	uint64_t t;
	char str[16];
	uint8_t key[8*3], data[8];
	
	
	calibrateTimer();
	print_overhead();
	
	memset(key,  0, 8*3);
	memset(data, 0, 8);
	
	startTimer(1);
	tdes_encrypt(data, data, key);
	t = stopTimer();
	uart_putstr_P(PSTR("\r\n\tencrypt time: "));
	ultoa((unsigned long)t, str, 10);
	uart_putstr(str);
	
	startTimer(1);
	tdes_decrypt(data, data, key);
	t = stopTimer();
	uart_putstr_P(PSTR("\r\n\tdecrypt time: "));
	ultoa((unsigned long)t, str, 10);
	uart_putstr(str);
	uart_putstr_P(PSTR("\r\n"));
}
/*****************************************************************************
 *  main																	 *
 *****************************************************************************/

int main (void){
	char  str[20];
	DEBUG_INIT();
	uart_putstr("\r\n");

	uart_putstr_P(PSTR("\r\n\r\nCrypto-VS ("));
	uart_putstr(cipher_name);
	uart_putstr_P(PSTR(")\r\nloaded and running\r\n"));

	PGM_P    u   = PSTR("nessie\0test\0performance\0");
	void_fpt v[] = {testrun_nessie_tdes, testrun_nessie_tdes, testrun_performance_tdes};

	while(1){ 
		if (!getnextwordn(str,20)){DEBUG_S("DBG: W1\r\n"); goto error;}
		if(execcommand_d0_P(str, u, v)<0){
			uart_putstr_P(PSTR("\r\nunknown command\r\n"));
		}
		continue;
	error:
		uart_putstr("ERROR\r\n");
	}
	
}

