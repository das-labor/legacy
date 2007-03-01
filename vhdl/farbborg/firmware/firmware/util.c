#include <avr/io.h>

#include "config.h"

#ifndef F_CPU
#define F_CPU 16000000
#endif

void wait(int ms){
/* 	TCCR2: FOC2 WGM20 COM21 COM20 WGM21 CS22 CS21 CS20
		CS22 CS21 CS20
		 0    0    0	       stop
		 0    0    1       clk
		 0    1    0       clk/8
		 0    1    1       clk/32
		 1    0    0       clk/64
		 1    0    1       clk/128
		 1    1    0       clk/256
		 1    1    1       clk/1024	
*/
	TCCR0 = 0x05;			//CTC Mode, clk/128
	
	//OCR0 = (F_CPU/128000);	//1000Hz 
	for (;ms>0;ms--) {
		TCNT0 = 256-(F_CPU/128000);
		while (!(TIFR&(1<<TOV0)));	//wait for compare match flag
		TIFR=(1<<TOV0);				//reset flag
	}
}
