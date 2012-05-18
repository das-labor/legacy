#include <avr/io.h>

#pragma once

#define M_EN_DDR  DDRB
#define M_EN_PORT PORTB
#define M_EN_PIN  (_BV(PB2))

#define M_FW_DDR  DDRD
#define M_FW_PORT PORTD
#define M_FW_PIN  (_BV(PD7))

#define M_RW_DDR  DDRB
#define M_RW_PORT PORTB
#define M_RW_PIN  (_BV(PB0))

#define M_OCR     OCR1B
#define M_TIMER_INIT() TCCR1B = (_BV(CS10) | _BV(CS11)); \
		TCCR1A = (_BV(WGM11) | _BV(WGM10) | _BV(COM1B1))

#define M_OCR_MAX 0x3ff

/* sense dec feature */
#define FEATURE_SENSE_DEC 1 
#define SENSE_DEC_DDR  DDRC
#define SENSE_DEC_PORT PORTC
#define SENSE_DEC_PIN  (_BV(PC1))

/* speed measurement feature */
#define FEATURE_SPEED_SENSE 1
#define SENSE_SPEED_SETUP() ADMUX = (_BV(REFS0) | _BV(REFS1) | _BV(ADC7)); \
	ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2) | _BV(ADIE))

