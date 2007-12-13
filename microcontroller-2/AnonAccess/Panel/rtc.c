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
void rtc_init(void){
	TCCR0A = 0x02;             /* CTC Mode */
	TCCR0B = 0x03;             /* CTC Mode, clk/64 */
	OCR0A = F_CPU/64/1000/4;   /* 1000Hz */
	TIMSK0 |= _BV(OCIE0A);     /* enable output compare match interrupt */
	timer_backup_idx = eeprom_read_byte(&timer_backup_idx_ee);
	eeprom_read_block(&milliseconds, &(timer_backup[timer_backup_idx]), sizeof(timestamp_t));
	milliseconds += 1000LL * 60LL * 60LL;	/* add one hour */
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
	milliseconds += 0x3FFFFFLL;
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







