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

#define UART_NI 1

#include "cli.h"
#include "dump.h"
#include "uart_ni.h"
#include "usbhw.h"

#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/io.h>

void test(void){
	cli_putstr_P(PSTR("\r\nHello World!\r\n"));
}

void foo(void){
	cli_putstr_P(PSTR("\r\nfoo\r\n"));
}

void foobar(void){
	cli_putstr_P(PSTR("\r\nfoobar\r\n"));
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

void usb_tx_zeros(){
	uint8_t buffer[32];
	memset(buffer, 0, 32);
	usb_tx_block(buffer, 32);
	cli_putstr_P(PSTR("\r\nsuccess!"));
}

void usb_tx_ones(){
	uint8_t buffer[32];
	memset(buffer, 0xff, 32);
	usb_tx_block(buffer, 32);
	cli_putstr_P(PSTR("\r\nsuccess!"));
}

void usb_tx_0loop(){
	uint8_t buffer[32];
	memset(buffer, 0x00, 32);
	while(!uart0_dataavail()){
		usb_tx_block(buffer, 32);

	}
}

void usb_tx_1loop(){
	uint8_t buffer[32];
	memset(buffer, 0xff, 32);
	while(!uart0_dataavail()){
		usb_tx_block(buffer, 32);

	}
}


/******************************************************************************/

const char test_str[]    PROGMEM = "test";
const char print_str[]   PROGMEM = "print";
const char beep_str[]    PROGMEM = "beep";
const char dump_str[]    PROGMEM = "dump";
const char echo_str[]    PROGMEM = "echo";
const char foo_str[]     PROGMEM = "foo";
const char foobar_str[]  PROGMEM = "foobar";
const char usb0_str[]    PROGMEM = "usb0";
const char usb1_str[]    PROGMEM = "usb1";
const char usb0l_str[]   PROGMEM = "usb0loop";
const char usb1l_str[]   PROGMEM = "usb1loop";


cmdlist_entry_t cmdlist[] PROGMEM = {
	{ test_str,        NULL, test},
	{ print_str,   (void*)1, (void_fpt)print},
	{ beep_str,        NULL, beep},
	{ dump_str,    (void*)1, (void_fpt)dump}, 
	{ echo_str,    (void*)1, (void_fpt)echo_ctrl},
	{ foobar_str,      NULL, foobar},
	{ foo_str,         NULL, foo},
	{ usb0_str,        NULL, usb_tx_zeros},
	{ usb1_str,        NULL, usb_tx_ones},
	{ usb0l_str,       NULL, usb_tx_0loop},
	{ usb1l_str,       NULL, usb_tx_1loop},
	{ NULL,            NULL, NULL}
};

int main (void){
	uart0_init();
	cli_rx = uart0_getc;
	cli_tx = uart0_putc;
	for(;;){
		cli_putstr_P(PSTR("\r\n\r\nAVR-USBHost testing system"));
		cli_putstr_P(PSTR("\r\n  *** loaded and running*** \r\n"));
		cmd_interface(cmdlist);
	}
}

