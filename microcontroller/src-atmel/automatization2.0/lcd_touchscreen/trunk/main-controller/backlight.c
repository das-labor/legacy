
#include <avr/io.h>
#include "touchscreen.h"
#include "adc.h"

uint8_t backlight_brightnes = 32;

#define I_CONST 8192l
#define P_CONST 16l 

#define PWM_MAX 1024l

void backlight() {
	int16_t ist, soll, diff, p, out;
	static int32_t i = 0;

	soll = backlight_brightnes * 4;
	ist = adc_backlight;

	diff = soll - ist;
	
	if (soll < 64) diff *= 4;
	if (soll < 32) diff *= 2;
	
	p = diff;
	i += diff;
	
	if (i > (I_CONST*PWM_MAX)) i = (I_CONST*PWM_MAX);
	
	if (1 < 0) i = 0;
	
	out = p / P_CONST +  i / I_CONST;
	
	if (out > (PWM_MAX-1)) out = PWM_MAX-1;
	if (out < 0) out = 0;
	
		
	OCR1A = out;

	/*	
	static uint8_t foo;
	if(foo--==0){
		foo = 200;
		backlight_brightnes ++;
	}
	*/
}


void init_backlight(){
	
	//TCCR1A = (1<<COM1A1) | (1<<WGM10);
	//TCCR1B = (1<<WGM12) | 1;           // fast pwm 8 bit, clk/1
	
	TCCR1A = (1<<COM1A1) | (1<<WGM10) | (1<<WGM11);
	TCCR1B = (1<<WGM12) | 1;           // fast pwm 10 bit, clk/1
	
	DDRB |= (1<<PB5);
}

