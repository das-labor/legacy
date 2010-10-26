/* uart-tools.c */
/*
    This file is part of the AVR-Crypto-Lib.
    Copyright (C) 2009 Daniel Otte (daniel.otte@rub.de)

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

#include <stdint.h>
#include <avr/pgmspace.h>
#include "uart-tools.h"
#include "uart.h"

void uart_hexdump(const void* buf, int len)
{
	while(len--){
		uart_putc(pgm_read_byte(hexdigit_tab_P + ((*((uint8_t*)buf))>>4)));
		uart_putc(pgm_read_byte(hexdigit_tab_P + ((*((uint8_t*)buf))&0xf)));
		uart_putc(' ');
		buf=(uint8_t*)buf+1;
	}
}

void uart_putstr(char *str) {
	while(*str) {
		uart_putc(*str++);
	}
}

void uart_putstr_P(PGM_P str) {
	char tmp;
	while((tmp = pgm_read_byte(str))) {
		uart_putc(tmp);
		str++;
	}
}
