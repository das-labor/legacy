
#include <avr/io.h>
#include "random.h"

uint8_t get_random_byte(){
	uint8_t x, v;
	ADMUX = (1<<REFS0); //AVCC Reference, Channel 0
	
	ADCSRA = (1<<ADEN); //enable ADC
	
	for(x=0, v=0; x<4; x++){
		ADCSRA = (1<<ADEN) | (1<<ADIF) | (1<<ADSC) ;//clear interrupt, start conversion
		while( (ADCSRA & (1<<ADIF)) == 0);//wait for conversion complete
		
		v <<= 2;
		v |= ADC & 0x03;
	}
	
	return v;
}

void generate_token(uint8_t * token){
	uint8_t x;
	for(x=0; x<8; x++){
		token[x] = get_random_byte();	
	}
}
