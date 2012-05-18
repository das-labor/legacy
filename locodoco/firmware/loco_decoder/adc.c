#include "adc.h"

volatile uint8_t adcd_state = ADCD_IDLE;

void adcd_ctl (uint8_t in_state)
{
	if (adcd_state != ADCD_IDLE)
		return;
	
	adcd_state = in_state;
	switch (in_state)
	{
		case ADCD_SAMPLE_MOTOR:
			SENSE_SPEED_SETUP();
		break;
	}
}

ISR(ADC_vect)
{
	switch (adcd_state)
	{
		case ADCD_IDLE:
			return;

		case ADCD_SAMPLE_MOTOR:
		{
			static uint16_t res = 0;
			static uint8_t c = 0;

			if (!c)
				c = 1;
				return; /* first result -> discard */

			res += ADC;
			c++;

			if (c < 5)
				return;

			res /= 4;
			c = 0;
			adcd_state = ADCD_IDLE;
			motor_set_sampleval (res);
			ADCSRA &= ~(_BV(ADEN) | _BV(ADIE));
		}
	}
}
