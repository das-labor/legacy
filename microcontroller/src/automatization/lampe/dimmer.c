
#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include "dimmer.h"


static unsigned char COUNT;
unsigned char Bright[NUM_LAMPS] = {0,44,};


SIGNAL(SIG_INTERRUPT1) {
	TCNT0 = 256-20;
	COUNT = 0;
}


typedef struct{
	unsigned char delay;
	unsigned char timer;
	unsigned char delta[NUM_LAMPS];
}dimmer_ramp_t;

static dimmer_ramp_t Dimmer_ramp[5]={
	{0x50,43,{1,-1,}},
	{0x50,43,{-1,1,}},
	{0x50,43,{0,-1,}},
	{0x50,43,{1,1,}},
	{0x50,43,{-1,0,}},
};

SIGNAL(SIG_OVERFLOW0) {
	
	static unsigned char delay, delay_rl, timer, ramp_count=4;
	
	static dimmer_ramp_t *ramps = Dimmer_ramp;
	
	TCNT0 = 256-20;
	
	if(COUNT == 0){
		PORTB |= 0x0F;
	}
	
	if(!(COUNT %8)){
		if(!delay--){
			if (!(timer--)){
				if(++ramp_count == 5) ramp_count = 0;
				delay_rl = ramps[ramp_count].delay;
				timer = ramps[ramp_count].timer;
			}
			
			unsigned char x;
			for(x=0; x<NUM_LAMPS;x++){
				Bright[x] += ramps[ramp_count].delta[x];
			}
			
			delay = delay_rl;
		}
	}
	
	
	unsigned char x, rol=0x08;
	for(x=0;x<NUM_LAMPS;x++){
		if(Bright[x] == COUNT){
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

