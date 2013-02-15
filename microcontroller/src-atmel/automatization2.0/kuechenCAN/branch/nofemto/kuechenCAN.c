
#define C_SW  (0x00)
#define C_PWM (0x01)
#define O_SW00 (0x00)
#define O_PWM05 (0x05)
#define F_SW_TOGGLE   (0x03)
#define F_PWM_MOD     (0x02)
#define F_PWM_DIR     (0x03)

#define PWM_KUECHE		(O_PWM05)
#define SWL_KUECHE		(O_SW00)

//0x02 0x05 0x02 0x00
//0x02 0x00 0x03 0x00
uint8_t light=0;
uint8_t lastlight=0;
uint8_t rgbled_stat=0;
uint8_t rgb_panic=0;
uint8_t lastpanic=0;
uint8_t panic=0;

// ALARM in hex
can_message panic_msg = {0x23, 0x00, ALARMCANPORT, 0x00, 0x06, {0x41,0x4C,0x41,0x52,0x4D,0x00}};
//can_message light_on_msg = {0x23, 0x02, LIGHTCANPORT, 0x01, 0x04, {0x00,0x00,0x01,0x00}};
//can_message light_off_msg = {0x23, 0x02, LIGHTCANPORT, 0x01, 0x04, {0x00,0x00,0x00,0x00}};

void app_run(void)
{
	static can_message dstpower = {0x23,0x02,LIGHTCANPORT,0x01,0x04, {0x00,0x00,0x00,0x00}};
	static uint8_t counter_0;
	static uint8_t clicked_0 = 0;
	static uint8_t held_0    = 0;
	static uint8_t last_held_0;
	
	// lampe
	clicked_0=0;
	held_0=0;
	if(!(PINB & _BV(PB1))) 
	{
		counter_0++;
		if(counter_0 > HOLD_THRESHOLD)
		{
			held_0 = 1;
			counter_0 = HOLD_THRESHOLD;
		}
	} 
	else
	{
		if(counter_0 > CLICK_THRESHOLD)
		{
			if(counter_0 < HOLD_THRESHOLD)
			{
				clicked_0 = 1;
			}
		}
		counter_0 = 0;
	}
	
	if(clicked_0 == 1)
	{
		dstpower.data[0] = C_SW;
		dstpower.data[1] = SWL_KUECHE;
		dstpower.data[2] = F_SW_TOGGLE;
		if(rgbled_stat==R_LED)
		{
			rgbled_stat=G_LED;
		}
		else
		{
			rgbled_stat=R_LED;
		}
		can_transmit(&dstpower);
	}

	if(held_0)
	{
		dstpower.data[0] = C_PWM;
		dstpower.data[1] = PWM_KUECHE;
		dstpower.data[2] = F_PWM_MOD;
		can_transmit(&dstpower);
	}
	else if(last_held_0)
	{
		dstpower.data[0] = C_PWM;
		dstpower.data[1] = PWM_KUECHE;
		dstpower.data[2] = F_PWM_DIR;
		can_transmit(&dstpower);
	}

	last_held_0 = held_0;

	//panic
	if((PINC & _BV(PC0)) && (panic==0))
	{
		panic=1;
	}
	if(!(PINC & _BV(PC0)) && (panic==1))
	{
		panic=2;
	}
	if(panic==2)
	{
		can_transmit(&panic_msg);
		if(lastpanic == 0)
			{
				rgb_panic=1;
				lastpanic=1;
			}
		// you can send panic as offen as you want
		panic=0;
	}
	
	_delay_ms(100);
}

#if 0
//this is the deactivated panic button, it does not work with the current version
void appLoop_alarmt(void)
{
	while(true)
		{
			if(lastpanic==1)
				{
					// 10 sec blinken
					taskDelayFromNow(10000);
					rgb_panic=0;
					lastpanic=0;
				}
			taskDelayFromNow(100);	
		}
}

void appLoop_alarm(void)
{
	while(true)
		{
			if(lastpanic==1)
				{
					if(rgb_panic == 1)
						{
							rgb_panic=2;
						}
					taskDelayFromNow(500);
					if(rgb_panic == 2)
						{
							rgb_panic=1;
						}
				}
			taskDelayFromNow(100);
		}
}
#endif


void update_leds(void)
{
	if((rgbled_stat & R_LED) !=0)
	{
		PORTC |= R_LED;
	}
	else
	{
		PORTC &= ~R_LED;
	}
	if((rgbled_stat & G_LED) !=0)
	{
		PORTC |= G_LED;
	}
	else
	{
		PORTC &= ~G_LED;
	}
	if( (rgbled_stat & B_LED) !=0)
	{
		PORTC |= B_LED;
	}
	else
	{
		PORTC &= ~B_LED;
	}

	if( rgb_panic == 1 )
	{
		PORTD |= _BV(PD5);
		PORTD |= _BV(PD6);
		PORTD |= _BV(PD7);
	}
	if( rgb_panic == 2 )
	{
		PORTD &= ~_BV(PD5);
		PORTD &= ~_BV(PD6);
		PORTD &= ~_BV(PD7);
	}
}

