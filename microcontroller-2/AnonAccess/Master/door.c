#include <stdint.h>
#include <avr/io.h>
#include "door.h"

#define NUMSTEPS 4

inline void mctl_step (uint8_t in_direction)
{
	static uint8_t currentstep = 0;
#if 1
	const uint8_t motorsteps[NUMSTEPS] = { 0x24, 0x30, 0x18, 0x0C };
#else
	const uint8_t motorsteps[NUMSTEPS] = { 0x20, 0x10, 0x08, 0x04 };
#endif
//	const uint8_t motorsteps[NUMSTEPS] = { 0x14, 0x28, 0x30, 0x0C };
//	const uint8_t motorsteps[NUMSTEPS] = { 0x30, 0x18, 0x18, 0x0C };
	
	if (in_direction == MDIR_OPEN)
		currentstep = (currentstep == (NUMSTEPS-1)) ? 0 : currentstep+1;
	else
		currentstep = (currentstep == 0) ? (NUMSTEPS-1) : currentstep-1;

	MOTOR_PORT &= ~(MOTOR_MASK);
	MOTOR_PORT |= motorsteps[currentstep];
}

extern void mctl_init ( void )
{
	MOTOR_DDR |= MOTOR_MASK;
	MOTOR_PORT &= ~(MOTOR_MASK);
	MOTOR_PORT |= 0x03;

	ADMUX = 0xc1;
	ADCSRA = (1<<ADEN) | 7; // activate adc.
	
#define BIT_LED PA7	
	PORTA |= (1<<BIT_LED);
	DDRA |= (1<<BIT_LED);
}

extern uint8_t mctl_ctl ( uint8_t in_action )
{
	static uint8_t currentstate = MCTL_STATE_UNKNOWN;

	/* disallow changes to the state if there's a pending operation */
	if (in_action & MCTL_STATE_BUSY)
		return currentstate;

	switch (in_action)
	{
		case MCTL_OPEN_DOOR:
			currentstate = MCTL_STATE_OPENING;
		break;
		
		case MCTL_CLOSE_DOOR:
			currentstate = MCTL_STATE_CLOSING;
		break;
		case MCTL_GET_STATE:
			return currentstate;
		default:
			currentstate = in_action;
		break;
	}
	
	return currentstate;
}

extern void mctl_tick ( void )
{
	static uint8_t mystate = MCTL_STATE_UNKNOWN;
	static uint8_t roundnum = 0;
	uint8_t i;
	uint16_t adcval;
	static uint16_t numsteps = 0;

	/* reset counters upon state change */
	if (mystate != mctl_ctl (MCTL_GET_STATE))
	{
		numsteps = 0;
		roundnum = 0;
		mystate = mctl_ctl (MCTL_GET_STATE);
	}
	
	if (mystate & MCTL_STATE_BUSY) numsteps++;

	switch (mystate)
	{
		case MCTL_STATE_OPENING:
			mctl_step (MDIR_OPEN);
		break;
		case MCTL_STATE_CLOSING:
		case MCTL_STATE_CALIBRATING:  /* calibration == closing... for now */
			mctl_step (MDIR_CLOSE);
		break;
	}
	
	/* ADC foo */	
	for(i=0; i<10; i++)
	{
		ADCSRA |= (1<<ADIF);  /* reset interrupt flag */
		ADCSRA |= (1<<ADSC);  /* start conversion */
		while( (ADCSRA & (1<<ADIF)) == 0  ); /* wait for result */
		adcval = ADC;
	};

	if (adcval > MCTL_ADC_TRESHOLD && numsteps > MCTL_ROUNDTRIGGERLEVEL)
	{
		roundnum++;
		numsteps = 0;
	}

	if (roundnum == MCTL_ROUNDS)
		mctl_ctl (MCTL_STATE_IDLE);

	if (numsteps > MCTL_NUMSTEPS) /* treshold hit! set error condition */
	{
		mystate = mctl_ctl (MCTL_STATE_BLOCKED);
	}
}
