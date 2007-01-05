#include "console.h"
#include "stdlib.h"
#include "7seg.h"
#include "uart.h"
#define stdin_getc_nb uart_getc_nb
#define stdin_getc uart_getc
//#define stdout_putc uart_putc
//#define stdout_putstr uart_putstr
//#define stdout_putstr_P uart_putstr_P

#define stdout_putc seg_putc
#define stdout_putstr seg_putstr
#define stdout_putstr_P seg_putstr_P


/*
void hexinput(unsigned char * buf){
	unsigned char tmp, x=0, sbuf[3];
	
	sbuf[2] = 0;
	
	while(1){
		tmp = stdin_getc();
		if (tmp == 'q') return;
		stdout_putc(tmp);
		sbuf[x++] = tmp;
		if(x == 2){
			x = 0;
			*buf++ = (unsigned char)strtol(sbuf,0, 16);
		}

	}
}
*/

void hexdump(unsigned char * addr, unsigned char size){
	unsigned char x=0, sbuf[3];
	
	while(size--){
		itoa(*addr++, sbuf, 16);
		if (sbuf[1] == 0) stdout_putc('0');
		stdout_putstr(sbuf);
		//stdout_putc(' ');
		if(++x == 16){
			//stdout_putstr_P(PSTR(""));
			x = 0;
		}
	}
}
