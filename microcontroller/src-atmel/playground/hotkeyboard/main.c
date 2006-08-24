#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "uart.h"
#include <stdlib.h>

#include "ps2.h"



//4   5  6  3
//11 12 13 14
//15  8  9
//7   0  1  2
//   a   j

unsigned char matrix[] PROGMEM ={2, 3, 16, 13, escape, 11, 12, 1, 5, 6, 0, 7, 8, 9, 14, 4,0,0,0, 15 };


void keypressed(unsigned char keynum, unsigned char state){
	unsigned char tmp = pgm_read_byte(&matrix[keynum]);
	
	if( tmp == 16){
		if(!state){
			key_make(alt);
		}else{
			key_break(alt);
		}
	}else if((tmp == escape) && (!state)){
		key_press(tmp);
	}else if(!state){
		key_make(ctrl);
		key_press(tmp);
		key_break(ctrl);
	}
}

void delay1(){
	unsigned char x;
	for(x=0;x<50;x++){
		unsigned char data;
		if(ps2_get_nb(&data)){
			ps2_put(1, 0xfa, 0);
			//char buf[10];
			//itoa(data, buf, 16);
			//uart_putstr(buf);
		}
	}
}

int main(){
	//DDRC = 0xFF;
	//PORTB = 0xFF;
	
	DDRB |= 0xE0; //Rows of Keyboard are outputs
	PORTB |= 0xE0; //and low active

	
	init_ps2();
	
	sei();

	volatile unsigned int delay;
	for(delay=0;delay<1000;delay++);
	
	ps2_put(1, 0xaa, 0);
		
		
	for(;;){
		unsigned char keys1[3], keys2[3];
		
		unsigned char x,y;
		for(x=0;x<3;x++){
			PORTB |= 0xE0;
			PORTB &= 0xE0^(0x20<<x);
			delay1();
			keys1[x] = ((PINB<<3) & 0xF8) | (PIND & 0x07);
			unsigned char rol = 0x01, tmp;
			tmp = keys1[x] ^ keys2[x];
			for(y=0;y<8;y++){
				if(tmp & rol){
					keypressed((x<<3)+y, keys1[x] & rol);
				}
				rol<<=1;
			}
			keys2[x] = keys1[x];
		}
		
			
	}
}

