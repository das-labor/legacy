/* main-uart_i-test.c */
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

#include "uart_i.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

void printstr_P(PGM_P s){
	unsigned char c;
	for(;;){
		c = pgm_read_byte(s++);
		if(c=='\0')
			return;
		uart0_putc(c);
	}
}

void printhex(unsigned char c){
	static char ht[] = {'0','1','2','3',
	                    '4','5','6','7',
	                    '8','9','A','B',
	                    'C','D','E','F' };
	uart0_putc(ht[c>>4]);
	uart0_putc(ht[c&0xf]);
}

void myhook(uint8_t c){
	printstr_P(PSTR("\r\nhook got: "));
	printhex(c);
	if(c=='u')
		uart0_sethook(NULL);
}

/******************************************************************************/

int main (void){
	uart0_init();
	printstr_P(PSTR("\r\nuart 0 initialized\r\n"));
	unsigned char c;
	uart0_putc(0x11);
	DDRD |= _BV(5) | _BV(6);
	for(;;){
	//	for(;;);
		c = uart0_getc();
		printhex(c);
		uart0_putc(' ');
		if(c=='h')
			uart0_sethook(myhook);
	}
}


