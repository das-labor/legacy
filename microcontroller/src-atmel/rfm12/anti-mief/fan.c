#include <avr/io.h>
#include <stdint.h>

#include "fan.h"
#include "menu.h"
#include "cron.h"

inline void fan_off(volatile uint16_t in_dummy)
{
	LPORT &= ~(LPIN);
}

inline void fan_on(volatile uint16_t in_dummy)
{
	LPORT |= (LPIN);
}


void fan_init()
{
	LDDR |= LPIN;
}

/* turn on the fan for in_seconds */
void fan_kick (uint16_t in_seconds)
{
	if (!in_seconds) return;
	fan_on(0);
	cron_add (in_seconds, fan_off, 0);
}

/* @description turn on the fan for a given time of minutes (!) after a given time of minutes.
 * @param the first (higher) 8 bits of the given parameter denotes the time in minutes after
 * 	which the fan shall be turned on. the lower 8 bits denote the time the fan should run.
 */
void fan_delaykick (uint16_t in_param)
{
	cron_add ((in_param >> 8) * 60, fan_on, 0);
	cron_add (((in_param & 0xff) * 60) + ((in_param >> 8) * 60), fan_off, 0);
}

