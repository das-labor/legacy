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
// i2c befehle
// atmega48 version?

volatile uint8_t mask = 0;
int32_t pos = 0;
volatile uint8_t bit2ms = 0;


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

void go_pos(int32_t soll) {
	pos_reg_soll = soll;
}

#define PWM_MAX 254
#define PWM_MIN 60

void posRegler() {
  int32_t regler_out;
  int32_t pos_abw; // Positionsabweichung

  pos_abw = pos - pos_reg_soll; // Regeldifferenz

  regler_out = pos_abw * 2; // P Anteil

  if (regler_out > PWM_MAX)
    regler_out = PWM_MAX;
  else if(regler_out < (-PWM_MAX))
    regler_out = -PWM_MAX;
    
  if (regler_out < PWM_MIN && regler_out > 0)
    regler_out = PWM_MIN;
  else if(regler_out > (-PWM_MIN) && regler_out < 0)
    regler_out = -PWM_MIN;

  setPwm(regler_out); 
}
//

void fahre_ventiel_zu() {
	uint16_t ct_last = TCNT1;
	uint16_t ct_act = 0;
	
  setPwm(150);
  _delay_ms(100);
	while (1) {
		ct_act = TCNT1;
		if (ct_last == ct_act)
			break;

		ct_last = TCNT1;
		_delay_ms(40);
	}
	setPwm(0);
  _delay_ms(1000);
	TCNT1 = 0;
	pos = 0;
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

//	TWIS_Init();

	//turn on interrupts
  sei();
  
  fahre_ventiel_zu();
}

int main(void) {
	uint8_t TWIS_ResponseType;
	int8_t data[2];
  uint8_t tim = 0;
	//system initialization
	init();

  uint16_t delay = 0;
  uint8_t toggle = 0;
	
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


		if (bit2ms) { // 1ms Routine
	    if (mask == IH1)
        pos -= TCNT1;
      else if (mask == IH2)
        pos += TCNT1;
      TCNT1 = 0;

		  tim++;
		  bit2ms = 0;
		  if (tim == 10) {
		    tim = 0;
		    posRegler();
		  }
		  if (delay == 0) {
		      toggle ^=1;
		      delay = 6000;
		      if (toggle) {
		        go_pos(300);
          } else {
            go_pos(0);
          }
		  } else {
		    delay--;
		  }
		}
	}

	return 1;
}

