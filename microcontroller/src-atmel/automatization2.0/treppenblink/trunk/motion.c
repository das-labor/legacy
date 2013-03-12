

volatile static uint16_t adcval[2];
volatile static uint8_t mtd_state = MTD_STATE_ENABLED;
volatile static uint8_t val_new = 0;


Tuint16 filter (Tuint08 in_idx, Tuint16 in_val)
{
	static int32_t fval[2];
	static Tuint08 init_done = 0;
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


Tuint16 motion_check()
{
	Tuint08 i;
	Tuint16 tmp;

	
	for (i = 0; i < 2; i++)
	{
		if (!(val_new & (i + 1)))
			continue;
		
		val_new &= ~(i + 1);
		tmp = filter (i, adcval[i]);
		if (tmp > 28)
			return tmp;
	}
	return 0;
}


#if (preTaskDefined(motiond))

void appLoop_motiond(void)
{
  can_message bla = {0x25,0x00,0x00,0x00,0x01, {0x00}};
  Tuint16 foo;
	while (true)
	{
		motiond_tick();
		foo = motion_check();
		if (foo)
		{
			bla.data[0] = foo;
			can_transmit(&bla);
		}
		taskDelayFromNow(100);
	}
}
#endif

void motiond_tick()
{
	static Tuint08 mystate = 0;
	
	switch  (mystate)
	{
		case 0x00:
			ADMUX = (_BV(REFS0) | _BV(REFS1) | 0x02); // int. 2.56V ref, left adjust result, channel 1 adc2
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

Tuint16 get_adc (Tuint08 in_num)
{
	return adcval[in_num];
}
