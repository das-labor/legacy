
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "borg_hw.h"
#include "pixel.h"
#include "7seg.h"



void test1(){
	while(1){
		uint8_t x;
		for(x=0;x<16;x++){
			_delay_ms(100);
			setpixel(x,1);
		}
		for(x=0;x<16;x++){
			_delay_ms(100);
			setpixel(x,0);
		}
	}

}


int main(){
	
	borg_hw_init();

	sei();

	while(1){
		uint8_t x;
		for(x=0;x<16;x++){
			//_delay_ms(100);
			uint8_t value = (keys[(x/4)] & (1<<(x%4))) ? 0:1;
			
			setpixel(x, value);
			
			if(value){
				uint8_t sbuf[10];
				sprintf(sbuf, "%04d",x);
				seg_print(0, sbuf);
			}
			
		}
	}

}