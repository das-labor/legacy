#include "lock.h"
volatile static uint8_t locks = 0x00;

uint8_t try_lock_get (lock_t in_type)
{

	if (in_type == LOCK_ADC)
	{
		if (LOCK_ADC_MASK & locks)
			return 0;

		locks |= LOCK_ADC_MASK;
		return 1;
	}

	return 0;
}

void lock_release (lock_t in_type)
{
	if (in_type == LOCK_ADC)
	{
		locks &= ~(LOCK_ADC_MASK);
	}
}
