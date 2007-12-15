/**
 * a simple counter for resets
 * Author: Daniel Otte
 * License: GPLv3
 * 
 */
#include <stdint.h>
#include "config.h"
#include "reset_counter.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

uint8_t  EEMEM resetcnt_backup_idx_ee=0;
uint64_t EEMEM resetcnt_backup[20]={0}; /* enough for about 200 years */

uint64_t resetcnt_read(void){
	uint8_t idx;
	uint64_t ret;
	idx = eeprom_read_byte(&resetcnt_backup_idx_ee);
	eeprom_read_block(&ret,&(resetcnt_backup[idx]),8);
	return ret;
}

void resetcnt_inc(void){
	uint8_t idx;
	uint64_t val,val2;
	idx = eeprom_read_byte(&resetcnt_backup_idx_ee);
	eeprom_read_block(&val,&(resetcnt_backup[idx]),8);
	val++;
	eeprom_write_block(&val,&(resetcnt_backup[idx]),8);
	eeprom_read_block(&val2,&(resetcnt_backup[idx]),8);
	while((val!=val2)&&(idx<20)){
		idx++;
		eeprom_write_byte(&resetcnt_backup_idx_ee,idx);
		eeprom_write_block(&val,&(resetcnt_backup[idx]),8);
		eeprom_read_block(&val2,&(resetcnt_backup[idx]),8);
	}
}




