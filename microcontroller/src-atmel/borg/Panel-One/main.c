
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

	unsigned char pos=0, buf[10];
	while(1){
		unsigned char c=uart_getc();
		switch (c){
				case '-':
						pos--;
						break;
				case '+':
						pos++;
						break;	
		}
		itoa(pos, buf, 16);
		uart_putstr(buf);
		uart_putstr_P(PSTR("\n\r"));
		clear_screen(0);
		setpixel(pos, 3);
	
	}
	
}
