/**
 * \file main-qport-test.c
 * \author Daniel Otte
 * \date 2007-08-22
 * \par license
 *  GPLv3
 */
 
#include <stdint.h>
#include <avr/io.h> 
#include <avr/eeprom.h>
#include <util/delay.h> 

#include <stdlib.h> /* random(), malloc(), free() */
#include <string.h> /* memset */
#include <stdio.h>
#include "uart.h"
#include "qport.h"
#include "lop.h"
#include "entropium.h"

#include "hd44780.h"

/******************************************************************************/

/* keys from /dev/random */
uint8_t qp0_key_rxa[16] EEMEM = {0x3c, 0x71, 0x68, 0xa9, 0x6b, 0xf1, 0x63, 0xac,
                                 0xc4, 0x7f, 0x61, 0x00, 0xcb, 0x96, 0x3d, 0x74};
uint8_t qp0_key_rxb[16] EEMEM = {0xd4, 0x82, 0x18, 0x04, 0xd2, 0x29, 0xe2, 0x7e,
                                 0x7b, 0x41, 0x2a, 0x2e, 0x30, 0xc6, 0x5c, 0xbd};
uint8_t qp0_key_txa[16] EEMEM = {0x15, 0x36, 0xb4, 0x47, 0x62, 0xc2, 0xc8, 0x35,
                                 0x78, 0xb5, 0x4c, 0xa3, 0xcb, 0x10, 0x8b, 0x34};
uint8_t qp0_key_txb[16] EEMEM = {0xec, 0x78, 0x0c, 0xf0, 0x44, 0xf9, 0x6f, 0x4a,
                                 0xd5, 0xf6, 0x3a, 0x87, 0xde, 0x3d, 0x8f, 0x18};

uint8_t qp0_key_hmac[32] EEMEM = {0x8c, 0x40, 0xc8, 0x3c, 0x84, 0x11, 0x8a, 0xec,
                                  0x66, 0x66, 0x5d, 0x9f, 0x3b, 0x79, 0x55, 0x63,
                                  0x68, 0x3e, 0xc3, 0xb2, 0x20, 0xc6, 0xa7, 0x7e,
                                  0x13, 0x54, 0x88, 0x77, 0x02, 0x9b, 0xb9, 0x8a};


uint8_t display=0;  /* if we have display or don't  */

/******************************************************************************/
/*
 * typedef struct lop_ctx {
	lopstates_t rxstate, txstate, msgretstate;
	uint32_t msgidx;
	uint16_t msglength;
	uint8_t* msgbuffer;
	uint8_t escaped;
	void (*sendrawbyte)(uint8_t);           // pointer to the writing function
	void (*on_msgrx)(uint16_t, uint8_t*);   // function called on message recieve
	void (*on_streamrx)(uint8_t);           // function called on recieve of a stream byte
	void (*on_streamsync)(void);            // function called on recieve of streamsync
} lop_ctx_t;
*/ 
lop_ctx_t lop0={
	idle, idle, idle,
	0, 0, NULL, 0, 
	NULL, NULL, NULL, NULL};
qport_ctx_t qp0;

lop_ctx_t lop1={
	idle, idle, idle,
	0, 0, NULL, 0, 
	NULL, NULL, NULL, NULL};

/******************************************************************************/

uint8_t getbadrandom(void){
	ADCSRA |= 0x40 | 0x80; /* start conversion */
	while(ADCSRA & 0x40)   /* wait for conversation to complete */
		;
	return (uint8_t)ADC;
}

/******************************************************************************/

void prng_init(void){
	/* here we should add some entropy to the prng */
	uint8_t i,j;
	uint8_t b[64];
	for(i=0; i<48; ++i){
		for(j=0; j<64; ++j){
			b[j] = getbadrandom();
		}
		entropium_addEntropy(64*8,&b);
	}
}

/******************************************************************************/

void wait(int ms){
	while(ms--)	
		_delay_ms(1);
#if 0
	TCCR2 = 0x0C;				/* CTC Mode, clk/64 */
	OCR2 = 125;					/* 1000Hz */
	for(;ms>0;ms--){
		while(!(TIFR&0x80));	/* wait for compare matzch flag */
		TIFR=0x80;				/* reset flag */
	}
#endif	
}

