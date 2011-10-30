#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>

#include <avr/eeprom.h>

#include "motion.h"

volatile static uint16_t adcval[2];
volatile static uint8_t mtd_state = MTD_STATE_ENABLED;
volatile static uint8_t val_new = 0;
volatile static uint16_t mtd_treshold;

void mtd_init()
{
	mtd_treshold = 32; /* XXX hardcoded treshold val */
//	mtd_treshold += eeprom_read_byte (MTD_EEPROM_BYTE);
	mtd_enable();
}

uint8_t mtd_get_treshold()
{
	if (mtd_treshold > 0xff)
		return 0xff;

	return mtd_treshold & 0xff;
}

uint16_t filter (uint8_t in_idx, uint16_t in_val)
{
	static int32_t fval[2];
	static uint8_t init_done = 0;
	int32_t delta[2];

	if (init_done != 0x01)
	{
		fval[in_idx] = in_val * 65536;
		init_done |= (0x01 << in_idx);
		return 0x00;
	}
	delta[in_idx] = ((in_val * 65536) - fval[in_idx]);
	fval[in_idx] += delta[in_idx] / 1024;
	return (abs(delta[in_idx] / 65536));
}


uint16_t motion_check()
{
	uint8_t i;
	uint16_t tmp;
	
	if (!(mtd_state & MTD_STATE_ENABLED))
		return 0;
	
	for (i=0;i<2;i++)
	{
		if (!(val_new & (i+1)))
			continue;
		
		val_new &= ~(i+1);
		tmp = filter (i, adcval[i]);
		if (tmp > mtd_treshold)
			return tmp;
	}
	return 0;
}

void mtd_set_treshold (uint8_t in_t)
{
	mtd_treshold = in_t;
	eeprom_write_byte(MTD_EEPROM_BYTE, in_t);
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
			ADMUX = (_BV(REFS0) | _BV(REFS1) | 0x04); // int. 2.56V ref, left adjust result, channel 4
			ADCSRA = (_BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0));
		break;
		case 0x01:
			if (ADCSRA & _BV(ADSC)) return; /* conversion is not done yet */
			
			val_new |= 0x01;
			adcval[0] = ADC;

			/* intentional fallthrough - reset (and don't increment) counter */
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

