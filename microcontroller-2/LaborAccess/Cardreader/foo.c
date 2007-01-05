#include "stdlib.h"
#include "console.h"

#define display_putc hd44780_data
#define display_putstr hd44780_print
#define display_putstr_P hd44780_print_P

#include "hd44780.h"


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


void hexdump(unsigned char * addr, unsigned char size){
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


#define DDR_POWER DDRC
#define PORT_POWER PORTC
#define BIT_POWER 0

#define CARDPOWER_ON() PORT_POWER &= ~(1<<BIT_POWER)
#define CARDPOWER_OFF() PORT_POWER |= (1<<BIT_POWER)

void cardpower_off(){
		CARDPOWER_OFF();
	
}

void cardpower_on(){
		CARDPOWER_ON();
	
}

void hexdump_d(unsigned char * addr, unsigned char size){
	unsigned char x=0, sbuf[3];
	
	while(size--){
		itoa(*addr++, sbuf, 16);
		if (sbuf[1] == 0) display_putc(' ');
		display_putstr(sbuf);
		display_putc(' ');
		if(++x == 16){
			display_putstr_P(PSTR("\r\n"));
			x = 0;
		}
	}
}
