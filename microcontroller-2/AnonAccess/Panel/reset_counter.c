/* reset_counter.c */
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




