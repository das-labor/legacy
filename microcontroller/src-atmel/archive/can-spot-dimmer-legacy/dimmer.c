
#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include "dimmer.h"


unsigned char Count;
unsigned char Bright[NUM_LAMPS];

dimmer_ramp Ramp;

SIGNAL(SIG_INTERRUPT1) {
	TCNT0 = 256-20;
	Count = 2;
}


SIGNAL(SIG_OVERFLOW0) {
	unsigned char x, rol;
	TCNT0 = 256-20;
	
	if(Count == 0){
		PORTB |= 0x0F;
	}
	
	for(x=0,rol=0x08;x<NUM_LAMPS;x++){
		if(Bright[x] <= Count){
			PORTB &= ~rol;
		}
		rol>>=1;
	}
	
	if(++Count == 64){
		Count = 0;
	};
	
	for(x=0;x<NUM_LAMPS;x++){
		if(--Ramp.delay[x]==0){
			Ramp.delay[x] = Ramp.delay_rl[x];
			if(Ramp.end_bright[x]>Bright[x]){
				Bright[x]++;
			}else if(Ramp.end_bright[x]<Bright[x]){
				Bright[x]--;
			}
		}
	}
}


void dimmer_init(){
	unsigned char x;
	for(x=0;x<NUM_LAMPS;x++){
		Bright[x] = 64;
		Ramp.end_bright[x] = 64;
		Ramp.delay_rl[x] = 1;
		Ramp.delay[x] = 1;
	}
	
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

