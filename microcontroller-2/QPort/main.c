
#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "prng.h"
#include "error-handling.h"
#include "qcrypt.h"
#include "qport.h"
#include "persistentCounter.h"
#include "uart.h"
#include "debug.h"


#ifdef DISPLAY
	#include "hd44780.h"
	#define display_init hd44780_init
	#define display_print hd44780_print
	#define display_print_P hd44780_print_P
	#define display_putc  hd44780_data
	#define display_set_cursor hd44780_set_cursor
	#define display_clear_line hd44780_clear_line
	#define display_command hd44780_command
#else
	#define display_init() 
	#define display_print(s) 
	#define display_print_P(s) 
	#define display_putc(s)  
	#define display_set_cursor(a,b)
	#define display_clear_line(s) 
	#define display_command(s) 
#endif

void wait(int ms){
	TCCR2 = 0x0C;				/* CTC Mode, clk/64 */
	OCR2 = 125;					/* 1000Hz */
	for(;ms>0;ms--){
		while(!(TIFR&0x80));	/* wait for compare matzch flag */
		TIFR=0x80;				/* reset flag */
	}
}

void onrx(uint8_t b){
	qp_byterx(b);
}


void onrxmsg(uint16_t length, uint8_t* buffer){
	PORTC = (length>0)?buffer[0]:0;
}


int main (void){	
	DDRB = 0x00;
	PORTB = 0XFF;
	PORTC = 0x01; 
	DDRC = 0xFF;

	uart_init();
	display_init();
	display_clear_line(1);
	display_clear_line(0);
	percnt_init();
	uart_hook = onrx;
	msgrx_hook = onrxmsg;
	qp_byterx(0x23);	/* reset ourself */
	display_putc('b');
	rekey();
	
	{	
		uint32_t x;
		x = percnt_get(); 
		addEntropy(32,(void*)&x);
	}

	wait(100);
	
	uint8_t x=1;
	while(1){
		if((PINB & 0x03) != 0x03){
			if(PINB & 0x01){
				x >>= 1;
				if(!x) 
					x = 0x80;
			} else {
				x <<= 1;
				if(!x) 
					x = 0x01;
			}
			msgtx(1, &x);
			wait(500);
		}
	}
}

