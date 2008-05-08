/**
 * AnonAccess-Terminal
 * Author: Martin Hermsen & Daniel Otte
 * License: GPLv3
 * 
 * 
 */ 

#include "config.h"
#include "lcd_tools.h"
#include "keypad.h"
#include "spi.h"
#include "uart.h"
#include "menu.h"
#include "lop.h"
#include "i2c.h"
#include "i2c_tools.h"
#include "24Cxxx.h"
#include "hwrnd.h"
#include "lop_debug.h"
#include "qport.h"
#include "rtc.h"
#include "base64_enc.h"
#include "reset_counter.h"
#include <stdint.h>
#include "comm.h"
#include "24C04.h"
#include "cardio.h"
#include "ui_primitives.h"
#include "logs.h"
#include <ctype.h>
#include <string.h>

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

/******************************************************************************/

uint16_t purify_str(char* str){
	uint16_t i=0,len=strlen(str);
	while(i<len && (!isprint(str[i]) || isspace(str[i])) )
		++i;
	memmove(str, str+i, len-i);
	return len-i;
}

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

lop_ctx_t lop0={
	idle, idle, idle, 0, 0, NULL, 0, 
	NULL, NULL, NULL, NULL};

qport_ctx_t qp0;

lop_ctx_t lop1={
	idle, idle, idle, 0, 0, NULL, 0, 
	NULL, NULL, NULL, NULL};

/******************************************************************************/

// this handler is called from the uart_hook, i.e. when the Uart receives
// a new byte.
void onuartrx(uint8_t b){
	//let lop handle the received byte.
	ui_statusstring[0]='a'+(1+(ui_statusstring[0]-'a'))%26;
	lop_recieve_byte(&lop0,b);
}

void lop0_sendrawbyte(uint8_t b){
	uart_putc((char)b);
}

void lop0_streamrx(uint8_t b){
	qport_recieve_byte(&qp0, b);
}


void lop0_streamsync(void){	
}

void lop0_messagerx(uint16_t length, uint8_t * msg){
	qport_incomming_msg(&qp0, length, msg);
}

/******************************************************************************/

void qp0_streamrx(uint8_t b){
	lop_recieve_byte(&lop1, b);
}

/******************************************************************************/

void lop1_sendrawbyte(uint8_t b){
	qport_streamsend(&qp0, b);
}

void lop1_streamrx(uint8_t b){
}

void lop1_messagerx(uint16_t length, uint8_t * msg){
	ui_statusstring[1]='o';
	if(length<3){
		/* DROP */
		return;
	}
	ui_statusstring[1]='q';

	if((msg[0]!=TERMINALUNIT_ID) || (msg[1]!=MASTERUNIT_ID)){
		ui_statusstring[1]='w';
		/* DROP */
		return;
	}
	ui_statusstring[1]='Q';
	if(msg_wait){	/* there is a blocking reques for a message */
		if((msg_data=lop1.msgbuffer)){
			lop1.msgbuffer=NULL;
			msg_length=length;
		}
		msg_wait=0;
		return;
	}
	if(msg[2]==MSGID_PRINT){
		if(msg[3]>STR_CLASS_MAX) 
			return; /* string class out of range */
		if(length!=5+msg[4])
			return; /* incorrect length */
		lcd_cls();
		ui_drawframe(1,1,LCD_WIDTH, LCD_HEIGHT,pgm_read_byte(str_class_char_P+msg[3]));
		lcd_gotopos(2,2);
		uint16_t strlen;
		strlen=purify_str((char*)msg+5);
		lcd_writelinen((char*)msg+5, MIN(strlen, LCD_WIDTH-2));
		ui_logappend(&masterlog, msg+5, copy_st, msg[3]);
		return;
	}
	if(msg[2]==MSGID_ACTION_REPLY){
		if(length<4)
			return;
		ui_statusstring[1]='O';	
		if(msg[3]==ACTION_ADDUSER){
			if(length<5)
				return;
			ui_statusstring[1]='p';	
			if(msg[4]==DONE){
				if(length!=5+sizeof(authblock_t)){
					ui_statusstring[1]='v';	
					return;
				}
				memcpy(&ab, &(msg[5]), sizeof(authblock_t));
				card_writeAB(&ab);
				ui_statusstring[2]='X';
				return;
			}
			ui_statusstring[1]='V';	
			
		}
	}
	if(msg[2]==MSGID_AB_REPLY){
		if(length!=3+sizeof(authblock_t)){
			return;
		}
		memcpy(&ab, &(msg[3]), sizeof(authblock_t));
		card_writeAB(&ab);
		ui_statusstring[3]='X';	
		return;
	}
}

