/* selfdestruct.c */
/*
 *   This file is part of AnonAccess, an access system which can be used
 *    to open door or doing other things with an anonymity featured
 *    account managment.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/**
 * 
 * Author: Daniel Otte
 * License: GPLv3
 *
 * see hwrnd.c for the parts which overwrite the eeprom
 *  
 */
 
#include "config.h"
#include "selfdestruct.h"
#include "lop.h"
#include "lop_debug.h"
#include <stdint.h>
#include <avr/interrupt.h>


extern lop_ctx_t lop0;

uint8_t tamperdetect=0;

void tamperdetect_init(void){
	TAMPER_DDR &= ~_BV(TAMPER_PIN);
	TAMPER_PORT |= _BV(TAMPER_PIN);
	TAMPER_PCMSK |= _BV(TAMPER_PIN);
	PCICR |= _BV(TAMPER_SECT);
}


ISR(TAMPER_PCINT){
	if(!tamperdetect){
//		lop_dbg_str_P(&lop0, PSTR("\r\nTAMPER-DETECT!!!"));
		tamperdetect=1;
//		lop_dbg_str_P(&lop0, PSTR("\r\nTAMPER (1)"));
		while(tamperdetect==1)
			;	/* wait until the ADC_INT starts working */
//		lop_dbg_str_P(&lop0, PSTR("\r\nTAMPER (2)"));
		while(tamperdetect&2)
			;	/* wait until the ADC_INT starts working */
		/* overwrite SRAM */	
		/* overwrite external EEPROM */	
//		lop_dbg_str_P(&lop0, PSTR("\r\nTAMPER-DETEC finished"));
	}
}

