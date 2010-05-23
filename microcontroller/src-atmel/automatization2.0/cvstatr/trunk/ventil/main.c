/* -*- Mode: C; tab-width: 2 -*- */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
//#include <uart.h>
#include <stdlib.h>

#include "config.h"
#include "twi_slave/twi_slave.h"


#define V_OEFFNEN() PORTC |= _BV(PC0) | _BV(PC1); PORTC &= ~(_BV(PC2) | _BV(PC3))
#define V_SCHLIESSEN() PORTC |= _BV(PC2) | _BV(PC3); PORTC &= ~(_BV(PC0) | _BV(PC1))
#define FREILAUF()	PORTC &= ~(_BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3))

#define SCHRITTE 420

#define VENTIL_STEP  0
#define VENTIL_RESET 1


ISR(TIMER1_COMPA_vect) {
	FREILAUF();
	TCNT1H = 0;
	TCNT1L = 0;
	PORTD &= ~_BV(PD6);
}


void fahre_ventiel_zu() {
	uint16_t ct_last = TCNT1;
	uint16_t ct_act = 0;
	
//	PORTD |= _BV(PD6);
	V_SCHLIESSEN();
	_delay_ms(45);

	while (1) {
		ct_act = TCNT1;
		if (ct_last == ct_act)
			break;

		ct_last = TCNT1;
		_delay_ms(25);
	}
	FREILAUF();
//	PORTD &= ~_BV(PD6);
}



void init(void) {
	// Output for h-bridge
	DDRC  |= _BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3);
	// Freilauf
	FREILAUF();
	
	// CTC Modus OCR1A Reg 16 bit, ext clock rising edge
	TCCR1B |= (WGM12) | _BV(CS12) | _BV(CS11) | _BV(CS10);
	
	// Output Compare Interrupt Timer 1 OC REG A
	TIMSK |= _BV(OCF1A);

	// LED Output
	DDRD |= _BV(PD6);
	
	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	fahre_ventiel_zu();

	TWIS_Init();

	//turn on interrupts
  sei();
}

int main(void) {
	uint8_t TWIS_ResponseType;
	int8_t ist_pos = 0, ziel_pos = 0, data[2];
	
	//system initialization
	init();

	uint16_t ct_last = TCNT1;
	uint16_t ct_act = 0;
	//the main loop continuously handles can messages
	while (1) {
		//TWIS_Init();
		if (TWIS_ResponseRequired(&TWIS_ResponseType)) {
			switch (TWIS_ResponseType) {
				// Slave is requests to read bytes from the master.
				case TW_SR_SLA_ACK:
					cli();
					data[0] = TWIS_ReadAck();
					data[1] = TWIS_ReadNack();
					TWIS_Stop();
					sei();

					switch (data[0]) {
						case VENTIL_STEP:
							if (data[1] < 6)
								ziel_pos = data[1];
							OCR1A = abs(ziel_pos - ist_pos) * SCHRITTE;

							if (ist_pos < ziel_pos)
							{
								V_OEFFNEN();
								PORTD |= _BV(PD6);
								ist_pos = ziel_pos;
							}
							if (ist_pos > ziel_pos)
							{
								V_SCHLIESSEN();
								PORTD |= _BV(PD6);
								ist_pos = ziel_pos;
							}
							_delay_ms(50);
							break;
						case VENTIL_RESET:
							//TCCR2 = 0;
							wdt_enable(0);
							while (1);
							break;
					break;
				}
				case TW_ST_SLA_ACK:
					TWIS_Stop();
					break;

				default:
					TWIS_Stop();
					break;
			}
		}
	// prüfen ob motor läuft - bei ende pos speichern
		ct_act = TCNT1;
		if (ct_last == ct_act) {
			FREILAUF();
			PORTD &= ~_BV(PD6);
		}
		ct_last = TCNT1;
		_delay_ms(30);
	}

	return 1;
}

