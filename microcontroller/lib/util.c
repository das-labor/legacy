#include <avr/io.h>

#include "util.h"
#ifdef UTIL_TIMER_HW

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
	TCCR2 = 0x0D;	//CTC Mode, clk/128
	OCR2 = (F_CPU/128000);	//1000Hz 
	for(;ms>0;ms--){
		while(!(TIFR&0x80));	//wait for compare matzch flag
		TIFR=0x80;		//reset flag
	}
}
#else // XXX
void wait(int ms){

	for(;ms>0;ms--){
 		unsigned int i;

		for(i=3000; i>0; i--) {
			asm volatile("nop");
		}
	}
	
}

#endif //UTIL_TIMER_HW

