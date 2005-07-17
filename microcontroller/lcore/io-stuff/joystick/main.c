//demo.c
//This is a simple Demo for the Labor Board.
//It displays 2 different light patterns on the LEDs,
//and allows the pattern to be selected, and the speed to be controled
//with the 4 keys. 

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "uart.h"
#include "util.h"
#include "console.h"



void ad_init(){
	//         reference AVCC     left adjust  channel 0
	ADMUX = (0<<REFS1)|(1<<REFS0)|(1<<ADLAR)|     0;
	
	//      A/D enable  start conv.  auto conv.  int enabl.  int flg    clk/128
	ADCSRA = (1<<ADEN) |(1<<ADSC) | (1<<ADATE) | (0<<ADIF) | (0<<ADIE) | 7;

	SFIOR &= 0x0F; //free running mode
}

unsigned char ad_read(){
	return (ADCH);
}

typedef enum{
	up,down, left, right
}direction;

unsigned char joystick_read(direction * dir){
	static unsigned char joy_ax[2];
	if (ADCSRA & (1<<ADIF)){
		joy_ax[(ADMUX^1)&0x0F] = ADCH;
		ADCSRA |= (1<<ADIF);
		ADMUX ^= 1;
		ADCSRA |= (1<<ADSC);
	}
	unsigned char count = 0;
	if(joy_ax[0]< 0x70){
		count++;
		*dir = up;
	}else if (joy_ax[0] > 0xb0){
		count++;
		*dir = down;
	}

	if(joy_ax[1]< 0x70){
		count++;
		*dir = left;
	}else if (joy_ax[1] > 0xb8){
		count++;
		*dir = right;
	}
	
	if(count == 1) return 1;
	return 0;
}

int main (void)
{	
	DDRB = 0; 	//Port B all inputs
	PORTB = 0x0F;	//Internal Pullups on on key inputs
	
	DDRC = 0xFF; 	//Port C all outputs
	PORTC = 0x00;   //1 LED on to start of Patterns
	
	PORTA = 0x0F;
	
	ad_init();
	
	uart_init();
	sei();
	
	//uart_putc('*');
	
	//char buf[10];
	//itoa(uart_getc(), buf, 16);
	//uart_putstr(buf);
	
	
	
			
	for ( ;; ){		//for ever
		unsigned char tmp;
		direction dir;
		if(joystick_read(&dir)){
			switch (dir){
				case up:
					PORTC = 0x04;
					break;
				case down:
					PORTC = 0x02;
					break;
				case right:
					PORTC = 0x01;
					break;
				case left:
					PORTC = 0x08;
					break;
			}
		}else{
			PORTC = 0;
		}
		
		//PORTC = 1<<((tmp-0x68)>>4);
		
		console();	
	};	
}