/******************************************************************************/
void onuartrx(uint8_t b){
	lop_recieve_byte(&lop0,b);
}
/******************************************************************************/
void lop0_sendrawbyte(uint8_t b){
	uart_putc((char)b);
}
/******************************************************************************/
void lop0_streamrx(uint8_t b){
	qport_recieve_byte(&qp0, b);
}
/******************************************************************************/
void lop0_msgrx(uint16_t len, uint8_t * msg){
	qport_incomming_msg(&qp0, len, msg);
}
/******************************************************************************/
void qp0_streamrx(uint8_t b){
//	PORTC = b;
	lop_recieve_byte(&lop1, b);
}
/******************************************************************************/
void lop1_sendrawbyte(uint8_t b){
	qport_streamsend(&qp0, b);
}
/******************************************************************************/
void lop1_streamrx(uint8_t b){
#ifdef LED_DEBUG
	PORTC &= 0xF0;
	PORTC |= b&0x0F;
#else
	PORTC = b;
#endif
}
/******************************************************************************/
void lop1_msgrx(uint16_t len, uint8_t * msg){
	if(display){
		msg[len-1] = '\0';
		hd44780_clear_line(0);
		hd44780_print((char*)msg);
	}
}
/******************************************************************************/


int main(){
	uint8_t pb, x=0;
	DDRC = 0xFF;
	PORTC = 0x00;
	DDRB = 0x00;
	PORTB = 0XFF;
	
	DDRA = 0x00;
	PORTA = 0x00;
	ADMUX = 0x40;  /* Vref=Avcc, ADC0 */
	ADCSRA = 0x83; /* turn ADC on, prescaler=8 */
	
	uart_init();
	prng_init();
	
	display=!hd44780_init();
	if(display){
		hd44780_clear_line(0);
		hd44780_clear_line(1);
		hd44780_set_cursor(0,0);
		hd44780_print("Hello World!");
	}
	/* flash LEDs two times */
	PORTC = 0xFF;
	wait(200);
	PORTC = 0x00;
	wait(400);
	PORTC = 0xFF;
	wait(200);
	PORTC = 0x00;
	
	
	lop0.msgbuffer = NULL;
	lop0.msgidx = 0;
	lop0.on_streamrx = lop0_streamrx;
	lop0.sendrawbyte = lop0_sendrawbyte;
	lop0.on_msgrx = lop0_msgrx;
	
	qp0.keystate = unkeyed;
	qp0.master_enc_key_rxa = qp0_key_rxa;
	qp0.master_enc_key_rxb = qp0_key_rxb;
	qp0.master_enc_key_txa = qp0_key_txa;
	qp0.master_enc_key_txb = qp0_key_txb;
	qp0.master_mac_key = qp0_key_hmac;
	qp0.on_byterx = qp0_streamrx;
	qp0.lop = &lop0;
	qp0.keyingdata = 0;
	
	lop1.on_streamrx = lop1_streamrx;
	lop1.sendrawbyte = lop1_sendrawbyte;
	lop1.on_msgrx = lop1_msgrx;
	
	lop_sendreset(&lop0);
	uart_hook = onuartrx;
	
	wait(500);
	if(qp0.keystate == unkeyed)
		qport_rekey(&qp0);
	while(1){
		if(((pb=PINB & 0x0f)) != 0x0f){
			pb = (~pb & 0x0f);
			if(pb & 1){
			//	PORTC ^= 0x20;
				qport_rekey(&qp0);	
			}
			if(pb & 2){
				/*
				uart_putstr("\r\n krxa: "); uart_hexdump(qp0.streamkey_rxa, 16);
				uart_putstr("\r\n krxb: "); uart_hexdump(qp0.streamkey_rxb, 16);
				uart_putstr("\r\n ktxa: "); uart_hexdump(qp0.streamkey_txa, 16);
				uart_putstr("\r\n ktxb: "); uart_hexdump(qp0.streamkey_txb, 16);
				
				uart_putstr("\r\n srxa: "); uart_hexdump(qp0.streamstate_rxa, 8);
				uart_putstr("\r\n srxb: "); uart_hexdump(qp0.streamstate_rxb, 8);
				uart_putstr("\r\n stxa: "); uart_hexdump(qp0.streamstate_txa, 8);
				uart_putstr("\r\n stxb: "); uart_hexdump(qp0.streamstate_txb, 8);
				*/
				static uint16_t counter=0;
				char ts[30];
				sprintf(ts,"counter = %u", counter);
				counter++;
				lop_sendmessage(&lop1,strlen(ts)+1,(uint8_t*)ts);
				
			}
			if(pb & 4){
			//	PORTC ^= 0x20;
				x+=1;
			//	qport_streamsend(&qp0, x);
				lop_sendstream(&lop1, x);
			}
			if(pb & 8){
			//	PORTC ^= 0x20;
				x-=1;
			//	qport_streamsend(&qp0, x);
				lop_sendstream(&lop1, x);
			}
			wait(50);
		}
	}

	return 0;
}



