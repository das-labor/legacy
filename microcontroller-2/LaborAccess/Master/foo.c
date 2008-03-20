
#include <avr/io.h>

#include "xlap.h"

void printhallo(){
	//printf("Hallo\n");
	//DDRC |= 0x04;
	//PORTC |= 0x04;
}



void hexdump(unsigned char * addr, unsigned char size){
	unsigned char x=0, sbuf[3];
	
	while(size--){
		itoa(*addr++, sbuf, 16);
		if (sbuf[1] == 0) stream_putc(' ');
		
		stream_putc(sbuf[0]);
		
		if (sbuf[1] != 0)
			stream_putc(sbuf[1]);
		
		//stdout_putstr(sbuf);
		//stdout_putc(' ');
		if(++x == 16){
			stream_putc('\n');
		
			//stdout_putstr_P(PSTR("\r\n"));
			x = 0;
		}
	}
}
