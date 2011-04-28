
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"

#define ADC_CHANNEL_UWAVE 7

volatile uint16_t adc_uwave;


ISR(ADC_vect){
	static uint8_t state;
	if(state == 0){
		state = 0;
		adc_uwave = ADC;
		//select 2.56V reference and u channel again for next conversion
		ADMUX = (1<<REFS1) | (1<<REFS0) | ADC_CHANNEL_UWAVE;
	}else{
		/*
		state = 0;
		adc_i = ADC;
		//select AVCC reference and i channel again for next conversion (u conversion running allready)
		ADMUX = (1<<REFS1) | (1<<REFS0) | ADC_CHANNEL_I;
		*/
	}
}

void init_adc(){
	//REFS1 REFS0 ADLAR MUX4 MUX3 MUX2 MUX1 MUX0      ADMUX
	//REFS1 REFS0 Voltage Reference Selection
	//0 0 AREF, Internal Vref turned off
	//0 1 AVCC with external capacitor at AREF pin
	//1 0 Reserved
	//1 1 Internal 2.56V Voltage Reference with external capacitor at AREF pin

	ADMUX = (1<<REFS1) | (1<<REFS0);

	//ADEN ADSC ADFR ADIF ADIE ADPS2 ADPS1 ADPS0      ADCSRA
	//ADPS2 ADPS1 ADPS0 Division Factor
	//0 0 0 2
	//0 0 1 2
	//0 1 0 4
	//0 1 1 8
	//1 0 0 16
	//1 0 1 32
	//1 1 0 64
	//1 1 1 128
	ADCSRA = (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIE) | 7;
	
}
