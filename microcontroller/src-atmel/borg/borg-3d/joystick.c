#include "joystick.h"
#include <avr/io.h>

unsigned char waitForFire;


void joy_init(){
	DDRJOYGND |= (1<<BITJOY0)|(1<<BITJOY1);
	PORTJOYGND |= (1<<BITJOY0)|(1<<BITJOY1);

	PORTJOYDIRS |= (1<<BITUP)|(1<<BITDOWN)|(1<<BITLEFT)|(1<<BITRIGHT)|(1<<BITFIRE);
}

