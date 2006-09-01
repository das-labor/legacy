#include <avr/io.h>

#include "util.h"
//#include "config.h"

void wait(int ms){
	TCCR1B = 0x0C;	//CTC Mode, clk/256
	OCR1A = (F_CPU/256000);	//1000Hz 
	for(;ms>0;ms--){
		while(!(TIFR&(1<<OCF1A)));	//wait for compare matzch flag
		TIFR=(1<<OCF1A);		//reset flag
	}
}


