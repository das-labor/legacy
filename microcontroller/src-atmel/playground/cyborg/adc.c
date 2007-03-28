
#include <avrx.h>
#include <avr/io.h>

volatile uint8_t adc_value[8];

void adc_init(){
	ADMUX = 0x00 | (1<<ADLAR);
	
	//ADPS2 ADPS1 ADPS0 Division Factor
	//  0     0     0           2
	//  0     0     1           2
	//  0     1     0           4
	//  0     1     1           8
	//  1     0     0          16

	ADCSRA = (1<<ADEN) | (1<<ADIE) | 4;
	ADCSRA |= (1<<ADSC);
	
}

AVRX_SIGINT(SIG_ADC){
	static uint8_t mux = 0;
	IntProlog();
	adc_value[mux] = ADCH;
	mux++;
	if(mux == 8){
		mux = 0;
	}
	ADMUX = mux | (1<<ADLAR);
	ADCSRA |= (1<<ADSC);
	Epilog();
}
