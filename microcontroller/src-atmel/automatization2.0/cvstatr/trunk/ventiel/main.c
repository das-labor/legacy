/* -*- Mode: C; tab-width: 2 -*- */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//#include "config.h"

#define V_OEFFNEN() PORTC |= _BV(PC0) | _BV(PC1); PORTC &= ~(_BV(PC2) | _BV(PC3))
#define V_SCHLIESSEN() PORTC |= _BV(PC2) | _BV(PC3); PORTC &= ~(_BV(PC0) | _BV(PC1))
#define MOTORBREMSE() PORTC |= _BV(PC1) | _BV(PC2); PORTC &= ~(_BV(PC0) | _BV(PC3))
#define FREILAUF()	PORTC &= ~(_BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3))



ISR(TIMER1_COMPA_vect)
{
	FREILAUF();
}


void fahre_ventiel_zu() {
	uint8_t ct_last = TCNT1L;
	uint8_t ct_act = 0;

	V_SCHLIESSEN();
	_delay_ms(50);

	while (1) {
		ct_act = TCNT1L;
		if (ct_last == ct_act)
			break;
		
		ct_last = TCNT1L;
		_delay_ms(25);
	}
	FREILAUF();
}



void init(void)
{
	// Output for h-bridge
	DDRC  |= _BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3);
	// Freilauf
	FREILAUF();
	
	// CTC Modus 
	TCCR1B |= (WGM12) | _BV(CS12) | _BV(CS11) | _BV(CS10);
	
	// Output Compare Interrupt Timer 1 OC REG A
	TIMSK |= _BV(OCF1A);
	
	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	fahre_ventiel_zu();

	OCR1AH = 0x00;
	OCR1AL = 0xff;

	//turn on interrupts
  sei();
}
	 
int main(void)
{
	//system initialization
	init();

	//V_OEFFNEN();

	//the main loop continuously handles can messages
	while (1)
	{
		
/*		FREILAUF();
		V_OEFFNEN();
		_delay_ms(5000);
		BREAK();
		_delay_ms(500);
		FREE();
		V_CLOSE();
		_delay_ms(5000);
		BREAK();
		_delay_ms(500);*/
	}
	
	return 1;
}

