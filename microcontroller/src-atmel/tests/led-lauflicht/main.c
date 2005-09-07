//demo.c
//This is a simple Demo for the Labor Board.
//It displays 2 different light patterns on the LEDs,
//and allows the pattern to be selected, and the speed to be controled
//with the 4 keys. 

#include <inttypes.h>
#include <avr/io.h>

#include "util.h"

int main (void)
{	
	DDRB = 0; 	//Port B all inputs
	PORTB = 0x0F;	//Internal Pullups on on key inputs
	
	DDRC = 0xFF; 	//Port C all outputs
	PORTC = 0x01;   //1 LED on to start of Patterns
	
	int delay = 200;
	char mode=0, ud=0;
		
	
	for ( ;; ){		//for ever
		
		wait(delay);

		if(!mode){
			//knight rider mode
			switch (ud){
				case 0: 		//shift light left
					if( (PORTC<<=1) == 0x80 ){ //until bit7 is reached
						ud = 1;
					};
					break;
				case 1:			//shift bit right
					if( (PORTC>>=1) == 0x01 ){ //until bit0 is reached
						ud = 0;
					};
					break;
			};
		}else{
			//scroll mode
			switch (ud){
				case 0:
					PORTC|=(PORTC<<1);//shift additional ones in from left
					if(PORTC & 0x80)  //until bit7 is reached
						ud = 1;
					break;
				case 1:
					PORTC<<=1;	//shift zeros in
					if (PORTC == 0){//until all zeros
						ud = 0;
						PORTC = 0x01;//start over
					}
					break;
			
			};
		
		}
		
		switch(PINB&0x0f){
			case 0x0E:	//Button 1 pressed
				if (delay < 1000){
					delay += 10;
				}
				break;
			case 0x0D:	//Button 2 pressed
				if (delay > 10){
					delay -= 10;
				}
				break;
			case 0x0B:	//Button 3 pressed
				mode = 0;
				break;
			case 0x07:	//Button 4 pressed	
				mode = 1;
				break;
		}; 
	
	
	};	
}
