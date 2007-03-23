
#include <string.h>
#include <avrx.h>

#include <stdio.h>

#include <avr/io.h>

#include "adc.h"

TimerControlBlock my_timer1, my_timer2;


AVRX_GCC_TASKDEF(task1, 200, 4)
{
	
	printf("Debug\r");
	
	//LED Pin to output
	DDRD |= (1<<PD4);
		
	while(1){
		AvrXDelay(&my_timer1, 50);
		printf("%d\t%d\t%d     \r", adc_value[2], adc_value[3], adc_value[6]);
	}
}


#define MTRL_FWD() PORTD |= (1<<PD5); PORTD &= ~(1<<PD6)
#define MTRL_BCK() PORTD &= ~(1<<PD5); PORTD |= (1<<PD6)

#define MTRR_FWD() PORTD |= (1<<PD7); PORTB &= ~(1<<PB0)
#define MTRR_BCK() PORTD &= ~(1<<PD7); PORTB |= (1<<PB0)




AVRX_GCC_TASKDEF(task2, 300, 5)
{
	
	//Motor-Ports initialisieren
	DDRD |= 0xE0;
	DDRB |= 0x07;
	
	//Fast PWM/8 bit
	TCCR1A = (1<<COM1A1) | (0<<COM1A0) | (1<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (1<<WGM10);
	TCCR1B = (0<<WGM13) | (1<<WGM12) | 5;// clk/1024
	
	PORTD |= 0xa0; //Motors forward
	
	PORTB |= (1<<PB3);// Pullup on button on
	
	int16_t pwm[2];
	
	//wait for button
	while(PINB & (1<<PB3) ){
		AvrXDelay(&my_timer2, 20);
	}
	
	while(1){
		AvrXDelay(&my_timer2, 1);
		
		int16_t diff;
		int16_t posdiff;
		uint16_t mid;
		
		diff = adc_value[3] - adc_value[2] + 6;
		mid = adc_value[3] + adc_value[2];
		
		
		posdiff = diff;
		if(posdiff < 0) posdiff = -posdiff;
		
		if((posdiff > 10) || (mid > 300) ){
			pwm[0] += 1;
			pwm[1] += 1;
		
			if(diff > 0){
				pwm[0] -= 2;
				pwm[1] += 2;
			}else{
				pwm[0] += 2;
				pwm[1] -= 2;
			}
		
			#define MAX_SPEED 70
			#define MIN_SPEED -50
		
			if(pwm[0] < MIN_SPEED) pwm[0] = MIN_SPEED;
			if(pwm[0] > MAX_SPEED) pwm[0] = MAX_SPEED;	
			if(pwm[1] < MIN_SPEED) pwm[1] = MIN_SPEED;
			if(pwm[1] > MAX_SPEED) pwm[1] = MAX_SPEED;

			if(pwm[0] >= 0){			
				MTRL_FWD();
				OCR1AL = pwm[0];
			}else{
				MTRL_BCK();
				OCR1AL = -pwm[0];
			}

			if(pwm[1] >= 0){			
				MTRR_FWD();
				OCR1BL = pwm[1];
			}else{
				MTRR_BCK();
				OCR1BL = -pwm[1];
			}

		}
	}
}
