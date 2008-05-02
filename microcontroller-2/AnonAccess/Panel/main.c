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
#include "rtc.h"
#include "base64_enc.h"
#include "reset_counter.h"
#include <stdint.h>
#include "comm.h"
#include "24C04.h"
#include "cardio.h"
#include "ui_primitives.h"
#include "logs.h"

lop_ctx_t lop0={
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

/******************************************************************************/

void lop0_streamsync(void){	
}

void lop0_sendrawbyte(uint8_t b){
	uart_putc((char)b);
}

void lop0_messagerx(uint16_t length, uint8_t * msg){
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
		msg_data=malloc(length);
		memcpy(msg_data, msg, length);
		msg_wait=0;
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

void lop0_streamrx(uint8_t b){
	
}

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
	
	BOOTLOG_APPEND_P("SPI init");
	spi_init();
	BOOTLOG_APPEND_OK;
	
	BOOTLOG_APPEND_P("RTC init");
	rtc_init();
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
	
	//set handlers for the outputs from lop
	BOOTLOG_APPEND_P("LOP init");
	lop0.on_streamrx = lop0_streamrx;
	lop0.sendrawbyte = lop0_sendrawbyte;
	lop0.on_streamsync = lop0_streamsync;
	lop0.on_msgrx = lop0_messagerx;
	uart_hook = onuartrx;
	lop_recieve_byte(&lop0, LOP_RESET_CODE);
	BOOTLOG_APPEND_OK;
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

