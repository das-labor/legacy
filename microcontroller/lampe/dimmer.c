
#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>

static unsigned char COUNT;
unsigned char BRIGHT[4];


SIGNAL(SIG_INTERRUPT1) {
	TCNT0 = 256-20;
	COUNT = 0;
}


SIGNAL(SIG_OVERFLOW0) {
	TCNT0 = 256-20;
	
	if(COUNT == 0){
		PORTB |= 0x0F;
	}
	
	
	unsigned char x, rol=0x08;
	for(x=0;x<4;x++){
		if(BRIGHT[x] == COUNT){
			PORTB &= ~rol;
		}
		rol>>=1;
	}
	
	
	if(++COUNT == 64){
		COUNT = 0;
	}	
}


void dimmer_init(){
	MCUCR |=  (1<<ISC11) | (1<<ISC10);
	GIMSK |= (1<<INT1);

	/* 	CS02 C021 C020
		 0    0    0	       stop
		 0    0    1       clk
		 0    1    0       clk/8
		 0    1    1       clk/64
		 1    0    0       clk/256
		 1    1    1       clk/1024	
*/
	TCCR0 = 0x03;		//clk/64 
	
	TCNT0 = 256-20;
	TIFR = (1<<TOV0);
	TIMSK = (1<<TOIE0);	//overflow int on
}

