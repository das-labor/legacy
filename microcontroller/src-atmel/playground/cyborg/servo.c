
#include <avr/io.h>
#include <avrx.h>

volatile uint8_t pulselength;

#define TCCR2_CLK128 5
#define TCCR2_CLK1024 7

#define PORT_SERVO PORTD
#define DDR_SERVO DDRD
#define BIT_SERVO PD4


AVRX_SIGINT(SIG_OUTPUT_COMPARE2){
	IntProlog();                // Save interrupted context, switch stacks
	if(TCCR2 == ((1<<WGM21) | TCCR2_CLK1024)){
		PORT_SERVO |= (1<<BIT_SERVO);
		TCCR2 = (1<<WGM21) | TCCR2_CLK128;
		OCR2 = pulselength;
	}else{
		PORT_SERVO &= ~(1<<BIT_SERVO);
		TCCR2 = (1<<WGM21) | TCCR2_CLK1024;
		OCR2 = 255;
	}
	Epilog();                   // Restore context of next running task
}

void servo_init(){
	DDR_SERVO |= (1<<BIT_SERVO);

	pulselength = 110;

	TCCR2 = (1<<WGM21) | TCCR2_CLK1024;
	OCR2 = 255;
	TIMSK |= (1<<OCIE2);
}

void set_servo(uint8_t value){
	if(value < 60) value = 60;
	//if(value > 255) value = 255;
	
	pulselength = value;
}
