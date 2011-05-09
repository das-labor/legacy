
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ioport.h"


//init timer 1 for fg sensor
void timer_init(){
	ACSR = (1<<ACIC); //Analog comparator for input capture
	
	TCCR1B = (1<<ICNC1) | 1; //noise canceler, clk/1
	TIMSK |= (1<<ICIE1) | (1<<OCIE1A);
	
}

#define SET_PWM(v) OCR0A=(v)

uint8_t overflow;

volatile uint8_t freq_correct;
volatile uint8_t enabled;

ISR(TIMER1_COMPA_vect){
	
	PORTD ^= 1<<PD1;
	
	if(enabled){
		SET_PWM(255);
	}
	overflow = 1;
	OCR1A += 32768;
}

int16_t   integral;


ISR(TIMER1_CAPT_vect){
	static uint16_t  old_icr;
	uint16_t         icr;
	uint16_t         y;
	int16_t          a;
	
	icr     = ICR1;
	y       = icr - old_icr;
	if(y < 1000) return;
	old_icr = icr;

	PORTD ^= 1<<PD0;

	freq_correct = 0;
	OCR1A = icr + 32768; //for overflow
		
	if(overflow == 1){
		overflow = 0;
	}else{		
		if(y < 0) y = 10000;
		if(y > 10000) y = 10000;
		
		a = y - 8000;
		
		if((a < 40) && (a > -40)){
			freq_correct = 1;
		}
		
		integral += a;
		
		#define I 32
		
		if(integral >  (I*256)) integral = I*256 ;
		if(integral < -(I*256)) integral = -(I*256);
		
		a *= 1;
		a += integral/I;
		if(a > 255) a = 255;
		if(a < 0  ) a = 0;
		
		if(enabled){
			SET_PWM(a);
		}
	}
}


#define ENABLE_PORT D
#define ENABLE_BIT  3


#define OC0A_PORT B
#define OC0A_BIT  2

#define LOCKED_PORT D
#define LOCKED_BIT  6


//init timer 0 for Motor PWM
void pwm_init(){
	TCCR0A = (1<<COM0A1) | (1<<COM0A0) | (1<<WGM01) | (1<<WGM00) ; //fast inverted PWM on OC0A
	TCCR0B = 1; //clk/1
	
	OCR0A = 0;
	SET_DDR(OC0A);
}


int main(){
	pwm_init();
	timer_init();
	
	OUTPUT_ON(ENABLE);//pullup
	SET_DDR(LOCKED);
	
	DDRD |= 3;
	PORTB = 3;
	
	sei();
	
	uint8_t locked;
	uint16_t correct;
	
	while(1){
		_delay_us (100);
		
		if(freq_correct){
			correct ++;
			if(correct > 2000){ 
				correct = 2000;
				locked = 1;
			}
		}else{
			correct = 0;
			locked = 0;
		}
		
		if(!INPUT(ENABLE)){
			enabled = 1;
			if(locked){
				OUTPUT_ON(LOCKED);
			}else{
				OUTPUT_OFF(LOCKED);
			}
		}else{
			enabled = 0;
			SET_PWM(0);
			OUTPUT_OFF(LOCKED);
			correct = 0;
		}
	}
	
}
