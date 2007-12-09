/**
 * \file	main.c
 * \author	Daniel Otte
 * \date	2007-12-06
 * \par license
 *  GPLv3
 * \brief mainpart of AnonAccess
 *
 */





#include "config.h"
#include "debug.h"
#include "uart.h"
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h> // itoa()
#include <avr/eeprom.h>
#include <stdlib.h>
#include "24Cxxx.h"
#include "i2c_tools.h"
#include "ticketDB.h"
#include "flmDB.h"
#include "prng.h"
#include "hmac-sha256.h"
#include "sha256.h"
#include "action.h"
#include "types.h"
#include "keys.h"
#include "reqvalidator.h"
#include "shabea.h"
#include "rtc.h"
#include "enc2E24C.h"
#include "hwrnd.h"
#include "lop.h"
#include "lop_debug.h"
#include "debug_tools.h"

/******************************************************************************/
/******************************************************************************/

#define HELP_STR "\r\n ==AnonAccess== " \
				 "\r\n  (debug console)" \
				 "\r\n h: print this help" \
				 "\r\n t: print timestamp" \
				 "\r\n s: print session info" \
				 "\r\n r: read serial eeprom in crypto mode (1KiB)" \
				 "\r\n R: read serial eeprom in crypto mode (8KiB)" \
				 "\r\n d: read serial eeprom in plain mode (1KiB)" \
				 "\r\n D: read serial eeprom in plain mode (8KiB)" \
				 "\r\n k: initiate keymigration" \
				 "\r\n f: format databases" \
				 "\r\n i: initialise databases" \
				 "\r\n"

/******************************************************************************/
/******************************************************************************/

uint8_t setupdone_flag EEMEM = 0;

struct {
	uint8_t users;
	uint8_t admins;
} session;

lop_ctx_t lop0={
	idle, idle, idle, 0, 0, NULL, 0, 
	NULL, NULL, NULL, NULL};
	
/******************************************************************************/
/******************************************************************************/

void lop0_sendrawbyte(uint8_t b){
	uart_putc(b);
}

/******************************************************************************/

// this handler is called from the uart_hook, i.e. when the Uart receives
// a new byte.
void onuartrx(uint8_t b){
	//let lop handle the received byte.
	lop_recieve_byte(&lop0,b);
}

/******************************************************************************/

bool setup_done(void){
	return eeprom_read_byte(&setupdone_flag);
}

/******************************************************************************/

void setup_system(void){
}

/******************************************************************************/

void streamrx(uint8_t b){
	i2c_detect_t dev_table;
	
	switch (b){
		case 'h': lop_dbg_str_P(&lop0, PSTR(HELP_STR)); 
			break;
		case 'h': i2c_detect(dev_table);
			break;
		case 't': console_dumptimestamp(); 
			break;
		case 'r': crypto_eeprom_dump(0, 1024);
			break;
		case 'R': crypto_eeprom_dump(0, 8*1024);
			break;
		case 'd': eeprom_dump_page(0xA0, 0, 1024);
			break;
		case 'D': eeprom_dump_page(0xA0, 0, 8*1024);
			break;
		case 's': lop_dbg_str_P(&lop0, PSTR("\r\nsession info:"));
			lop_dbg_str_P(&lop0, PSTR("\r\n\tusers: "));
			lop_dbg_hexdump(&lop0, &session.users, 1);
			lop_dbg_str_P(&lop0, PSTR("\r\n\tadmins: "));
			lop_dbg_hexdump(&lop0, &session.admins, 1);
			break;
		case 'k': do_keymigrate(); 
			break;
    	case 'f': ticketdb_format(TICKETDB_SIZE); 
    		flmdb_format();/* break;*/
    	case 'i': ticketdb_init();
    		dump_dbstats();
    		break;
    		
		default: lop_dbg_str_P(&lop0, PSTR("\r\n unknown command "));
				 lop_sendstream(&lop0, b);
				 break;
	}
	lop_dbg_str_P(&lop0, PSTR("\r\n done\r\n"));
}

/******************************************************************************/

void messagerx(uint16_t len, uint8_t * msg){
}

/******************************************************************************/

void init_system(void){
	uart_init();
	lop0.sendrawbyte = lop0_sendrawbyte;
	lop0.on_streamrx = streamrx;
	lop0.on_msgrx = messagerx;
	uart_hook = onuartrx;
    i2c_init();
    load_eeprom_crypt_key(eeprom_key);
    E24C_init();
    rtc_init();
    prng_init();
    ticketdb_init();
}

/******************************************************************************/

int main(void){
	init_system();
	
	if (!setup_done()){
		setup_system();
	}
	
	while (1){
		;
	}
	return 0;
}

/******************************************************************************/

