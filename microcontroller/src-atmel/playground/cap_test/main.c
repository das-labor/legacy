

#include <avr/io.h>
#include <util/delay.h>

// code by tixiv
//
// cap 100n connected between PD5 and PD6.
// PD 6 is additionally connected to capacitive sensor plate.
// code charges plate by applying VCC on PC3, then charges this charge into
// the 100n cap between PC2 and PC3. It is counted how many cycles are needed 
// to charge the 100n cap. 
//
//  Indicator LED connected to PORTA
//
// LEDs in proximity to sensor are connected to PC4(Anode) and PC5(Cathode)


//#define FILTER_CONSTANT 8192ul
#define FILTER_CONSTANT 1024ul

int main()
{

	DDRB |= _BV(PB0);

	uint32_t filt = FILTER_CONSTANT * 600;
	
	uint16_t thresh;
	uint8_t output = 0;

	while (1)
	{
		uint16_t counter = 0;
		
		
		DDRC |= _BV(PC3) | _BV(PC2) ; //discharge cap

		_delay_us(10);		
		
		do
		{
			counter++;
			DDRC &= ~(_BV(PC2) | _BV(PC3));//discharge off from last cycle

			__asm volatile ( "nop");
			
			PORTC |= _BV(PC3) | _BV(PC4) ;//charge cap 
			if(output) PORTC |=  _BV(PC5); //dont activate illumination if touched
			DDRC |= _BV(PC3) | _BV(PC4) | _BV(PC5) ;
			__asm volatile ( "nop");
			__asm volatile ( "nop");
			__asm volatile ( "nop");
			__asm volatile ( "nop");
			__asm volatile ( "nop");
			
			PORTC |=  _BV(PC5); //set all lines to high
			
			__asm volatile ( "nop");
			__asm volatile ( "nop");
			
			DDRC &= ~(_BV(PC3) | _BV(PC4) | _BV(PC5)); //release lines
			PORTC &= ~(_BV(PC3) | _BV(PC4) | _BV(PC5));
			
			DDRC  |= _BV(PC2);//discharge plate to 100n cap
			__asm volatile ( "nop");
			__asm volatile ( "nop");
		} while ((PINC & _BV(PC3)) == 0);

		DDRC &= ~_BV(PC2);//discharge off
		

		thresh = filt/FILTER_CONSTANT;
		
		filt -= thresh;
		filt += counter;


		if (counter > (thresh-10) )
		{
			output = 0;
			PORTB &= ~_BV(PB0);
		}
		else if (counter < (thresh - 20) )
		{
			output = 1;
			PORTB |= _BV(PB0);
		}
	
	}	
}
