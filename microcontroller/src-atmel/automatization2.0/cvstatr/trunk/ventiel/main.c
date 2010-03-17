/* -*- Mode: C; tab-width: 2 -*- */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <stdlib.h>

#include "config.h"
#include "twi_slave/twi_slave.h"


#define V_OEFFNEN() PORTC |= _BV(PC0) | _BV(PC1); PORTC &= ~(_BV(PC2) | _BV(PC3))
#define V_SCHLIESSEN() PORTC |= _BV(PC2) | _BV(PC3); PORTC &= ~(_BV(PC0) | _BV(PC1))
#define FREILAUF()	PORTC &= ~(_BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3))

#define SCHRITTE 420


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
	
	// CTC Modus OCR1A Reg 16 bit, ext clock rising edge
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
	uint8_t TWIS_ResponseType;
	uint8_t ist_pos = 0, ziel_pos;
	
	//system initialization
	init();

	TWIS_Init();

	//V_OEFFNEN();

	//the main loop continuously handles can messages
	while (1)
	{
		if (TWIS_ResponseRequired(&TWIS_ResponseType))
		{
			switch (TWIS_ResponseType)
			{
				// Slave is requests to read bytes from the master. 
				case TW_SR_SLA_ACK:
					ziel_pos = TWIS_ReadNack();
					TWIS_Stop();
					
					OCR1A = abs(ist_pos - ziel_pos) * SCHRITTE;
					
					if (ist_pos < ziel_pos)
						V_OEFFNEN();
					if (ist_pos > ziel_pos)
						V_SCHLIESSEN();
						
					break;

				case TW_ST_SLA_ACK:
					TWIS_Stop();
					break;

				default:
					TWIS_Stop();
					break;
			}
		}
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

