/**
 * \file	main-lop-avr-test.c
 * \author	Daniel Otte
 * \date	2007-07-10
 * \par license
 *  GPL
 * \brief demonstration of lop usage
 * 
 * 
 */

/* 
 * This demo uses lop to transfer lightpatterns between two laborboards via
 * a serial connection.
 * Two types of communication are used:
 *  -Messages 
 *  -Stream
 * 1) Messages have a random length and all bytes have the same value, which is
 *    to be displayed.
 *    +-+-+-+-+-     -+-+-+
 *    |b|b|b|b|b ... b|b|b|
 *    +-+-+-+-+-     -+-+-+
 * 2) Via stream frames are transfered. Theses frames are eight bytes long and
 *    start with the byte wich is to be displayed, the other bytes have random
 *    values.
 * 	  +-+-+-+-+-+-+-+-+
 *    |b|r|r|r|r|r|r|r|
 *    +-+-+-+-+-+-+-+-+
 */  
 
 
 
#include <stdint.h>
#include <avr/io.h> 
#include <util/delay.h> 
#include <stdlib.h> /* random(), malloc(), free() */
#include <string.h> /* memset */
#include "uart.h"

#include "lop.h"

/******************************************************************************/
/* GLOBAL VARIABLES                                                           */
/******************************************************************************/

uint8_t onsync=0;

lop_ctx_t lop0={
	idle, idle, idle, 0, 0, NULL, 0, 
	NULL, NULL, NULL, NULL};


/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/

void wait(int ms){
	TCCR2 = 0x0C;				/* CTC Mode, clk/64 */
	OCR2 = 125;					/* 1000Hz */
	for(;ms>0;ms--){
		while(!(TIFR&0x80));	/* wait for compare match flag */
		TIFR=0x80;				/* reset flag */
	}
}

/******************************************************************************/

void lop0_sendrawbyte(uint8_t b){
	uart_putc((char)b);
}

/******************************************************************************/

// this handler is called from the uart_hook, i.e. when the Uart receives
// a new byte.
void onuartrx(uint8_t b){
	//let lop handle the received byte.
	lop_recieve_byte(&lop0,b);
}

/******************************************************************************/

void lop0_streamrx(uint8_t b){
	static uint8_t i=0;
	if(onsync){
		if(i==0)
			PORTC = b;
		i++;
		i %= 8;
	}
}

/******************************************************************************/

void lop0_streamsync(){	
	onsync=1;
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

void sendportmsg(uint8_t b){
	uint8_t * msg=NULL;
	uint16_t length;
	do{
		length = rand() & 0x02FF;
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
	
	//set the handler for the bytes received by uart
	//this function is then called by uart.c
	uart_hook = onuartrx;
	
	//set handlers for the outputs from lop
	lop0.on_streamrx = lop0_streamrx;
	lop0.sendrawbyte = lop0_sendrawbyte;
	lop0.on_streamsync = lop0_streamsync;
	lop0.on_msgrx = lop0_messagerx;
	
	lop_sendreset(&lop0);
	uint8_t pb,x=1,y=1;
	while(1){
		if(((pb=PINB & 0x0f)) != 0x0f){
			pb = (~pb & 0x0f);
		
			if(pb & 1){
				x >>= 1;
				if(!x) 
					x = 0x80;
				sendportmsg(x);
				PORTC=x;
			}
			
			if(pb & 2) {
				x <<= 1;
				if(!x) 
					x = 0x01;
				sendportmsg(x);
				PORTC=x;
			}
			if(pb & 4){
				uint8_t i;
				lop_streamsync(&lop0);
				lop_sendstream(&lop0, y);
				for(i=0; i<7; ++i){
					lop_sendstream(&lop0, rand()&0xff);
				}
				PORTC=y;
				++y;
			}
			if(pb & 8){
				lop_sendreset(&lop0);
				y=0;
			}
			
			wait(500);
		}
	}

	return 0;
}
