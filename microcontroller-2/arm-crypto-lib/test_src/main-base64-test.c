/* main-base64-test.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2008, 2009  Daniel Otte (daniel.otte@rub.de)

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
 * base64 test-suit
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
#include "base64_enc.h"
#include "base64_dec.h"
#include "string-extras.h"
#include "cli.h"
#include "performance_test.h"

void uart0_putc(char byte){
	uart_putc(UART_0, byte);
}

char uart0_getc(void){
	return uart_getc(UART_0);
}

char* algo_name = "Base64";

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/
#define TESTLENGTH 4096

void testrun_stdtest_base64(void){
	uint8_t fail=0;
	unsigned l,i;
	int     sl;
	char str[32];
	uint8_t bin_buffer[TESTLENGTH];
	char    b64_buffer[TESTLENGTH*4/3+10];
	uint8_t bin_buffer2[TESTLENGTH];
	random_seed(bin_buffer);
	
	for(l=0; l<TESTLENGTH-1; ++l){
		cli_putstr("\r\nTest ");
		ultoa(l, str, 10);
		cli_putstr(str);
		
		uart_flush(0);
		for(i=0; i<l; ++i){
			bin_buffer[i] = random8();
		}
		cli_putstr("\r\n bin: ");
		cli_hexdump(bin_buffer, l);

		uart_flush(0);
		base64enc(b64_buffer, bin_buffer, l);
		cli_putstr("\r\n b64: ");
		cli_putstr(b64_buffer);

		uart_flush(0);
		sl = base64_binlength(b64_buffer, 1);
		
		if(sl!=l){
			cli_putstr("\r\n ERROR length ");
			cli_putstr(str);
			if(sl!=-1){
				cli_putstr(" != ");
				ultoa(l, str, 10);
				cli_putstr(str);
			}else{
				cli_putstr(" != -1");
			}
			fail=1;
		}else{
			cli_putstr("\r\n length ok");
			uart_flush(0);
		}
		uart_flush(0);
		base64dec(bin_buffer2, b64_buffer, 1);
		if(memcmp(bin_buffer, bin_buffer2, l)){
			cli_putstr("\r\n ERROR value\r\n out: ");
			cli_hexdump(bin_buffer2, l);
			fail=1;
		}else{
			cli_putstr("\r\n value ok");
			uart_flush(0);
		}
		if(fail)
			break;
	}	
	cli_putstr(fail?"\r\n [FAIL]\r\n":"\r\n [OK]\r\n");
}

void testrun_performance_base64(void){
/*
	uint64_t t;
	char str[16];
	uint8_t key[16], data[16];
	noekeon_ctx_t ctx;
	
	calibrateTimer();
	print_overhead();
	
	memset(key,  0, 16);
	memset(data, 0, 16);
	
	startTimer(1);
	noekeon_init(key, &ctx);
	t = stopTimer();
	cli_putstr("\r\n\tctx-gen time: ");
	ultoa((unsigned long)t, str, 10);
	cli_putstr(str);	
	
	startTimer(1);
	noekeon_enc(data, &ctx);
	t = stopTimer();
	cli_putstr("\r\n\tencrypt time: ");
	ultoa((unsigned long)t, str, 10);
	cli_putstr(str);	
	
	startTimer(1);
	noekeon_dec(data, &ctx);
	t = stopTimer();
	cli_putstr("\r\n\tdecrypt time: ");
	ultoa((unsigned long)t, str, 10);
	cli_putstr(str);
	
	cli_putstr("\r\n");
*/
}
/*****************************************************************************
 *  main																	 *
 *****************************************************************************/

const char test_str[]        = "test";
const char performance_str[] = "performance";
const char echo_str[]        = "echo";

const cmdlist_entry_t cmdlist[] = {
	{ test_str,        NULL, testrun_stdtest_base64},
	{ performance_str, NULL, testrun_performance_base64},
	{ echo_str,    (void*)1, (void_fpt)echo_ctrl},
	{ NULL,            NULL, NULL}
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
