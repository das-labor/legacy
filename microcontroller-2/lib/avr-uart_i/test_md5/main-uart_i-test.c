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

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "md5.h"
#include "uart_i.h"

void printstr_P(PGM_P s){
	unsigned char c;
	for(;;){
		c = pgm_read_byte(s++);
		if(c=='\0')
			return;
		uart0_putc(c);
	}
}

void printstr(char* s){
	while(*s){
		uart0_putc(*s++);
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

void md5_hash(char* str){
	uint16_t len = strlen(str);
	md5_hash_t hash;
	md5(&hash, str, len*8);
	uint8_t i;
	printstr_P(PSTR("\r\n"));
	printstr(str);
	printstr_P(PSTR(": "));
	for(i=0; i<16; ++i)
		printhex(((uint8_t*)hash)[i]);
}

/*
 *
 * MD5 test suite:
 * MD5 ("") = d41d8cd98f00b204e9800998ecf8427e
 * MD5 ("a") = 0cc175b9c0f1b6a831c399e269772661
 * MD5 ("abc") = 900150983cd24fb0d6963f7d28e17f72
 * MD5 ("message digest") = f96b697d7cb7938d525a2f31aaf161d0
 * MD5 ("abcdefghijklmnopqrstuvwxyz") = c3fcd3d76192e4007dfb496cca67e13b
 * MD5 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") =
 * d174ab98d277d9f5a5611c2c9f419d9f
 * MD5 ("123456789012345678901234567890123456789012345678901234567890123456
 * 78901234567890") = 57edf4a22be3c955ac49da2e2107b67a
 *
 */
void md5_test(void){
	md5_hash("");
	md5_hash("a");
	md5_hash("abc");
	md5_hash("message digest");
	md5_hash("abcdefghijklmnopqrstuvwxyz");
	md5_hash("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	md5_hash("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
}
/******************************************************************************/

int main (void){
	uart0_init();
	printstr_P(PSTR("\r\nuart 0 initialized\r\n"));
	unsigned char c;
	for(;;){
		c = uart0_getc();
		printhex(c);
		uart0_putc(' ');
		if(c == 'h'){
		    printstr_P(PSTR("\r\HOOK activated\r\n"));
			uart0_sethook(myhook);
		}
		if(c == 'm')
			md5_test();
	}
}


