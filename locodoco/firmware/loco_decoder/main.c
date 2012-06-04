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
	ldc_header_t tst;

	MCUCR |= _BV(IVCE);
	MCUCR &= ~(_BV(IVSEL));
	//SENSE_SPEED_SETUP();
	SENSE_DEC();
	motor_init();
	LIGHTS_INIT();
	LIGHT_CABIN_OFF();
	rfm12_init();
	rfm12_init();
	rfm12_init();
	//LIGHT_RW_ON();
	sei();

	rfm12_tx (sizeof(ldc_header_t), LDC_TYPE, (uint8_t*) &tst);


	while (23)
	{
		motor_tick();
		ldc_packet_handler();
	}
}
