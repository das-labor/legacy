/* main-arcfour-test.c */
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
 * arcfour (RC4 compatible) test-suit
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

#include "arcfour.h"
#include "scal_arcfour.h"

#include "scal-nessie.h"
#include "nessie_stream_test.h"
#include "performance_test.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

const char* algo_name = "Arcfour";

void uart0_putc(char byte){
	uart_putc(UART_0, byte);
}

char uart0_getc(void){
	return uart_getc(UART_0);
}

/*****************************************************************************
 *  additional validation-functions											 *
 *****************************************************************************/



void testrun_nessie_arcfour(void){
	scal_nessie_run(&arcfour_desc);
}

void testrun_performance_arcfour(void){
	uint64_t t;
	char str[16];
	uint8_t key[16];
	arcfour_ctx_t ctx;
	
	calibrateTimer();
	print_overhead();	
	
	memset(key,  0, 16);
	
	startTimer(1);
	arcfour_init(key, 16, &ctx);
	t = stopTimer();
	cli_putstr("\r\n\tctx-gen time: ");
	ultoa((unsigned long)t, str, 10);
	cli_putstr(str);	
	
	startTimer(1);
	arcfour_gen(&ctx);
	t = stopTimer();
	cli_putstr("\r\n\tencrypt time: ");
	ultoa((unsigned long)t, str, 10);
	cli_putstr(str);	
	
	cli_putstr("\r\n");
}


/*****************************************************************************
 *  main																	 *
 *****************************************************************************/

const cmdlist_entry_t cmdlist[] = {
	{ "nessie",      NULL, testrun_nessie_arcfour },
	{ "test",        NULL, testrun_nessie_arcfour},
	{ "performance", NULL, testrun_performance_arcfour},
	{ "echo",    (void*)1, (void_fpt)echo_ctrl},
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
