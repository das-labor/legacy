#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>

#include "motion.h"

volatile static uint16_t adcval[2];
volatile static uint8_t mtd_state = MTD_STATE_ENABLED;

uint16_t filter (uint8_t in_idx, uint16_t in_val)
{
	static int32_t fval[2];
	static uint8_t init_done = 0;
	int32_t delta[2];

	if (init_done != 0x03)
	{
		fval[in_idx] = in_val * 65536;
		init_done |= (0x01 << in_idx);
		return 0;
	}
	delta[in_idx] = ((in_val * 65536) - fval[in_idx]);
	fval[in_idx] += delta[in_idx] / 1024;
	return abs(delta[in_idx] / 65536);
}


uint8_t motion_check()
{
	uint8_t i, tmp;

	for (i=0;i<2;i++)
	{
		tmp = filter (i, adcval[i]);
		if (tmp > MTD_TRESHOLD)
			return tmp;
/*
		if (adcval[i] > (refval[i] + MTD_TRESHOLD) || adcval[i] < (refval[i] - MTD_TRESHOLD))
//		if (adcval[i] < MTD_TRESHOLD)
			return 1 + i;

		if (adcval[i] < MTD_MINVAL)
			return i+1;
*/
	}
	return 0;
}

void mtd_enable()
{
	mtd_state |= MTD_STATE_ENABLED;
}

void mtd_disable()
{
	mtd_state &= ~(MTD_STATE_ENABLED);
}

void motiond_tick ()
{
	static uint8_t mystate = 0;

	if (!(mtd_state & MTD_STATE_ENABLED)) return;
	
	switch  (mystate)
	{
		case 0x00:
			ADMUX = (_BV(REFS0) | _BV(REFS1) | 0x01); // int. 2.56V ref, left adjust result, channel 1
			ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0));
		break;
		case 0x01:
			if (ADCSRA & _BV(ADSC)) return; /* conversion is not done yet */
			
			adcval[0] = ADC;
		break;
		case 0x02:
			ADMUX = (_BV(REFS0) | _BV(REFS1) | 0x02); // int. 2.56V ref, left adjust result, channel 2
			ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0));
		break;
		case 0x03:
			if (ADCSRA & _BV(ADSC)) return; /* conversion is not done yet */
			
			adcval[1] = ADC;
			mystate = 0;
		break;
		default:
			mystate = 0;
			return;
	}
	mystate++;
}

uint16_t get_adc (uint8_t in_num)
{
	return adcval[in_num];
}

