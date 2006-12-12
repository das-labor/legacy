
#include <setjmp.h>
#include <avr/interrupt.h>

#include "config.h"
#include "borg_hw.h"
#include "pixel.h"
#include "borg_can.h"
#include "uart.h"
#include <stdlib.h>

jmp_buf newmode_jmpbuf;

int main (void){
	unsigned char mode;

	clear_screen(0);
	borg_hw_init();
	bcan_init();	
	sei();

	uart_init();
	uart_putstr("Hallo\r\n");

		clear_screen(0);
	
	unsigned char x;
	for(x=0;x!=255;x++){
		setpixel(x,3);
		wait(20);
	}	
		
		
	unsigned char pos=0, buf[10], bright=3;
	
	while(1){
		uint8_t x;
		for(x=0;x<8;x++){
			itoa(keys[x],buf,16);
			uart_putstr(buf);
			uart_putc(' ');
		}
		uart_putc('\r');
	
	
	
	}
	
	
	
	while(1){
		unsigned char c=uart_getc();
		switch (c){
				case '-':
						pos--;
						break;
				case '+':
						pos++;
						break;	
				case 't':
						if (bright++ == 3) bright = 0;
						break;
				case '#':
					{
					unsigned int x;
					unsigned char buf[10];
					for(x=0;x<65535;x++){
						itoa(x,buf,16);
						seg_print(buf);
						wait(10);
						
						}
					
					}
					break;
				
		}
		itoa(pos, buf, 16);
		uart_putstr(buf);
		uart_putstr_P(PSTR("\n\r"));
		setpixel(pos, bright);

		itoa(keys[pos],buf,16);
		seg_print(buf);
	}
	
}
