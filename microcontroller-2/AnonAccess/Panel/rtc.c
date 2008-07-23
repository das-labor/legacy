/* rtc.c */
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
 * a simple rtc implementation to make timestamps
 * Author: Daniel Otte
 * License: GPLv3
 * 
 */
#include <stdint.h>
#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "rtc.h"

timestamp_t milliseconds=0;
uint8_t EEMEM timer_backup_idx_ee=0;
uint8_t timer_backup_idx=0;
timestamp_t EEMEM timer_backup[20]={0}; /* enough for about 200 years */

#ifdef ATMEGA644
void rtc_init(void){
	TCCR0A = 0x02;             /* CTC Mode */
	TCCR0B = 0x03;             /* CTC Mode, clk/64 */
	OCR0A = F_CPU/64/1000/4;   /* 1000Hz */
	TIMSK0 |= _BV(OCIE0A);     /* enable output compare match interrupt */
	timer_backup_idx = eeprom_read_byte(&timer_backup_idx_ee);
	eeprom_read_block(&milliseconds, &(timer_backup[timer_backup_idx]), sizeof(timestamp_t));
	milliseconds += 0x3FFFFFLL;
	sei();
}
#else
void rtc_init(void){
	TCCR0 = 0x0C;             /* CTC Mode, clk/64 */
	OCR0 = F_CPU/64/1000/4;   /* 1000Hz */
	TIMSK |= _BV(OCIE0);     /* enable output compare match interrupt */
	timer_backup_idx = eeprom_read_byte(&timer_backup_idx_ee);
	eeprom_read_block(&milliseconds, &(timer_backup[timer_backup_idx]), sizeof(timestamp_t));
//	milliseconds += 1000LL * 60LL * 60LL;	/* add one hour */

	sei();
}
#endif


timestamp_t gettimestamp(void){
	return milliseconds;
}

#ifdef ATMEGA644
ISR(TIMER0_COMPA_vect){
#else
ISR(TIMER0_COMP_vect){
#endif
	++milliseconds;
//	if(milliseconds%(1000LL*60LL*60LL)==0){ /* every hour */
	if((milliseconds & 0x3FFFFFLL)==0){ /* ~1,16 hours */
		 uint64_t t;
		 eeprom_write_block(&milliseconds, &(timer_backup[timer_backup_idx]), sizeof(timestamp_t));
		 eeprom_read_block(&t, &(timer_backup[timer_backup_idx]), sizeof(timestamp_t));
		 if(t != milliseconds){ /* this is only going to happen all 11.4 years */
		 	++timer_backup_idx;
		 	eeprom_write_byte(&timer_backup_idx_ee, timer_backup_idx);
		 	eeprom_write_block(&milliseconds, &(timer_backup[timer_backup_idx]), sizeof(timestamp_t));
		 }
	}
}


timestamp_t abstimedifference(timestamp_t t1, timestamp_t t2){
	if(t1<t2){
		return t2-t1;
	} else {
		return t1-t2;
	}
}





