/* -*- Mode: C; tab-width: 2 -*- */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdlib.h>

#include "config.h"
#include "twi_slave/twi_slave.h"

#define IL1 _BV(PC1)
#define IL2 _BV(PC2)
#define IH1 _BV(PC0)
#define IH2 _BV(PC3)


#define FREILAUF() PORTC &= ~(IL1 | IL2); PORTC |= IH1 | IH2

#define LED _BV(PD6)

#define SCHRITTE 420

#define VENTIL_STEP  0
#define VENTIL_RESET 1

//XXX Todo
// pos tracking
// i2c befehle
// atmega48 version?

volatile uint8_t mask = 0;
volatile int16_t pos16 = 0;
int32_t pos = 0;
volatile uint8_t bit2ms = 0;


// abschalten bei erreichen eines Wertes
ISR(TIMER1_COMPA_vect) {
	FREILAUF();
	TCNT1 = 0;
}

// Timer0 Motor PWM off
ISR(TIMER2_COMP_vect) {
  PORTC |= mask;
}

// Timer0 Motor PWM on
ISR(TIMER2_OVF_vect) {
  PORTC &= ~mask;
  bit2ms = 1;
}

// Setzt PWM für Motor
// val intger 16 bit
// positive Werte bis 255 vorwärtes
// negative Werte bis 255 rückwärts
// 0 stopp
void setPwm(int16_t val) {
  if (val > 0) {
    mask = IH1;
    PORTC |= IL2 | IH2;
    PORTC &= ~(IL1);
    PORTD |= LED;
    OCR2 = val;
  }
  else if (val < 0) {
    mask = IH2;
    PORTC |= IL1 | IH1;
    PORTC &= ~(IL2);
    PORTD |= LED;
    OCR2 = -val;
  } else {
    mask = 0;
    PORTC &= ~(IL1 | IL2);
    PORTC |= IH1 | IH2;
    PORTD &= ~(LED);
  }
}

//Positions Regler
int32_t pos_reg_soll;    // wohin will ich?
int32_t pos_soll;    //
uint8_t pos_regeln;  // soll ich da jetzt hinfahren oder was?
uint8_t ramp_state = 0;

void go_pos(int32_t soll) {
	pos_soll = soll;
	ramp_state = 1;
}

#define PWM_MAX 250
#define PWM_MIN 14

void posRegler() {
  int32_t regler_out;
  int32_t pos_abw; // Positionsabweichung

  pos_abw = pos - pos_reg_soll; // Regeldifferenz

  regler_out = pos_abw / 64; // P Anteil

  if (regler_out > PWM_MAX)
    regler_out = PWM_MAX;
  else if(regler_out < (-PWM_MAX))
    regler_out = -PWM_MAX;
    
  if (regler_out < PWM_MIN && regler_out > 0)
    regler_out = PWM_MIN;
  else if(regler_out > (-PWM_MIN)  && regler_out < 0)
    regler_out = -PWM_MIN;

  setPwm(regler_out); 
}
//

void fahre_ventiel_zu() {
	uint16_t ct_last = TCNT1;
	uint16_t ct_act = 0;
	

	while (1) {
		ct_act = TCNT1;
		if (ct_last == ct_act)
			break;

		ct_last = TCNT1;
		_delay_ms(25);
	}
	FREILAUF();
}



void init(void) {
	// Output for h-bridge
	DDRC  |= IL1 | IL2 | IH1 | IH2;
	// Freilauf
	FREILAUF();

  // Timer 1 Zähler für Position
	// CTC Modus OCR1A Reg 16 bit, ext clock rising edge
	TCCR1B |= (WGM12) | _BV(CS12) | _BV(CS11) | _BV(CS10);

  // Init Timer2 Motor PWM und Regler Zeitschlitz
  TCCR2 = _BV(CS22); // 8MHz / 64
  TIMSK |= _BV(OCF2) | _BV(TOV2); // Comparematch & Overflow
  
	// LED Output
	DDRD |= LED;
	
	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	TWIS_Init();

	//turn on interrupts
  sei();
  
  fahre_ventiel_zu();
}

int main(void) {
	uint8_t TWIS_ResponseType;
	int8_t ist_pos = 0, ziel_pos = 0, data[2];
  uint8_t tim = 0;
	//system initialization
	init();

	uint16_t ct_last = TCNT1;
	uint16_t ct_act = 0;
	//the main loop continuously handles can messages
	while (1) {
		if (TWIS_ResponseRequired(&TWIS_ResponseType)) {
			switch (TWIS_ResponseType) {
				// Slave is requests to read bytes from the master.
				case TW_SR_SLA_ACK:
					data[0] = TWIS_ReadAck();
					data[1] = TWIS_ReadNack();
					TWIS_Stop();

					switch (data[0]) {
						case VENTIL_STEP:
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
/*		cli();
    tmppos16 = pos16;
    pos16 = 0;
    sei();
    pos += tmppos16;*/

		if (bit2ms) { // 1ms Routine
		  tim++;
		  bit2ms = 0;
		  if (tim == 20) {
		    tim = 0;
		    posRegler();
		  }
/*		  if (delay == 0) {
		      toggle ^=1;
		      delay = 8000;
		      if (toggle) {
		        go_pos(418000);
          } else {
            go_pos(0);
          }
		  } else {
		    delay--;
		  }*/
		}
	}

	return 1;
}

