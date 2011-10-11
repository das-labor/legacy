/* main-rc6-test.c */
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
 * rc6 test-suit
 * 
*/

#include "main-test-common.h"

#include "rc6.h"
#include "nessie_bc_test.h"
#include "performance_test.h"
#include "bcal-performance.h"
#include "bcal_rc6.h"

#define RC6_ROUNDS 20
char* algo_name = "RC6-32/20/16";

const bcdesc_t* algolist[] = {
	(bcdesc_t*)&rc6_desc,
	NULL
};
/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/

void testrun_nessie_rc6(void){
	nessie_bc_init();
	nessie_bc_ctx.blocksize_B =  16;
	nessie_bc_ctx.keysize_b   = 128;
	nessie_bc_ctx.name        = algo_name;
	nessie_bc_ctx.ctx_size_B  = sizeof(rc6_ctx_t);
	nessie_bc_ctx.cipher_enc  = (nessie_bc_enc_fpt)rc6_enc;
	nessie_bc_ctx.cipher_dec  = (nessie_bc_dec_fpt)rc6_dec;
	nessie_bc_ctx.cipher_free = (nessie_bc_free_fpt)rc6_free;
	nessie_bc_ctx.cipher_genctx  = (nessie_bc_gen_fpt)rc6_init;
	
	nessie_bc_run();
	
	nessie_bc_ctx.keysize_b   = 192;
	nessie_bc_run();
	
	nessie_bc_ctx.keysize_b   = 256;
	nessie_bc_run();
	
}


void testrun_performance_rc6(void){
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
	{ nessie_str,      NULL, testrun_nessie_rc6},
	{ test_str,        NULL, testrun_nessie_rc6},
	{ performance_str, NULL, testrun_performance_rc6},
	{ echo_str,    (void*)1, (void_fpt)echo_ctrl},
	{ NULL,            NULL, NULL}
};

int main (void){
	main_setup();
	
	for(;;){
		welcome_msg(algo_name);
		cmd_interface(cmdlist);
    }
}
