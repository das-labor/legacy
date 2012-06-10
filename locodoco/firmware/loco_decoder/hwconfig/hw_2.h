#include <avr/io.h>
#include <avr/interrupt.h>

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
#define M_TIMER_INIT(a) \
		TCCR1B = (_BV(CS11)); /* clk / 8 */ \
		TCCR1A = (_BV(WGM11) | _BV(WGM10) | _BV(COM1B1))

#define M_TIMER_OFF(a) \
		TCCR1B = 0x00
	
#define M_CHOP(a) \
		M_EN_PORT &= ~(M_EN_PIN); \
		M_RW_PORT ^= M_RW_PIN; \
		M_FW_PORT ^= M_FW_PIN; \
		M_EN_PORT |= (M_EN_PIN)

#define M_OCR_MAX 0x3ff

#define DDR_LIGHT_CABIN   DDRC
#define DDR_LIGHT_FRONT   DDRC
#define DDR_LIGHT_BACK    DDRC

#define PORT_LIGHT_CABIN  PORTC
#define PORT_LIGHT_BACK   PORTC
#define PORT_LIGHT_FRONT  PORTC

#define PIN_LIGHT_BACK    (_BV(PC3))
#define PIN_LIGHT_FRONT   (_BV(PC2))
#define PIN_LIGHT_CABIN   (_BV(PC5))

#define LIGHTS_INIT(a) \
	DDR_LIGHT_CABIN |= PIN_LIGHT_CABIN; \
	DDR_LIGHT_FRONT |= PIN_LIGHT_FRONT; \
	DDR_LIGHT_BACK  |= PIN_LIGHT_BACK

#define LIGHT_FW_ON(a) \
	PORT_LIGHT_FRONT |= PIN_LIGHT_FRONT

#define LIGHT_FW_OFF(a) \
	PORT_LIGHT_FRONT &= ~(PIN_LIGHT_FRONT)

#define LIGHT_RW_ON(a) \
	PORT_LIGHT_BACK |= PIN_LIGHT_BACK

#define LIGHT_RW_OFF(a) \
	PORT_LIGHT_BACK &= ~(PIN_LIGHT_BACK)

#define LIGHT_CABIN_ON(a) \
	PORT_LIGHT_CABIN |= (PIN_LIGHT_CABIN)

#define LIGHT_CABIN_OFF(a) \
	PORT_LIGHT_CABIN &= ~(PIN_LIGHT_CABIN)

/* sense dec feature */
#define FEATURE_SENSE_DEC 1 
#define SENSE_DEC_DDR  DDRC
#define SENSE_DEC_PORT PORTC
#define SENSE_DEC_PIN  (_BV(PC1))

#if FEATURE_SENSE_DEC
	#define SENSE_DEC(a) \
		SENSE_DEC_DDR  &= ~(SENSE_DEC_PIN); \
		SENSE_DEC_PORT &= ~(SENSE_DEC_PIN)
	#define SENSE_INC(a) \
		SENSE_DEC_DDR  |=  (SENSE_DEC_PIN); \
		SENSE_DEC_PORT &= ~(SENSE_DEC_PIN)
#else
	#define SENSE_DEC(a)
	#define SENSE_INC(a)
#endif

/* speed measurement feature */
#define FEATURE_SPEED_SENSE 1
#define SENSE_SPEED_SETUP(a) \
	ADMUX = (0x07 | _BV(REFS0) | _BV(REFS1)); /* 1.1V, ADC7 */ \
	ADCSRB = (_BV(ADTS0) | _BV(ADTS2)); /* start on ocr1b match (midist of off cycle) */ \
	ADCSRA = (_BV(ADATE) | _BV(ADEN) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2) \
		| _BV(ADIE) | _BV(ADSC)) /* int, start, auto trigger (ocr1b), /128 */

/* rfm12 config */
#define DDR_SS DDRB
#define PORT_SS PORTB
#define BIT_SS 1

//SPI port
#define DDR_SPI DDRB
#define PORT_SPI PORTB
#define PIN_SPI PINB
#define BIT_MOSI 3
#define BIT_MISO 4
#define BIT_SCK  5
#define BIT_SPI_SS 2

#define RFM12_BASEBAND RFM12_BAND_433

//center frequency to use (+-125kHz FSK frequency shift)
#define FREQ 433000000UL

//use this for datarates >= 2700 Baud
#define DATARATE_VALUE RFM12_DATARATE_CALC_HIGH(9600.0)

//use this for 340 Baud < datarate < 2700 Baud
//#define DATARATE_VALUE RFM12_DATARATE_CALC_LOW(1200.0)

//TX BUFFER SIZE
#define RFM12_TX_BUFFER_SIZE 32

//RX BUFFER SIZE (there are going to be 2 Buffers of this size for double_buffering)
#define RFM12_RX_BUFFER_SIZE 32


/************************
 * INTERRUPT VECTOR
 * set the name for the interrupt vector here
 */
 

//the interrupt vector
#define RFM12_INT_VECT (INT1_vect)

//the interrupt mask register
#define RFM12_INT_MSK EIMSK

//the interrupt bit in the mask register
#define RFM12_INT_BIT (INT1)

//the interrupt flag register
#define RFM12_INT_FLAG EIFR

//the interrupt bit in the flag register
#define RFM12_FLAG_BIT (INTF1)

//setup the interrupt to trigger on negative edge
#define RFM12_INT_SETUP() EICRA = (1<<ISC11) 


/************************
 * FEATURE CONFIGURATION
 */

#define RFM12_LIVECTRL 0
#define RFM12_NORETURNS 0
#define RFM12_NOCOLLISIONDETECTION 0
#define RFM12_TRANSMIT_ONLY 0
#define RFM12_SPI_SOFTWARE 0
#define RFM12_USE_POLLING 0
#define RFM12_RECEIVE_ASK 0
#define RFM12_TRANSMIT_ASK 0
#define RFM12_USE_WAKEUP_TIMER 0
#define RFM12_LOW_POWER 0


/************************
 * UART DEBUGGING
 * en- or disable debugging via uart.
 */
 
#define RFM12_UART_DEBUG 0
