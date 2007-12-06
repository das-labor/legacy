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

uint8_t setupdone_flag EEMEM = 0;

bool setup_done(void){
	return eeprom_read_byte(&setupdone_flag);
}

void setup_system(void){
}

void init(void){
    i2c_init();
    E24C_init();
    rtc_init();
    prng_init();
    ticketdb_init();
}

int main(){
	init_system();
	
	if (!setup_done()){
		setup_system();
	}
	
	while (1){
		action = getActionReq();
		if (validAction(action)){
			giveNewTokens();
			doAction(action);
		} else {
			doNotActio(action);
		}
	}
	return 0;
}


