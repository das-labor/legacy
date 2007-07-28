
#include <string.h>
#include <avrx.h>

#include <stdio.h>

#include <avr/io.h>

#include "adc.h"


//#define RANGE

TimerControlBlock my_timer1, my_timer2;


AVRX_GCC_TASKDEF(task1, 100, 4)
{
	
	printf("Debug\r");
	
	//LED Pin to output
	DDRB |= (1<<PB4);
		
	while(1){
		AvrXDelay(&my_timer1, 200);
		//PORTB ^= (1<<PB4);
	}
}


#define MTRL_FWD() PORTD |= (1<<PD5); PORTD &= ~(1<<PD6)
#define MTRL_BCK() PORTD &= ~(1<<PD5); PORTD |= (1<<PD6)

#define MTRR_FWD() PORTD |= (1<<PD7); PORTB &= ~(1<<PB0)
#define MTRR_BCK() PORTD &= ~(1<<PD7); PORTB |= (1<<PB0)



	int16_t pwm[2];

void set_motor(){
	#define MAX_SPEED 0x80
	#define MIN_SPEED -0x60
	
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

uint8_t noline = 0;
uint8_t end = 0;


uint8_t opto[6];
uint8_t offset[6]={40,20,20,20,20,60};

int8_t get_linepos(){
	uint8_t x;
	for(x=0;x<6;x++){
		uint16_t tmp;
		tmp = adc_value[x] + offset[x];
		if(tmp > 255) tmp = 255;
		opto[x] = tmp;
	}
	int16_t pos = 0;
	int16_t div = 0;
	for(x=0;x<6;x++){
		int16_t tmp;
		tmp = 255 - opto[x];
		div += tmp;
//		printf("%d ", tmp);
		tmp = tmp * ((x*32) - 80);
		pos += tmp;
	}
//	printf("\r%d %d\r", div, pos);
	
	static int8_t lastpos;
	if(div > 750){
		end = 1;
	}else{
		end = 0;
	}
	if(div < 20){
		noline = 1;
		return lastpos;
	}else{
		noline = 0;
		pos = pos/div;
		lastpos = pos;
		return pos;
	}
}


#ifdef RANGE
uint8_t tas;

void turn_and_search(){
	tas = 1;
	pwm[0] = 0x60;
	pwm[1] = -0x60;
	set_motor();
	AvrXDelay(&my_timer2, 200);
	pwm[0] = 0x20;
	pwm[1] = 0x70;
	set_motor();
	
	do{
		get_linepos();
		AvrXDelay(&my_timer2, 20);
	}while(noline);	
	
	pwm[0] = -0x60;
	pwm[1] = 0x60;
	set_motor();
	AvrXDelay(&my_timer2, 100);
	tas = 0;
	get_linepos();
}

#endif

void followline(){
		
		
		
		static uint8_t oldpos[16];
		static uint8_t oldpos_cnt;
		int8_t delta;
		int16_t reg;
		
		int8_t pos; 

		//read line sensor		
		pos = get_linepos();

		//calculate delta-value
		oldpos[oldpos_cnt] = pos;
		delta = pos - oldpos[(oldpos_cnt+15)%16];
		oldpos_cnt = (oldpos_cnt + 1)%16;

		//printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\r", adc_value[0], adc_value[1], adc_value[2], adc_value[3], adc_value[4], adc_value[5], pos);
		//printf("%d\r", pos);
		
		reg = 3*pos + 55*delta;
		
		pwm[0] = 0x65 + reg;
		pwm[1] = 0x65 - reg;
	
		set_motor();
	
		if (noline){
			PORTB |= (1<<PB4);
		}else{
			PORTB &= ~(1<<PB4);
		}

#ifdef RANGE		
		static uint8_t rc;
			
		if(!tas){
			if(adc_value[6] > 32){
				rc++;
				if(rc == 5){
					rc = 0;
					turn_and_search();
				}
			}else{
				rc = 0;
			}
		}
#endif
		AvrXDelay(&my_timer2, 4);

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
	
	while(!end){
		followline();
	}

/*
	pwm[0] = -80;
	pwm[1] = 80;
	set_motor();
	AvrXDelay(&my_timer2, 500);
	
	do{
		get_linepos();
		AvrXDelay(&my_timer2, 20);
	}while(noline);	
	
	uint16_t y;
	
	for(y=0; y<500; y++){
		followline();
	}
	
	while(!end){
		followline();
	}
*/	
	pwm[0] = 0;
	pwm[1] = 0;
	set_motor();
	
	while(1);

}


TimerControlBlock range_timer;
uint8_t range[20];

AVRX_GCC_TASKDEF(rangefinder, 100, 6)
{
	while(1){
		uint8_t pos;
		for(pos=0;pos<20;pos++){
			AvrXDelay(&range_timer, 100);
			range[pos] = adc_value[6];
			set_servo(60+pos*10);
		}
		set_servo(60);
		printf("-----------------\r");
		for(pos=0;pos<20;pos++){
			//printf("%03d ",range[pos]);
			uint8_t c;
			for(c=0;c<range[pos];c+=2){
				printf("#");
			}
			printf("\r");
			
		}
		printf("------------------\r");
		AvrXDelay(&range_timer, 200);
	}	
}
