

#include <avr/io.h>

// code by tixiv
//
// cap 100n connected between PD5 and PD6.
// PD 6 is additionally connected to capacitive sensor plate.
// code charges plate by applying VCC on PD6, then charges this charge into
// the 100n cap between PD5 and PD6. It is counted how many cycles are needed 
// to charge the 100n cap. 
//
//  Indicator LED connected to PORTA


int main(){

	DDRA = 0xff;

	while(1){
		uint16_t counter = 0;
		
		DDRD = (1<<PD6) | (1<<PD5);
		
		__asm volatile ( "nop");
		__asm volatile ( "nop");
		__asm volatile ( "nop");
		__asm volatile ( "nop");
		__asm volatile ( "nop");
		
		
		do{
			counter++;
			DDRD = 0;
			
			PORTD = (1<<PD6);
			DDRD = (1<<PD6);
			DDRD = 0;
			PORTD = 0;
			
			DDRD  = (1<<PD5);
			__asm volatile ( "nop");
			__asm volatile ( "nop");
		}while((PIND & (1<<PD6)) == 0);

		DDRD = 0;
			

		if(counter > 155){
			PORTA = 0x0;
		}	else if (counter < 145){
			PORTA = 0xff;
		}
	
	}	
}
