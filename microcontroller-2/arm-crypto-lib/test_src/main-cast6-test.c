/*
 * rc6 test-suit
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

#include "cast6.h"
#include "nessie_bc_test.h"
#include "cli.h"
#include "performance_test.h"
#include "bcal-performance.h"
#include "bcal_cast6.h"

char* algo_name = "CAST-256";

void uart0_putc(char byte){
	uart_putc(UART_0, byte);
}

char uart0_getc(void){
	return uart_getc(UART_0);
}

const bcdesc_t* algolist[] = {
	(bcdesc_t*)&cast6_desc,
	NULL
};

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/


void testrun_nessie_cast6(void){
	nessie_bc_init();
	nessie_bc_ctx.blocksize_B =  16;
	nessie_bc_ctx.keysize_b   = 128;
	nessie_bc_ctx.name        = algo_name;
	nessie_bc_ctx.ctx_size_B  = sizeof(cast6_ctx_t);
	nessie_bc_ctx.cipher_enc  = (nessie_bc_enc_fpt)cast6_enc;
	nessie_bc_ctx.cipher_dec  = (nessie_bc_dec_fpt)cast6_dec;
	nessie_bc_ctx.cipher_genctx  = (nessie_bc_gen_fpt)cast6_init;
	
	nessie_bc_run();
	
	nessie_bc_ctx.keysize_b   = 192;
	nessie_bc_run();
	
	nessie_bc_ctx.keysize_b   = 256;
	nessie_bc_run();
}

void testrun_rfc_cast6(void){
	cli_putstr("\r\n testvalues from rfc-2612\r\n");
	uint8_t key[32], data[16];
	cast6_ctx_t ctx;
	memcpy(key, "\x23\x42\xbb\x9e\xfa\x38\x54\x2c"
	            "\x0a\xf7\x56\x47\xf2\x9f\x61\x5d", 16);
	memset(data, 0, 16);
	
	cli_putstr("\r\n key: ");
	cli_hexdump(key, 16);
	cli_putstr("\r\n PT:  ");
	cli_hexdump(data, 16);
	cast6_init(key, 128, &ctx);
	cast6_enc(data, &ctx);
	cli_putstr("\r\n CT:  ");
	cli_hexdump(data, 16);
	cast6_dec(data, &ctx);
	cli_putstr("\r\n PT:  ");
	cli_hexdump(data, 16);
	
	cli_putstr("\r\n\r\n");

	memcpy(key, "\x23\x42\xbb\x9e\xfa\x38\x54\x2c"
	            "\xbe\xd0\xac\x83\x94\x0a\xc2\x98"
				"\xba\xc7\x7a\x77\x17\x94\x28\x63", 24);
	
	cli_putstr("\r\n key: ");
	cli_hexdump(key, 24);
	cli_putstr("\r\n PT:  ");
	cli_hexdump(data, 16);
	cast6_init(key, 192, &ctx);
	cast6_enc(data, &ctx);
	cli_putstr("\r\n CT:  ");
	cli_hexdump(data, 16);
	cast6_dec(data, &ctx);
	cli_putstr("\r\n PT:  ");
	cli_hexdump(data, 16);
	
	cli_putstr("\r\n\r\n");
	
	memcpy(key, "\x23\x42\xbb\x9e\xfa\x38\x54\x2c"
	            "\xbe\xd0\xac\x83\x94\x0a\xc2\x98"
				"\x8d\x7c\x47\xce\x26\x49\x08\x46"
				"\x1c\xc1\xb5\x13\x7a\xe6\xb6\x04", 32);
	cli_putstr("\r\n key: ");
	cli_hexdump(key, 32);
	cli_putstr("\r\n PT:  ");
	cli_hexdump(data, 16);
	cast6_init(key, 256, &ctx);
	cast6_enc(data, &ctx);
	cli_putstr("\r\n CT:  ");
	cli_hexdump(data, 16);
	cast6_dec(data, &ctx);
	cli_putstr("\r\n PT:  ");
	cli_hexdump(data, 16);
	
	cli_putstr("\r\n\r\n");
}

void testrun_performance_cast6(void){
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
	{ nessie_str,      NULL, testrun_nessie_cast6 },
	{ test_str,        NULL, testrun_rfc_cast6},
	{ performance_str, NULL, testrun_performance_cast6},
	{ echo_str,    (void*)1, (void_fpt)echo_ctrl},
	{ NULL,            NULL, NULL}
};

int main (void){
	sysclk_set_freq(SYS_FREQ);
	sysclk_mosc_verify_enable();
	uart_init(UART_0, 115200, 8, UART_PARATY_NONE, UART_STOPBITS_ONE);
	gptm_set_timer_32periodic(TIMER0);

	cli_rx = (cli_rx_fpt)uart0_getc;
	cli_tx = (cli_tx_fpt)uart0_putc;	 	
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
