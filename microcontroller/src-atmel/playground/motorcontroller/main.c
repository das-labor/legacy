/* -*- Mode: C; tab-width: 2 -*- */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <stdlib.h>

#include "config.h"
#include "twi_slave/twi_slave.h"

// XXX TODO
// temeratur messen?

// Pins for h-bridge
#define IL1 _BV(PC3)
#define IL2 _BV(PC0)
#define IH1 _BV(PC2)
#define IH2 _BV(PC1)

// simple h-bridge operations
#define HOCH()      PORTC &= ~(IL2 | IH1); PORTC |= IL1 | IH2
#define RUNTER()    PORTC &= ~(IL1 | IH2); PORTC |= IL2 | IH1
#define FREILAUF()  PORTC &= ~(IH1 | IH2 | IL1 | IL2);

// LED pins
#define GRUEN _BV(PD7)
#define ROT _BV(PD6)

// I2C / TWI Befehle
#define CMD_RUNTER 0
#define CMD_HOCH	 1
#define CMD_STOP   2
#define CMD_RESET  3

// Beispiel EEPROM Werte
const uint8_t EE_addr EEMEM = 42; // Adresse
const uint16_t EE_endpos EEMEM = 1010; // End Position

//const uint8_t EE_addr EEMEM = 44;
//const uint16_t EE_endpos EEMEM = 1159;

// Globale Variablen
volatile uint8_t mask = 0;  // h-bridge pin 
volatile int16_t pos16 = 0; // 16Bit zaeher im Interrupt
int32_t pos = 0;            // 32Bit Zaehler der Position
volatile uint8_t tick = 0; // Tick

// INT 0 count position
ISR(INT0_vect) {
//  if (PIND & _BV(PD3)) // read direction from quadrature encoder
  if (mask == IH2)
    pos16++;
  else
    pos16--;
}

// Timer0 PWM off IH1
ISR(TIMER0_COMPA_vect) {
  PORTC &= ~IH1;
}

// Timer0 PWM off IH2
ISR(TIMER0_COMPB_vect) {
  PORTC &= ~IH2;
}

// Timer 0 PWM on
ISR(TIMER0_OVF_vect) {
  PORTC |= mask;
  tick = 1;
}

// 0 Position Anfahren, erkennung durch Reflexlichtschranke
void init_pos0(void);

void init(void) {
	// Output for h-bridge
	DDRC  |= IL1 | IL2 | IH1 | IH2;
	// H-Bruecke aus
	FREILAUF();

	// LED Output
	DDRD |= ROT | GRUEN;
	
	ACSR = _BV(ACD);// Disable Analog Comparator (power save)

  // init i2c
	TWIS_Init();

	// start animation
	PORTD |= ROT;
	_delay_ms(100);
	PORTD |= GRUEN;
	_delay_ms(100);
	PORTD &= ~ROT;
	_delay_ms(100);
	PORTD &= ~GRUEN;

	// Timer 0 fuer soft pwm
	TCCR0A = 0;
	TCCR0B = _BV(CS01) | _BV(CS00); // 8mhz clk/64
	TCNT0 = 0;
	TIMSK0 = _BV(TOIE0);
  
	//turn on interrupts
  sei();
  
  // Fahre zu null pos Marker an Rollo (Reflexlichtschranke)
  init_pos0();
  
  // External Interrupt 0 fuer pos bestimmung
  EIMSK = _BV(INT0);
  EICRA = _BV(ISC01);
}

// Setzt PWM fuer Motor
// val intger 16 bit
// positive Werte bis 255 vorwaertes
// negative Werte bis 255 rückwaerts
// 0 stopp
void setPwm(int16_t val) {
  if (val > 0) {
    mask = IH1;
    PORTC |= IL2;
    PORTC &= ~(IL1|IH2);
    PORTD |= ROT;
		PORTD &= ~GRUEN;
	  TIMSK0 = _BV(OCIE0A) | _BV(TOIE0);
    OCR0A = val;
  }
  else if (val < 0) {
    mask = IH2;
    PORTC |= IL1;
    PORTC &= ~(IL2|IH1);
    PORTD &= ~ROT;
		PORTD |= GRUEN;
	  TIMSK0 = _BV(OCIE0B) | _BV(TOIE0);
    OCR0B = -val;
  } else {
    mask = 0;
    //TIMSK0 = 0;
    PORTC &= ~(IH1|IH2|IL1|IL2);
    PORTD &= ~(ROT | GRUEN);
  }
}

// Positions Regler 
int32_t pos_reg_soll; // Zielwert
int32_t pos_soll;     // Akutelles Regler Soll das durch Rampe vorgegeben wird
uint8_t ramp_state = 0; // Status der Rampe

// Setzt Ziel Position und startet Rampe
void go_pos(int32_t soll) {
	pos_soll = soll;
	ramp_state = 1;
}

#define PWM_MAX 250 // Maximale Geschwindigkeit
#define PWM_MIN 14  // Minimale Geschwindigkeit (sorgt für noetiges drehmoment)

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
// States der Rampe

#define RMP_START 1
#define RMP_RUN 2
#define RMP_END 3

#define ACCEL 14

void rampe() {
  static int32_t v;

  switch (ramp_state) {
    case RMP_START:
      v += ACCEL;
      if (v >= 5000)
        ramp_state = 2;
      break;
    case RMP_RUN:
      break;
    case RMP_END:
      v -= ACCEL;
      if (v <= 0) {
        v = 0;
        ramp_state = 0;
      }
      break;
      
    default:
      break;
  }
  int32_t rest_strecke;
  if (pos > pos_soll) {
    pos_reg_soll -= v;
    rest_strecke = -1 * (pos_soll - pos_reg_soll);
   }
  else {
    pos_reg_soll += v;
    rest_strecke = pos_soll - pos_reg_soll;
  }

  if ((ramp_state == RMP_START) || (ramp_state == RMP_RUN)) {
    if ((v * v / ACCEL/2) >= rest_strecke) {
      ramp_state = RMP_END;
    }
  }
}

// 0 Position Anfahren, erkennung durch Reflexlichtschranke
void init_pos0(void) {
  setPwm(60);
  while (PIND & _BV(PD5));
  setPwm(0);
  _delay_ms(1000);
  pos = 0;
}

int main(void) {
	uint8_t TWIS_ResponseType;
	uint8_t data[2];
  int16_t tmppos16;
  int32_t end_pos = (int32_t) eeprom_read_word((uint16_t*)1) * 1000; // Unterer Endpunkt / Maximale Position aus dem EEPROM lesen
  uint8_t tim = 0;
	//system initialization
	init();
  /* //test code
  uint16_t delay = 0;
  uint8_t toggle = 0;*/

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
						case CMD_RUNTER:
						    go_pos(end_pos);
							break;
						case CMD_HOCH:
              go_pos(0);          
							break;
						case CMD_STOP:
              pos_soll = pos;
							break;
						case CMD_RESET:
							wdt_enable(0);
							while (1);
							break;
					}
					break;
				case TW_ST_SLA_ACK:
					TWIS_Stop();
					break;

				default:
					TWIS_Stop();
					break;
			}
		}
		
    cli();
    tmppos16 = pos16;
    pos16 = 0;
    sei();
    pos += tmppos16;

		if (tick) { // 1ms Routine
		  tim++;
		  tick = 0;
		  if (tim == 10)
		    rampe();
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

