
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
	DDRB |= (1<<PB4);
		
	while(1){
		AvrXDelay(&my_timer1, 200);
		PORTB ^= (1<<PB4);
	}
}


#define MTRL_FWD() PORTD |= (1<<PD5); PORTD &= ~(1<<PD6)
#define MTRL_BCK() PORTD &= ~(1<<PD5); PORTD |= (1<<PD6)

#define MTRR_FWD() PORTD |= (1<<PD7); PORTB &= ~(1<<PB0)
#define MTRR_BCK() PORTD &= ~(1<<PD7); PORTB |= (1<<PB0)



	int16_t pwm[2];

void set_motor(){
	#define MAX_SPEED 200
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

uint8_t opto[6];
uint8_t offset[6]={50,3,1,1,0,55};

int8_t get_linepos(){
	uint8_t x;
	uint8_t minpos = 0;
	uint16_t minval = 65535;
	
	for(x=0;x<6;x++){
		uint16_t tmp;
		tmp = adc_value[x] + offset[x];
		if(tmp > 255) tmp = 255;
		opto[x] = tmp;
	//	printf("%d ", tmp);
		
	}	
	//printf("\r");
		

	for(x=0;x<5;x++){
		uint16_t val;
		val = opto[x] + opto[x+1];
		//printf("%d ", val);
		
		if(val < minval){
			minval = val;
			minpos = x;
		}
	}
	
	//printf("\r");
	//printf("%d\r", minpos);
	
	
	uint8_t a,b;
	a = opto[minpos];
	b = opto[minpos+1];
	
	int8_t diff;
	diff = a-b;
	
	diff /= 2;
	
	if(diff > 7) diff = 7;
	if(diff < -8) diff = -8;
	
	int8_t pos;
	pos = -32 + minpos*16 + diff;
	return pos;
}

#define P 256
#define I 1
#define D 0

int16_t pid(int8_t in){
	static int16_t integrator;
	static int8_t old = 0;
	int16_t prop;
	int16_t diff;
	int16_t out;
		
	prop = in * P;
	integrator += in * I;
	diff = (in - old) * D;
	
	out = prop + integrator + diff;
	
	return out;
}

AVRX_GCC_TASKDEF(task2, 300, 5)
{
	
	//Motor-Ports initialisieren
	DDRD |= 0xE0;
	DDRB |= 0x07;
	
	//Fast PWM/8 bit
	TCCR1A = (1<<COM1A1) | (0<<COM1A0) | (1<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (1<<WGM10);
	TCCR1B = (0<<WGM13) | (1<<WGM12) | 5;// clk/1024
	
	PORTB |= (1<<PB3);// Pullup on button on
	
	
	//wait for button
	while(PINB & (1<<PB3) ){
		printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\r", adc_value[0], adc_value[1], adc_value[2], adc_value[3], adc_value[4], adc_value[5], adc_value[6]);
		AvrXDelay(&my_timer2, 20);
	}
	
	while(1){
		int8_t pos; int16_t out;
		pos = get_linepos();
		printf("%d\r", pos);
		
		//out = pid(pos);
	
		//printf("%d", out);
		
		pwm[0] = 0xb0 + pos*8;
		pwm[1] = 0xb0 - pos*8;
		
		set_motor();
	
		AvrXDelay(&my_timer2, 10);
	}

}
