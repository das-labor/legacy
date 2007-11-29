
#include <avr/io.h>
#include <avr/interrupt.h>


#define PORT_PWM PORTD
#define BIT_PWM 5

SIGNAL(SIG_OUTPUT_COMPARE0){
	//PORT_PWM &= ~(1<<BIT_PWM);
}

SIGNAL(SIG_OVERFLOW0){
	PORT_PWM |= (1<<BIT_PWM);
}


void pwm_init(){
	TCCR0 = 2; //clk/8 (PWM freq = 5.4kHz)
	OCR0 = 128;
	
	TIMSK |= (1<<TOIE0) | (1<<OCIE0);
	
}
