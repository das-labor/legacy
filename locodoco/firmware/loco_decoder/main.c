#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"

//#include "rfm12/src/rfm12.h"
#include "rfm12.h"
#include "motor.h"
#include "adc.h"
#include "locodoco.h"

int main (void)
{
	MCUCR |= _BV(IVCE);
	MCUCR &= ~(_BV(IVSEL));
	//SENSE_SPEED_SETUP();
	SENSE_DEC();
	rfm12_init();
	motor_init();
	LIGHTS_INIT();
	LIGHT_CABIN_OFF();
	LIGHT_RW_ON();
	sei();

	while (23)
	{
		motor_tick();
		ldc_packet_handler();
	}
}
