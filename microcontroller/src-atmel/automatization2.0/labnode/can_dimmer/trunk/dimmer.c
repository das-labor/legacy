
#include <avr/io.h>
#include <avr/interrupt.h>

int dim_val = 300;

ISR(TIMER1_CAPT_vect){
	TCNT1 = 0;
	PORTA &= ~(1<<PA4);
	
	dim_val += 1;
	if(dim_val == 625) dim_val = 0;
	OCR1B = dim_val;
}

uint8_t state;

ISR(TIMER1_COMPB_vect){
	if(state == 0){
		PORTA |= (1<<PA4);
		OCR1B = dim_val + 10;
	}else{
		PORTA &= ~(1<<PA4);
		OCR1B = dim_val;
	
	}
	state ^= 1;
	
}

void dimmer_init(){
	DDRA |= (1<<PA4);

	TCCR1B |= (1<<ICNC1) | (1<<WGM12) | (1<<CS12); //CTC (TOP = OCR1A), clk/256
	OCR1A = 625;

	OCR1B = dim_val;

	TIMSK |= (1<<TICIE1) | (1<<OCIE1B);//Input capture int on, output compare 1B on


}
