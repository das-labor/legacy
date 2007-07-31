
#include <avr/io.h>
#include <avr/interrupt.h>

#include "pwm.h"

void mydelay(){
	uint8_t x;
	for(x=0;x<100;x++){
		asm volatile("nop");
	}
}



int main(){
	
	bright[0][0] = 1;
	bright[0][1] = 16;
	bright[1][2] = 16;
	
	update_timeslots();
	
	init_pwm();
	sei();
	
	uint8_t x=0, y=0, up=1;
	while (1) {
		/* at the beginning of each pwm cycle, call the fading engine and
		* execute all script threads */
		if (global.flags.new_cycle) {
			global.flags.new_cycle = 0;
		
			if(y++ < 10) continue;
			y=0;
			
			if(up){
				x++;
				if (x == 63) up=0;
			}else{
				x--;
				if(x==0)up=1;
			}
			
			bright[0][2] = x;
			bright[1][2] = 63-x;
			bright[2][3] = x;
			bright[0][3] = 63-x;
			bright[1][4] = x;
			bright[2][4] = 63-x;
	
				update_timeslots();

		}
	}



}
