#include "adc.h"

volatile static uint8_t adcd_state = ADCD_IDLE;

void adcd_ctl (uint8_t in_state)
{
	if (adcd_state != ADCD_IDLE)
		return;
	
	adcd_state = in_state;
	switch (in_state)
	{
		case ADCD_SAMPLE_MOTOR:
			SENSE_SPEED_SETUP();
			/*
			currywurst
			M_TIMER_OFF();
			M_EN_PORT &= ~(M_EN_PIN);
			M_CHOP();
			M_TIMER_INIT();
			*/
		break;
	}
}

ISR(ADC_vect)
{
	uint16_t a = ADC;
/*
	//SENSE_SPEED_SETUP();
	M_OCR++;
	//ADCSRA |= (_BV(ADIF) | _BV(ADSC));
	return;
*/
	switch (adcd_state)
	{
		case ADCD_IDLE:
			return;

		case ADCD_SAMPLE_MOTOR:
		{
			static uint16_t res = 0;
			static uint8_t c = 0;

			c++;
			if (c == 1)
			{
				/*
				M_CHOP();
				M_EN_PORT &= ~(M_EN_PIN);
				M_TIMER_OFF();
				SENSE_INC();
				*/
				return; /* first result -> discard */
			}

			res += a;

			if (c < 9)
				return;

			ADCSRA &= ~(_BV(ADEN) | _BV(ADIE));
			res /= 8;
			c = 0;
			adcd_state = ADCD_IDLE;
			lock_release (LOCK_ADC);
			motor_set_sampleval (res);
			/*
			SENSE_DEC();
			M_TIMER_INIT();
			*/
		}
	}
}
