

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


#define FILTER_CONSTANT 32768ul

int main()
{

	DDRB |= _BV(PB0);


	uint32_t filt = FILTER_CONSTANT * 400;
	
	uint16_t thresh;


	while (1)
	{
		uint16_t counter = 0;
		
		DDRC |= _BV(PC3) | _BV(PC2);
		
		__asm volatile ( "nop");
		__asm volatile ( "nop");
		__asm volatile ( "nop");
		__asm volatile ( "nop");
		__asm volatile ( "nop");
		
		
		do
		{
			counter++;
			DDRC = 0;
			
			PORTC = _BV(PC3);
			DDRC = _BV(PC3);
			DDRC = 0;
			PORTC = 0;
			
			DDRC  = _BV(PC2);
			__asm volatile ( "nop");
			__asm volatile ( "nop");
		} while ((PINC & _BV(PC3)) == 0);

		DDRC = 0;
		


	
		thresh = filt/FILTER_CONSTANT;
		
		filt -= thresh;
		filt += counter;


		if (counter > (thresh-10) )
		{
			PORTB &= ~_BV(PB0);
		}
		else if (counter < (thresh - 30) )
		{
			PORTB |= _BV(PB0);
		}
	
	}	
}
