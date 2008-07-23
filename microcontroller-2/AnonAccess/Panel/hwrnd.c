/* hwrnd.c */
/*
 *   This file is part of AnonAccess, an access system which can be used
 *    to open door or doing other things with an anonymity featured
 *    account managment.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include <avr/interrupt.h>

#include "config.h"
#include "entropium.h"
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
//	DDRA &= ~_BV(HWRND_PIN);
//	PORTA &= ~_BV(HWRND_PIN);
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
		entropium_addEntropy(64*8,&b);
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
			entropium_addEntropy(72*8,(void*)buffer);
		}
		busy &= ~2;
	}
}

/******************************************************************************/

