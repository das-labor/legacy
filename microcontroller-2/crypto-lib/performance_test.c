/*
 * author: Daniel Otte
 * email:  daniel.otte@rub.de
 * license: GPLv3
 * 
 * 
 **/
 
#include "config.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "uart.h"
#include "performance_test.h"


#ifdef ATMEGA644
	#define TIMSK TIMSK1
#endif



uint32_t ovfcounter;

uint16_t const_overhead=0;
uint16_t int_overhead=0;

ISR(TIMER1_OVF_vect){
	ovfcounter++;
}

void calibrateTimer(void){
	startTimer(1);
	stopTimer();
	const_overhead = TCNT1;
	startTimer(1);
	TCNT1=0xFFFE;
	; ; ; ;
//	asm volatile("NOP\n"::); asm volatile("NOP\n"::);
	stopTimer();
	int_overhead = TCNT1;
}

void startTimer(uint8_t granularity){
	TCCR1B = 0; /* stop timer */
	TCNT1  = 0;
	ovfcounter = 0;
	TCCR1A = 0x00;
	TIMSK &= 0xC3;
	TIMSK |= _BV(TOIE1); /* enable TOIE1 */
	TCCR1B = granularity & 0x7;	/* start timer */
}

uint64_t stopTimer(void){
	TCCR1B = 0; /* stop timer */
	uint64_t ret;
	ret = (ovfcounter<<16) | TCNT1;
	ret -= const_overhead;
	ret -= ovfcounter * int_overhead;
	return ret;
}

void getOverhead(uint16_t* constoh, uint16_t* intoh){
	*constoh = const_overhead;
	*intoh   = int_overhead; 
}

void print_time_P(PGM_P s, uint64_t t){
	char sv[16];
	uint8_t c;
	uart_putstr_P(PSTR("\r\n"));
	uart_putstr_P(s);
	ultoa((unsigned long)t, sv, 10);
	for(c=strlen(sv); c<11; ++c){
		uart_putc(' ');
	}
	uart_putstr(sv);
}

void print_overhead(void){
	char str[16];
	uart_putstr_P(PSTR("\r\n\r\n=== benchmark ==="));
	utoa(const_overhead, str, 10);
	uart_putstr_P(PSTR("\r\n\tconst overhead:     "));
	uart_putstr(str);
	utoa(int_overhead, str, 10);
	uart_putstr_P(PSTR("\r\n\tinterrupt overhead: "));
	uart_putstr(str);
}


