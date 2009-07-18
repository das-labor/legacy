/* main-huffman-test.c */
/*
    This file is part of the AVR-Huffman.
    Copyright (C) 2009  Daniel Otte (daniel.otte@rub.de)

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

#include "cli.h"
#include "dump.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/io.h>

void test(void){
	cli_putstr_P(PSTR("\r\nHello World!\r\n"));
}

void print(char* s){
	cli_putc('\r');
	cli_putc('\n');
	cli_putstr(s);
}

void beep(void){
	DDRD |= _BV(7);
	TCCR2A |= _BV(6); /* set toggle of OCR2A */
	TCCR2A |= _BV(1); /* set CTC mode */
	TCNT2 = 0;
	OCR2A = 0x20;
	TCCR2B |=  _BV(1) | _BV(0); /* set prescaler to 1024 */
}

/******************************************************************************/

const char test_str[]    PROGMEM = "test";
const char print_str[]   PROGMEM = "print";
const char beep_str[]    PROGMEM = "beep";
const char dump_str[]    PROGMEM = "dump";
const char echo_str[]    PROGMEM = "echo";

cmdlist_entry_t cmdlist[] PROGMEM = {
	{ test_str,        NULL, test},
	{ print_str,   (void*)1, (void_fpt)print},
	{ beep_str,        NULL, beep},
	{ dump_str,    (void*)1, (void_fpt)dump}, 
	{ echo_str,    (void*)1, (void_fpt)echo_ctrl},
	{ NULL,            NULL, NULL}
};

int main (void){
	uart_init();
	cli_rx = uart_getc;
	cli_tx = uart_putc;
	for(;;){
		cli_putstr_P(PSTR("\r\n\r\nAVR-CLI testing system"));
		cli_putstr_P(PSTR("\r\n  *** loaded and running*** \r\n"));
		cmd_interface(cmdlist);
	}
}

