
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

uint8_t charge_table[256] PROGMEM = {
	0, 21, 90, 120, 138, 151, 160, 167, 173, 177, 181, 185, 188, 191, 193, 195,
	197, 199, 200, 202, 203, 204, 206, 207, 208, 209, 210, 210, 211, 212, 213, 213,
	214, 215, 215, 216, 216, 217, 218, 218, 218, 219, 219, 220, 220, 221, 221, 221,
	222, 222, 222, 223, 223, 223, 224, 224, 224, 224, 225, 225, 225, 226, 226, 226,
	226, 226, 227, 227, 227, 227, 228, 228, 228, 228, 228, 228, 229, 229, 229, 229,
	229, 229, 230, 230, 230, 230, 230, 230, 231, 231, 231, 231, 231, 231, 231, 231,
	232, 232, 232, 232, 232, 232, 232, 232, 233, 233, 233, 233, 233, 233, 233, 233,
	233, 233, 234, 234, 234, 234, 234, 234, 234, 234, 234, 234, 234, 235, 235, 235,
	235, 235, 235, 235, 235, 235, 235, 235, 235, 236, 236, 236, 236, 236, 236, 236,
	236, 236, 236, 236, 236, 236, 236, 236, 237, 237, 237, 237, 237, 237, 237, 237,
	237, 237, 237, 237, 237, 237, 237, 237, 237, 237, 238, 238, 238, 238, 238, 238,
	238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 238, 239, 239,
	239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239,
	239, 239, 239, 239, 239, 239, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
	240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
	240, 240, 240, 240, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241,
};


void init_pwm(){
	DDRB |= _BV(PB2); //set pwm pin to output	
	TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00); //fast pwm
	TCCR0B = 1;//clk / 1
}

#define BLITZER_DDR DDRB
#define BLITZER_PORT PORTB
#define BLITZER_BIT PB0


uint8_t adc_is_ready(){
	if(ADCSRA & _BV(ADSC)) return 0;
	return 1;
}

void adc_start_conversion(){
    ADMUX = _BV(REFS1) | 0; //1.1V reference, channel 0
	ADCSRA = (1<<ADEN)| 7;
    // start the conversion
    ADCSRA |= (1<<ADSC);
}

//#define ADC_VAL ((uint16_t)(1023.0 * 330.0 / 313.03 / 1.1)
//#define ADC_VAL 980

#define ADC_VAL 950

static uint16_t last_adc_val;
	
uint16_t read_adc_nb(){
	if(adc_is_ready()){
		last_adc_val = ADC;
		adc_start_conversion();
	}
	return last_adc_val;
}

uint16_t read_adc(){
	while(!adc_is_ready());
	
	last_adc_val = ADC;
	adc_start_conversion();
	return last_adc_val;
}

void init_timer1(){
	ACSR = _BV(ACIC);//analog comparator controlls input capture
}

#define LED_DDR DDRA
#define LED_PORT PORTA
#define LED_BIT PA7


int main(){
	BLITZER_DDR |= _BV(BLITZER_BIT);
	DDRB |= _BV(PB2); //set pwm pin to output	
	LED_DDR |= _BV(LED_BIT);
	
	init_timer1();
	
	while(1){
		if(ACSR & _BV(ACO)){
			LED_PORT |= _BV(LED_BIT);
		}else{
			LED_PORT &= ~_BV(LED_BIT);
		}
	}
	
	
	
	
	init_pwm();

	uint16_t x;
	uint16_t val = 0;
	adc_start_conversion();	
	while(1){
		//OCR0A = 240;
		for(x=0;x<256;x++){
			OCR0A = pgm_read_byte(&charge_table[x]);
			_delay_us(400);
			val = read_adc_nb();
			if (val > ADC_VAL){
				OCR0A = 0;
				break;
			}
		}
		
		while(val < ADC_VAL){
			val = read_adc();
		}
		
		OCR0A = 0;
		uint16_t delay = 4000;
		
		while(1){
			val = read_adc();
			if(val > ADC_VAL){
				OCR0A = 0;
			}else{
				OCR0A = 200;
			}
			delay--;
			if(delay == 0) break;
		}
		
			
		BLITZER_PORT |= _BV(BLITZER_BIT);
		_delay_ms(1);
		BLITZER_PORT &= ~_BV(BLITZER_BIT);
		
		_delay_ms(100);
	}
}
