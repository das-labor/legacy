#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"

//#include "rfm12/src/rfm12.h"
#include "rfm12.h"
#include "motor.h"
#include "adc.h"

int main (void)
{
	MCUCR |= _BV(IVCE);
	MCUCR &= ~(_BV(IVSEL));
	//SENSE_SPEED_SETUP();
	SENSE_DEC();
	motor_init();
	sei();

	while (23)
	{
		motor_tick();
	}
}
