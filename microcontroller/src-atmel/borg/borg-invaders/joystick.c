#include "joystick.h"
#include <avr/io.h>

unsigned char waitForFire;


inline void joy_init(){
	DDRB  &= ~((1<<BITUP)|(1<<BITDOWN)|(1<<BITLEFT)|(1<<BITRIGHT));
	PORTB |=   (1<<BITUP)|(1<<BITDOWN)|(1<<BITLEFT)|(1<<BITRIGHT);

	DDRD  &= ~(1<<BITFIRE);
	PORTD |=  (1<<BITFIRE);
}
