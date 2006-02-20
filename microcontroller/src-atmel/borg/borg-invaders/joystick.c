#include "joystick.h"
#include <avr/io.h>

unsigned char waitForFire;


inline void joy_init(){
	DDRJOYDIRS  &= ~((1<<BITUP)|(1<<BITDOWN)|(1<<BITLEFT)|(1<<BITRIGHT));
	PORTJOYDIRS |= (1<<BITUP)|(1<<BITDOWN)|(1<<BITLEFT)|(1<<BITRIGHT);

	DDRJOYFIRE  &= ~(1<<BITUP);
	PORTJOYFIRE |= (1<<BITUP);
}
