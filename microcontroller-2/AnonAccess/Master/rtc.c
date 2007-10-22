/*
 * a simple rtc implementation to make timestamps
 * 
 * 
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
 #define TCCR0 TCCR0A
 #define OCR0  OCR0A 
 #define TIMSK TIMSK0
 #define OCIE0 OCIE0A
#endif

void rtc_init(void){
	TCCR0 = 0x0C;             /* CTC Mode, clk/64 */
	OCR0 = F_CPU/64/1000/4;   /* 1000Hz */
	TIMSK |= _BV(OCIE0);     /* enable output compare match interrupt */
	timer_backup_idx = eeprom_read_byte(&timer_backup_idx_ee);
	eeprom_read_block(&milliseconds, &(timer_backup[timer_backup_idx]), sizeof(timestamp_t));
	milliseconds += 1000LL * 60LL * 60LL;	/* add one hour */
	sei();
}

timestamp_t gettimestamp(void){
	return milliseconds;
}
    
ISR(TIMER0_COMP_vect){
	++milliseconds;
	if(milliseconds%(1000LL*60LL*60LL)==0){ /* every hour */
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
