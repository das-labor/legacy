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

timestamp_t milliseconds;
timestamp_t EEMEM timer_backup=0;

void rtc_init(void){
	TCCR0A = 0x0C;             /* CTC Mode, clk/64 */
	OCR0A = F_CPU/64/1000/4;   /* 1000Hz */
	TIMSK0 |= _BV(OCIE0A);     /* enable output compare match interrupt */
	eeprom_read_block(&milliseconds, &timer_backup, sizeof(timestamp_t));
	milliseconds += 1000LL * 60LL * 60LL;	/* add one hour */
}

timestamp_t gettimestamp(void){
	return milliseconds;
}

ISR(TIMER0_COMPA_vect){
	++milliseconds;
	if(milliseconds%(1000LL*60LL*60LL)){ /* every hour */
		 eeprom_write_block(&milliseconds, &timer_backup, sizeof(timestamp_t));
	}
}
