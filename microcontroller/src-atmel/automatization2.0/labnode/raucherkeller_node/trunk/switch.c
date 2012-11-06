#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "switch.h"

uint8_t sreg;
uint8_t stat_licht = 0;

#define HOLD_THRESHOLD 18
#define CLICK_THRESHOLD 0

static volatile uint16_t time_cnt;

//simple system timer
ISR(TIMER0_OVF_vect) {
	time_cnt++;
}

void init_switch() {
	TCCR0 = 0; //stop counting
	TCNT0 = 0;
	TIMSK |= _BV(TOIE0);	//Overflow Interrupt enable
	OCR0 = 0;
	TCCR0 = _BV(CS02) | _BV(CS00);	//1:1024 prescaler
	time_cnt = 0;
}

/* channel -> switch port/pin map
 * 0 -> ??
 * 1 -> ??
 */
void switch_handler() {
	/*static uint16_t last_tickscounter;

	cli();
	uint16_t tc = time_cnt;
	sei();*/
	
	//implement switch handling
}

