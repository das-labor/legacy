/**
 * 
 * 
 * 
 * 
 * 
 * 
 */
 
#include <stdint.h>
#include <avr/io.h> 
#include <util/delay.h> 
#include <stdlib.h> /* random(), malloc(), free() */
#include <string.h> /* memset */
#include "uart.h"

#include "lop.h"

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

/******************************************************************************/

void wait(int ms){
	TCCR2 = 0x0C;				/* CTC Mode, clk/64 */
	OCR2 = 125;					/* 1000Hz */
	for(;ms>0;ms--){
		while(!(TIFR&0x80));	/* wait for compare matzch flag */
		TIFR=0x80;				/* reset flag */
	}
}

/******************************************************************************/

lop_ctx_t lop0={
	idle, idle, idle, 0, 0, NULL, 0, 
	NULL, NULL, NULL, NULL, NULL};

/******************************************************************************/

void lop0_sendrawbyte(uint8_t b){
	uart_putc((char)b);
}

/******************************************************************************/

void lop0_streamrx(uint8_t b){
	PORTC = b;
}

/******************************************************************************/

void lop0_streamstart(){	
}

/******************************************************************************/

void lop0_messagerx(uint16_t length, uint8_t * msg){
	uint8_t b;
	if(!length)
		return;
	b=*msg;
	while(length--){
		if(b!=*msg++)
			return;
	}
	PORTC=b;
}

/******************************************************************************/

void onuartrx(uint8_t b){
	lop_recieve_byte(&lop0,b);
}

/******************************************************************************/

void sendport(uint8_t b){
	uint8_t * msg=NULL;
	uint16_t length;
	do{
		length = 3; //rand() & 0x02FF;
	}while(!(msg=(uint8_t*)malloc(length)));
	memset(msg, b, length);
	lop_sendmessage(&lop0, length, msg);
	free(msg);
}

/******************************************************************************/

int main(){
	DDRC = 0xFF;
	PORTC = 0x01;
	DDRB = 0x00;
	PORTB = 0XFF;
	
	uart_init();
	display_init();
	display_clear_line(1);
	display_clear_line(0);
	
	uart_hook = onuartrx;
	lop0.on_streamrx = lop0_streamrx;
	lop0.sendrawbyte = lop0_sendrawbyte;
	lop0.on_streamstart = lop0_streamstart;
	lop0.on_msgrx = lop0_messagerx;
	lop_sendreset(&lop0);
//	lop_streamstart(&lop0);
	uint8_t pb,x=1,y=1;
	while(1){
		if(((pb=PINB & 0x0f)) != 0x0f){
			pb = (~pb & 0x0f);
		
			if(pb & 1){
				x >>= 1;
				if(!x) 
					x = 0x80;
				sendport(x);
				PORTC=x;
			}
			
			if(pb & 2) {
				x <<= 1;
				if(!x) 
					x = 0x01;
				sendport(x);
				PORTC=x;
			}
			if(pb & 4){
				lop_sendstream(&lop0, y);
				PORTC=y;
				++y;
			}
			if(pb & 8){
				lop_sendreset(&lop0);
				lop_streamstart(&lop0);
			}
			
			wait(500);
		}
	}

	return 0;
}



