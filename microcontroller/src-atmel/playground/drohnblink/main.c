
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "pwm.h"
#include "uart.h"

void mydelay(){
	uint8_t x;
	for(x=0;x<100;x++){
		asm volatile("nop");
	}
}


#define NUM_CHANNELS 12

int main(){
	uint8_t c, x, y, addr_offset;
	
	addr_offset = eeprom_read_byte((uint8_t *)1);
	//addr_offset = 0;
	
	uart_init();
	
	init_pwm();
	sei();
		
	while(1){
		//wait for sync
		while(1){
			c = uart_getc();
			if(c == 0x55)
				break;
		}
	
	sync:
		
		
		for(x=0; x<addr_offset; x++){
			c = uart_getc();
			if(c == 0x55) goto sync;
		}
		
		for(x=0; x<NUM_CHANNELS; x++){
			for(y=0;y<3;y++){		
				c = uart_getc();
				if(c == 0x55) goto sync;
				bright[y][x] = c;
			}
		}
				
		update_timeslots();
	}
}












/*
	uint8_t x=0, y=0, up=1;
	while (1) {
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
	
			bright[0][5] = x;
			bright[1][5] = 63-x;
			bright[2][6] = x;
			bright[0][6] = 63-x;
			bright[1][7] = x;
			bright[2][7] = 63-x;
	
				update_timeslots();

		}
	}
	*/
