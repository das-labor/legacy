
#define NUMSTEPS 4

inline void mctl_step (uint8_t in_direction)
{
	static uint8_t currentstep;
	const uint8_t motorsteps[NUMSTEPS] = { 0x24, 0x30, 0x18, 0x0C };
	
	if (in_direction == MDIR_OPEN)
		currentstep = (currentstep == (NUMSTEPS-1)) ? 0 : currentstep+1;
	else
		currentstep = (currentstep == 0) ? (NUMSTEPS-1) : currentstep-1;

	MOTOR_PORT &= ~(MOTOR_MASK);
	MOTOR_PORT |= motorsteps[currentstep];
}

inline void mctl_init ( void )
{
	MOTOR_DDR |= MOTOR_MASK;
	MOTOR_PORT &= ~(MOTOR_MASK);

	ADMUX = 0xc1;
	ADCSRA = (1<<ADEN) | 7; // activate adc.
}

extern inline uint8_t mctl_ctl ( uint8_t in_action )
{
	static uint8_t currentstate = MCTL_STATE_UNKNOWN;

	/* disallow changes to the state if there's a pending operation */
	if (currentstate & MCTL_STATE_BUSY && in_action != MCTL_STATE_BLOCKED)
		return currentstate;

	switch (in_action)
	{
		case MCTL_OPEN_DOOR:
			currentstate = MCTL_STATE_OPENING;
		break;
		
		case MCTL_CLOSE_DOOR:
			currentstate = MCTL_STATE_CLOSING;
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
	static uint8_t adcstate = 0;
#if MCTL_NUMSTEPS > 0xff
	static uint16_t numsteps = 0;
#else
	static uint8_t numsteps = 0;
#endif

	/* reset counters upon state change */
	if (mystate != mctl_ctl (MCTL_GET_STATE))
	{
		numsteps = 0;
		roundnum = 0;
		mystate = mctl_ctl (MCTL_GET_STATE);
	}

	switch (mystate)
	{
		case MCTL_STATE_OPENING:
			mctl_step (MCTL_OPEN_DOOR);
			numsteps++;
		break;
		case MCTL_STATE_CLOSING:
		case MCTL_STATE_CALIBRATE:  /* calibration == closing... for now */
			mctl_step (MCTL_CLOSE_DOOR);
			numsteps++;
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

	if (adcstate == 0) /* virgin state value */
	{
		adcstate = (adcval > MCTL_ADC_TRESHOLD) ? 2 : 1;
	} else if ((adcval > MCTL_ADC_TRESHOLD && adcstate == 1)   /* rising edge */
		|| (adcval < MCTL_ADC_TRESHOLD && adcstate == 2))  /* falling edge */
	{
		roundnum++;
		numsteps = 0;
	}

	if (numsteps > MCTL_NUMSTEPS) /* treshold hit! set error condition */
	{
		mystate = mctl_ctl (MCTL_STATE_BLOCKED);
	}
}
