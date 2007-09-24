#include <avr/io.h>
#include "avrx.h"

#include "door.h"


/* @brief shut down motor the hard way.
 */
inline void door_motor_off(void)
{
	DOOR_PORT &= ~(DOOR_PIN0 | DOOR_PIN1 | DOOR_PIN2 | DOOR_PIN3);
}

/* @brief motor watchdog function to turn motor automatically off.
 */
void door_motor_wd (uint8_t in_kick)
{
	static uint8_t mykicks = 0;
	if (in_kick > 0)
	{
		mykicks = in_kick;
	} else
	{
		if (mykicks > 0)
			mykicks--;
		else
			door_motor_off();
	}
}



/* @brief get adc values from infrared sensor on door
 */
uint16_t door_get_adcval ( void )
{
	uint16_t retval;
	uint8_t i;

	for(i=0; i<10; i++)
	{
		ADCSRA |= (1<<ADIF); // Reset interrupt flag
		ADCSRA |= (1<<ADSC); // start conversion
		while( (ADCSRA & (1<<ADIF)) == 0  ) asm volatile("nop");
		retval = ADC;
	}
	door_motor_wd (10);
	return retval;
}

/* @brief 0 = sensor is at mark point, 1 = sensor receives reflection
 */
uint8_t door_get_sensorstate ( void )
{
	if (door_get_adcval() < DOOR_SENSORTRESHOLD)
		return 1;
	return 0;
}

/* @brief set up ddrs for adc and motor port
 */
void door_init ()
{
	ADMUX = 0xc1; // see datasheet page 215.
	ACSR = 0x00;
	DOOR_ADCDDR &= ~(DOOR_ADCPIN);
	DOOR_ADCPORT &= ~(DOOR_ADCPIN);
	ADCSRA = (1<<ADEN) | 7; //ADC aktivieren, Takt/128
	DOOR_DDR |= (DOOR_PIN0 | DOOR_PIN1 | DOOR_PIN2 | DOOR_PIN3);
	DDRA |= (_BV(PA7));
	PORTA |= (_BV(PA7));
	door_motor_off();
}

/* @brief actually turns step motor for door
 */
void door_turn (uint8_t in_direction)
{
	static uint8_t mystate = 0x01;
	uint8_t oldstate;
	
	oldstate = mystate;

	if (in_direction == DOOR_DIR_LEFT)
	{
		mystate++;

	} else if (in_direction == DOOR_DIR_RIGHT)
	{
		mystate--;
	}

	switch (mystate & 0x03)
	{
		case 0x00:
			DOOR_PORT &= ~(DOOR_PIN3 | DOOR_PIN1 | DOOR_PIN2);
			DOOR_PORT |= DOOR_PIN0;
		break;
		case 0x01:
			DOOR_PORT &= ~(DOOR_PIN3 | DOOR_PIN0 | DOOR_PIN2);
			DOOR_PORT |= DOOR_PIN1;
		break;
		case 0x02:
			DOOR_PORT &= ~(DOOR_PIN3 | DOOR_PIN1 | DOOR_PIN0);
			DOOR_PORT |= DOOR_PIN2;
		break;
		case 0x03:
			DOOR_PORT &= ~(DOOR_PIN0 | DOOR_PIN1 | DOOR_PIN2);
			DOOR_PORT |= DOOR_PIN3;
		break;
	}

}

AVRX_GCC_TASKDEF(door, 200, 5)
{
	uint8_t mystate = DOOR_STATE_INIT;
	uint16_t optotimeout = DOOR_OPTOTIMEOUT;
	uint8_t spincycles = 0;
#if (DOOR_SPINTICKS>0xff)
	uint16_t spinticks = 0;
#else
	uint8_t  spinticks = 0;
#endif

#if (DOOR_MAXTICKS > 0xff)
	uint16_t totalticks = 0;
#else
	uint8_t  totalticks = 0;
#endif
	TimerControlBlock doortimer;

	door_init();

	// wait 500ms for adc & stuff to calm
	AvrXDelay (&doortimer, 500);
	mystate = DOOR_DIR_LEFT;
	while (1)
	{
		AvrXDelay (&doortimer, DOOR_WAITCYCLES);
//		door_motor_wd(0);
		if (!door_get_sensorstate()) // mark hit
		{
			if (spinticks > 0)
			{
				spinticks--;
			} else
			{
				spinticks = DOOR_SPINTICKS;
				spincycles++;
			}
			/* reset optotimeout */
			optotimeout = DOOR_OPTOTIMEOUT;
			door_turn (mystate & 0xF0);
		} else if (optotimeout > DOOR_WAITCYCLES) // spin around and count down
		{
			optotimeout -= DOOR_WAITCYCLES;
			door_turn (mystate & 0xF0);
		} else // timeout hit!
		{
			door_motor_off();
		}
		totalticks++;
		if (totalticks >= DOOR_MAXTICKS) door_motor_off();
	}
}
