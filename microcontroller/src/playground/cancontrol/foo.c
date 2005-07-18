#include "console.h"
#include "stdlib.h"


void hex_input(unsigned char * buf){
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

void hex_dump(unsigned char * addr, unsigned char size){
	unsigned char x=0, sbuf[3];
	
	while(size--){
		itoa(*addr++, sbuf, 16);
		if (sbuf[1] == 0) stdout_putc(' ');
		stdout_putstr(sbuf);
		stdout_putc(' ');
		if(++x == 16){
			stdout_putstr_P(PSTR("\r\n"));
			x = 0;
		}
	}
}



