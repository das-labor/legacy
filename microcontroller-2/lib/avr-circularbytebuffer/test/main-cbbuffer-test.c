/* main-cbbuffer-test.c */
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

#include "config.h"
#include "uart_ni.h"
#include "circularbytebuffer.h"
#include <stdlib.h>
#include <avr/pgmspace.h>

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

/******************************************************************************/

int main (void){
	uint16_t r;
	circularbytebuffer_t cbb;
	uint8_t buffer[10];
	circularbytebuffer_init2(10, &cbb, buffer);
	for(r=0;r<10;++r){
		cbb.buffer[r]=r;
	}
	uart0_init();
	printstr_P(PSTR("\r\nuart 0 initialized\r\n"));
	unsigned char c;
	for(;;){
		r = rand();
		c = uart0_getc();
		switch(c){
			case 'a':
				printstr_P(PSTR("\r\nappending: "));
				printhex(r);
				if(circularbytebuffer_append(r, &cbb)){
			    	printstr_P(PSTR("\r\nbuffer full!"));
				}
				break;
			case 'p':
				printstr_P(PSTR("\r\npushing: "));
				printhex(r);
				if(circularbytebuffer_push(r, &cbb)){
			    	printstr_P(PSTR("\r\nbuffer full!"));
				}
				break;
			case 'l':
				r = circularbytebuffer_get_lifo(&cbb);
				if(r==0xffff){
			    	printstr_P(PSTR("\r\nbuffer empty!"));
				}else{
					printstr_P(PSTR("\r\nlast: "));
					printhex(r);
				}
				break;
			case 'f':
				r = circularbytebuffer_get_fifo(&cbb);
				if(r==0xffff){
			    	printstr_P(PSTR("\r\nbuffer empty!"));
				}else{
					printstr_P(PSTR("\r\nlast: "));
					printhex(r);
				}
				break;
			case 'c':
				r = circularbytebuffer_cnt(&cbb);
				printstr_P(PSTR("\r\ncount: "));
				printhex(r);
				break;
			default:
				break;
		}
	}
}


