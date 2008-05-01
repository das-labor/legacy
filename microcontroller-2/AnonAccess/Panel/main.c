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
				writeABtoCard(&ab);
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
		writeABtoCard(&ab);
		ui_statusstring[3]='X';	
		return;
	}
}

void lop0_streamrx(uint8_t b){
	
}



int main(void){
	ui_loglist_t log;
	ui_loginit(&log,12);
	
PGM_P gpl_text= PSTR(
    "Copyright (C) 2008 \007 Daniel Otte\n"
    "\n"
    "This program is free software: you can redistribute it and/or modify\n"
    "it under the terms of the GNU General Public License as published by\n"
    "the Free Software Foundation, either version 3 of the License, or\n"
    "(at your option) any later version.\n"
    "\n"
    "This program is distributed in the hope that it will be useful,\n"
    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
    "GNU General Public License for more details.\n"
    "\n"
    "You should have received a copy of the GNU General Public License\n"
    "along with this program.  If not, see <http://www.gnu.org/licenses/>.");
    
	//Initialisierung
	 ui_logappend(&log, PSTR("lcd init"), flash_st, processing_st);
	lcd_init();
	 ui_logchangelaststate(&log, success_st);
	 //---
	 ui_logappend(&log, PSTR("keypad init"), flash_st, processing_st);
	keypad_init();	
	 ui_logchangelaststate(&log, success_st);
	//---
	 ui_logappend(&log, PSTR("user interface init"), flash_st, processing_st);
	ui_primitives_init();
	 ui_logchangelaststate(&log, success_st);
	//---
	lcd_cls();
	 ui_logappend(&log, PSTR("bla"), flash_st, informative_st);
	 ui_logappend(&log, PSTR("blub"), flash_st, informative_st);
	 ui_logappend(&log, PSTR("foo"), flash_st, informative_st);
	 ui_logappend(&log, PSTR("bar"), flash_st, informative_st);
	 ui_logappend(&log, PSTR("foobar"), flash_st, informative_st);
	
	ui_logprint(1,1,LCD_WIDTH,LCD_HEIGHT, &log);
	ui_waitforkey('E');
	ui_logreader(1,1,LCD_WIDTH, LCD_HEIGHT, &log);
	ui_drawframe(1,1,LCD_WIDTH,LCD_HEIGHT,'*');
	lcd_gotopos(2,3);
	lcd_writestr("booting ...");
	uint8_t i;
	for(i=0; i<255; ++i){
//		print_progressbar(i/255.0, 2, 3, LCD_WIDTH-2);
//		_delay_ms(10);
	}
//	lcd_cls();
//	radioselect("anon\0not anon\0");
//	radioselect_P(PSTR("bla\0blub\0foo\0bar\0foobar\0"));
//	checkselect_P(PSTR("A\0B\0C\0D\0E\0F\0G\0H\0"),(uint8_t*)"\0");
//	ui_hexdump("bla", 57);
	ui_textwindow_P(1,1,LCD_WIDTH,LCD_HEIGHT, gpl_text);
	resetcnt_inc();
	uart_init();
	uart_putc(XON);
	spi_init();
	i2c_init();
	E24C04_init();
  //  E24C_init();
 	rtc_init();
 	prng_init();
 	DDRC = 0xF0;
 //	PORTC|= 0xF0;
 
	//Set I2C SPEED 
	i2c_set_speed(I2C_SPEED_FASTEST);
	
	//set handlers for the outputs from lop
	lop0.on_streamrx = lop0_streamrx;
	lop0.sendrawbyte = lop0_sendrawbyte;
	lop0.on_streamsync = lop0_streamsync;
	lop0.on_msgrx = lop0_messagerx;
	uart_hook = onuartrx;
	lop_recieve_byte(&lop0, LOP_RESET_CODE);
	//Interupts global aktivieren
	sei();
	
//	lop_dbg_str_P(&lop0,PSTR("\r\nMAIN\r\n"));
		
	ui_statusstring[0]='a';
	
	while(1){
		master_menu();
	}
	
	return 0;
} 

