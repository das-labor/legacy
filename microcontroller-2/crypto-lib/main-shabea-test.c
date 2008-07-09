/* main-shabea-test.c */
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
/**
 * \file	main-shabea-test.c
 * \author	Daniel Otte 
 * \date	2007-06-07
 * \brief	test suit for SHABEA
 * \par License	
 * GPLv3 or later
 * 
 */
#include "config.h"
#include "serial-tools.h"
#include "uart.h"
#include "debug.h"

#include "shabea.h"
#include "nessie_bc_test.h"
#include "cli.h"
#include "performance_test.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

char* cipher_name = "Shabea";

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/
void shabea_genctx_dummy(uint8_t* key, uint16_t keysize, void* ctx){
	memcpy(ctx, key, keysize);
}

void shabea_enc_dummy(void* buffer, void* ctx){
	shabea256(buffer, ctx, 256, 1, 16);
}

void shabea_dec_dummy(void* buffer, void* ctx){
	shabea256(buffer, ctx, 256, 0, 16);
}


void testrun_nessie_shabea(void){
	nessie_bc_ctx.blocksize_B =  32;
	nessie_bc_ctx.keysize_b   = 256;
	nessie_bc_ctx.name        = cipher_name;
	nessie_bc_ctx.ctx_size_B  = 32;
	nessie_bc_ctx.cipher_enc  = (nessie_bc_enc_fpt)shabea_enc_dummy;
	nessie_bc_ctx.cipher_dec  = (nessie_bc_dec_fpt)shabea_dec_dummy;
	nessie_bc_ctx.cipher_genctx  = (nessie_bc_gen_fpt)shabea_genctx_dummy;
	
	nessie_bc_run();
}


void testrun_performance_shabea(void){
	uint64_t t;
	char str[16];
	uint8_t key[32], data[32];
	
	calibrateTimer();
	print_overhead();
	
	memset(key,  0, 32);
	memset(data, 0, 32);
	
	startTimer(1);
	shabea256(data, key, 256, 1, 16);
	t = stopTimer();
	uart_putstr_P(PSTR("\r\n\tencrypt time: "));
	ultoa((unsigned long)t, str, 10);
	uart_putstr(str);
	
	
	startTimer(1);
	shabea256(data, key, 256, 0, 16);
	t = stopTimer();
	uart_putstr_P(PSTR("\r\n\tdecrypt time: "));
	ultoa((unsigned long)t, str, 10);
	uart_putstr(str);
	
	uart_putstr_P(PSTR("\r\n"));
}

/*****************************************************************************
 *  self tests																 *
 *****************************************************************************/

void testencrypt(uint8_t* block, uint8_t* key){
	uart_putstr("\r\n==testy-encrypt==\r\n key: ");
	uart_hexdump(key,16);
	uart_putstr("\r\n plain: ");
	uart_hexdump(block,32);
	shabea256(block,key,128,1,16);
	uart_putstr("\r\n crypt: ");
	uart_hexdump(block,32);
}

void testdecrypt(uint8_t* block, uint8_t* key){

	uart_putstr("\r\n==testy-decrypt==\r\n key: ");
	uart_hexdump(key,16);
	uart_putstr("\r\n crypt: ");
	uart_hexdump(block,32);
	shabea256(block,key,128,0,16);
	uart_putstr("\r\n plain: ");
	uart_hexdump(block,32);
}

void testrun_shabea(void){
	uint8_t keys[4][16]=
		{ {	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		  {	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
			0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f },
		  { 0x47, 0x06, 0x48, 0x08, 0x51, 0xE6, 0x1B, 0xE8,
		  	0x5D, 0x74, 0xBF, 0xB3, 0xFD, 0x95, 0x61, 0x85 },
		  { 0x28, 0xDB, 0xC3, 0xBC, 0x49, 0xFF, 0xD8, 0x7D,
		  	0xCF, 0xA5, 0x09, 0xB1, 0x1D, 0x42, 0x2B, 0xE7,}
		};
	uint8_t datas[4][32]=
		{ {	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
			0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
			0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
			0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f },
		  {	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		  { 0x83, 0xA2, 0xF8, 0xA2, 0x88, 0x64, 0x1F, 0xB9, 
		  	0xA4, 0xE9, 0xA5, 0xCC, 0x2F, 0x13, 0x1C, 0x7D,
		  	0x83, 0xA2, 0xF8, 0xA2, 0x88, 0x64, 0x1F, 0xB9, 
		  	0xA4, 0xE9, 0xA5, 0xCC, 0x2F, 0x13, 0x1C, 0x7D },
		  { 0xB4, 0x1E, 0x6B, 0xE2, 0xEB, 0xA8, 0x4A, 0x14, 
		  	0x8E, 0x2E, 0xED, 0x84, 0x59, 0x3C, 0x5E, 0xC7,
		  	0xB4, 0x1E, 0x6B, 0xE2, 0xEB, 0xA8, 0x4A, 0x14, 
		  	0x8E, 0x2E, 0xED, 0x84, 0x59, 0x3C, 0x5E, 0xC7 }
		};
	uint8_t i=0;
	for(i=0; i<4; ++i){
		testencrypt(datas[i],keys[i]);
		testdecrypt(datas[i],keys[i]);	
	}
}



/*****************************************************************************
 *  main																	 *
 *****************************************************************************/

int main (void){
	char str[20];

	DEBUG_INIT();
	uart_putstr("\r\n");

	uart_putstr_P(PSTR("\r\n\r\nCrypto-VS ("));
	uart_putstr(cipher_name);
	uart_putstr_P(PSTR(")\r\nloaded and running\r\n"));

	PGM_P    u   = PSTR("nessie\0test\0performance\0");
	void_fpt v[] = {testrun_nessie_shabea, testrun_shabea, testrun_performance_shabea};

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

