#include <avr/io.h>
#include <stdint.h>
#include <avr/io.h>

#include "eyes.h"

static uint16_t adcval[2];

uint16_t get_adcval (uint8_t in_num)
{
	return adcval[in_num];
}

void eyes_tick()
{
	static uint8_t mystate = 0;
	
	switch  (mystate)
	{
		case 0x00:
			ADMUX = (_BV(REFS0) | 0x00); // AVCC ref, left adjust result, channel 0
			ADCSRA |= (_BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0));
		break;
		case 0x01:
			if (ADCSRA & _BV(ADSC)) return; /* conversion is not done yet */
			
			adcval[0] = ADC;
		break;
		case 0x02:
			ADMUX = (_BV(REFS0) | 0x01); // AVCC ref, left adjust result, channel 1
			ADCSRA |= (_BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0));
		break;
		case 0x03:
			if (ADCSRA & _BV(ADSC)) return; /* conversion is not done yet */
			
			adcval[1] = ADC;
		break;
		default:
			mystate = 0;
			return;
	}
	mystate++;
}
