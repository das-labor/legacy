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