/******************************************************************************/

int main(void){
	
	//Initialisierung
	init_logs();
	
	BOOTLOG_APPEND_P("LCD init");
	lcd_init();
	BOOTLOG_APPEND_OK;
	 //---
	BOOTLOG_APPEND_P("Keypad init");
	keypad_init();	
	BOOTLOG_APPEND_OK;
	 //---
	BOOTLOG_APPEND_P("UI init");
	ui_primitives_init();
	BOOTLOG_APPEND_OK;
	//---
	ui_drawframe(1,1,LCD_WIDTH,LCD_HEIGHT,'*');
	lcd_gotopos(2,3);
	lcd_writestr("booting ...");
	
	BOOTLOG_APPEND_P("Reset counter++");
	resetcnt_inc();
	BOOTLOG_APPEND_OK;
	
	BOOTLOG_APPEND_P("UART init");
	uart_init();
	uart_putc(XON);
	BOOTLOG_APPEND_OK;
	
	BOOTLOG_APPEND_P("LOP0 init");
	lop0.msgbuffer = NULL;
	lop0.msgidx = 0;
	lop0.on_streamrx = lop0_streamrx;
	lop0.sendrawbyte = lop0_sendrawbyte;
	lop0.on_msgrx = lop0_messagerx;
	BOOTLOG_APPEND_OK;
	
	BOOTLOG_APPEND_P("QPORT init");
	qp0.keystate = unkeyed;
	qp0.master_enc_key_rxa = qp0_key_rxa;
	qp0.master_enc_key_rxb = qp0_key_rxb;
	qp0.master_enc_key_txa = qp0_key_txa;
	qp0.master_enc_key_txb = qp0_key_txb;
	qp0.master_mac_key = qp0_key_hmac;
	qp0.on_byterx = qp0_streamrx;
	qp0.lop = &lop0;
	qp0.keyingdata = 0;
	BOOTLOG_APPEND_OK;
	
	BOOTLOG_APPEND_P("LOP1 init");
	lop1.on_streamrx = lop1_streamrx;
	lop1.sendrawbyte = lop1_sendrawbyte;
	lop1.on_msgrx = lop1_messagerx;
	lop_sendreset(&lop0);
	uart_hook = onuartrx;
	
	
	if(qp0.keystate == unkeyed)
		qport_rekey(&qp0);
		
	BOOTLOG_APPEND_OK;
	
	BOOTLOG_APPEND_P("SPI init");
	//spi_init();
	BOOTLOG_APPEND_OK;
	
	BOOTLOG_APPEND_P("RTC init");
	//rtc_init();
 	BOOTLOG_APPEND_OK;
	
 	BOOTLOG_APPEND_P("PRNG init");
	prng_init();
 	BOOTLOG_APPEND_OK;
	
	BOOTLOG_APPEND_P("I2C init");
	i2c_init();
	i2c_set_speed(I2C_SPEED_FASTEST);
	DDRC = 0xF0;	
	BOOTLOG_APPEND_OK;
	
 //	PORTC|= 0xF0;
 
	//Set I2C SPEED 
/*	
	//set handlers for the outputs from lop
	BOOTLOG_APPEND_P("LOP init");
	lop0.on_streamrx = lop0_streamrx;
	lop0.sendrawbyte = lop0_sendrawbyte;
	lop0.on_streamsync = lop0_streamsync;
	lop0.on_msgrx = lop0_messagerx;
	uart_hook = onuartrx;
	lop_recieve_byte(&lop0, LOP_RESET_CODE);
	BOOTLOG_APPEND_OK;
*/	
	//Interupts global aktivieren
	sei();
	
//	lop_dbg_str_P(&lop0,PSTR("\r\nMAIN\r\n"));
		
//	ui_logreader(1,1,LCD_WIDTH,LCD_HEIGHT, &log);
//	ui_textwindow_P(1,1,LCD_WIDTH,LCD_HEIGHT, gpl_text);
	
	while(1){
		ui_menuexec(main_menu_mt);
	}
	
	return 0;
} 

