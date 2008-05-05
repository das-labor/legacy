
#include <avr/interrupt.h>

#include "config.h"
#include "prng.h"
#include "rtc.h"

#include "uart.h"

volatile uint8_t busy=0;

/******************************************************************************/

uint8_t getbadrandom(void){
	ADCSRA |= 0x40 | 0x80; /* start conversion */
	while(ADCSRA & 0x40)   /* wait for conversation to complete */
		;
	return (uint8_t)ADC;
}

/******************************************************************************/

void prng_init(void){
	/* here we should add some entropy to the prng */
	DDRA &= ~_BV(HWRND_PIN);
	PORTA &= ~_BV(HWRND_PIN);
	ADMUX = 0x40 | HWRND_PIN;  /* Vref=Avcc, ADC0 */
	ADCSRA = 0x83; /* turn ADC on, prescaler=8 */
#ifdef ATMEGA644
	DIDR0 =  0x3F;  /* turn off digital input for AD0 */
#endif	

	uint8_t i,j;
	uint8_t b[64];
	for(i=0; i<32; ++i){
		for(j=0; j<64; ++j){
			b[j] = getbadrandom();
		}
		addEntropy(64*8,&b);
	}
//	ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADATE) | _BV(ADIE) | 7; /* turn ADC on, start ADC, enable interrupt prescaler=128 */
#ifdef ATMEGA644	
	ADCSRB = 0; /* freerunning mode*/
#endif
}

/******************************************************************************/

ISR(ADC_vect)
{
	static uint8_t buffer[72];
	static uint8_t idx=0;	
	if (!busy){
		busy |=2;
		buffer[idx++] = (uint8_t)ADC;
		if(idx==64){
			*((uint64_t*)(buffer+72-8)) = gettimestamp();		
		//	uart_putstr("\r\nadc ");
		//	uart_hexdump(buffer, 10);
			idx=0;
			addEntropy(72*8,(void*)buffer);
		}
		busy &= ~2;
	}
}

/******************************************************************************/

