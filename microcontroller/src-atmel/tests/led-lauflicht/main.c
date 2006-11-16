//demo.c
//This is a simple Demo for the Labor Board.
//It displays different light patterns on the LEDs,
//and allows the pattern to be selected, and the speed to be controled
//with the 4 keys. 

//avr io Registers like PORTC DDRC PINC ..... and much more
#include <avr/io.h>

//library for uint8_t uint16_t ....
#include <inttypes.h>

//header from our own utility library  - wait(uint16_t ms)
#include "util.h"



//main function
int main (void){	
	DDRB = 0; 	//Port B all inputs
	PORTB = 0x0F;	//Internal Pullups on on key inputs
	DDRC = 0xFF; 	//Port C all outputs
	PORTC = 0x01;   //1 LED on to start of Patterns
	
	//for lfsr mode
	uint8_t lfsr = 0;

	//variable for delay after each state
	uint16_t delay = 200;
	
	//which pattern are we displaying?
	uint8_t mode=0;
		
	while (1){		//for ever
		
		//wait a little
		wait(delay);

		//now decide, which Pattern we show, by mode
		switch(mode){
		//knight rider mode
		case 0:{
			//static variables are only initialized once at startup,
			//not on every cycle.
			static uint8_t ud1 = 0;
			
			switch (ud1){
				case 0: 		//shift light left
					if( (PORTC<<=1) == 0x80 ){ //until bit7 is reached
						ud1 = 1;
					}
					break;
				case 1:			//shift bit right
					if( (PORTC>>=1) == 0x01 ){ //until bit0 is reached
						ud1 = 0;
					}
					break;
			}
			break;
		}
		//scroll mode
		case 1:{
			static uint8_t ud2 = 0;
			switch (ud2){
				case 0:
					PORTC|=(PORTC<<1);//shift additional ones in from left
					if(PORTC & 0x80)  //until bit7 is reached
						ud2 = 1;
					break;
				case 1:
					PORTC<<=1;	//shift zeros in
					if (PORTC == 0){//until all zeros
						ud2 = 0;
						PORTC = 0x01;//start over
					}
					break;
			}
			break;
		}
		//flash all
		case 2:{
			static uint8_t fl=0;
			switch (fl){
				case 0:
					PORTC=0xFF;
					fl = 1;
					break;
				case 1:
					PORTC=0;
					fl = 0;
					break;
			}
			break;
		}
		//flash half half
		case 3:{
			static uint8_t hh=0;
			switch (hh){
				case 0:
					PORTC=0xF0;
					hh = 1;
					break;
				case 1:
					PORTC=0x0F;	
					hh = 0;
					break;
			
			}
			break;
		}
		//binary counter
		case 4:{
			static uint8_t count;
			PORTC = count;
			count++;		
			break;
		}
		//LFSR (linear feedback shiftregister)
		case 5:{
			uint8_t tmp;
			#define LFSR_MASK 0x83
			#define LFSR_INIT 1
			PORTC = lfsr;
			tmp = lfsr & LFSR_MASK;
			lfsr<<=1;
			while(tmp){
				lfsr^=tmp&1;
				tmp>>=1;
			}
			break;
		}
		}
		
		switch(PINB&0x0f){ //mask to only get the Button bits from PINB
			case 0x0E:	//Button 1 pressed
				if (delay < 1000){
					delay += 10; //increase delay
				}
				break;
			case 0x0D:	//Button 2 pressed
				if (delay > 10){
					delay -= 10; //decrease delay
				}
				break;
			case 0x0B:	//Button 3 pressed
				--mode; //last mode
				if(mode == 0xff){
					mode=5;
				}
				if (mode == 5){
					//init lfsr if we change to that mode
					lfsr=LFSR_INIT;
				}
				break;
			case 0x07:	//Button 4 pressed
				mode++;
				
				if (mode>5){
					mode = 0;
				}
				
				if (mode == 5){
					//init lfsr if we change to that mode
					lfsr=LFSR_INIT;
				}
				break;
		} 
	}	
}
