//Timer1 carriage driver control
//Timer4 prism motor control
//Timer0

uint8_t exposer.status 
		exposer.powersupply
		exposer.start
void main(){
	CONFIGURE_LED_RUNNING		//set DDR
	CONFIGURE_LED_USBON		//set DDR
	CONFIGURE_LED_CALIBRATING	//set DDR
	CONFIGURE_LED_READY		//set DDR
	CONFIGURE_KEY_ON	//enable int0 pullup
	CONFIGURE_KEY_START	//enable int1 pullup
	CONFIGURE_INT0
	ENABLE_INT0
	CONFIGURE_INT3
	ENABLE_INT3
	POWER_REDUCE	//shut down all hardware that isn't needed
	sei();			//global allow interrupts
	
start:
	while(!exposer.running)
	{
		POWER_DOWN		//go to sleep mode
	}
	
	//move carriage to start
	LED_READY_ON
	
	while(!exposer.start)
	{
		POWER_DOWN		//go to sleep mode
	}
	exposer.start=0;
	
	//calibrate
	LED_READY_OFF
	LED_CALIBRATING_ON
	
	
	//goto to idle mode

	//wait for data
	LED_CALIBRATING_OFF
	LED_RUNNING_ON
	
	//plot
	while(exposer.running&exposer.powersupply){
	
	
	}
	
	LED_RUNNING_OFF
	goto start;
}

//Key On/Off pressed
ISR(INT0_vect)
{
		if(!exposer.running)	//is off, turn on
		{
			if(SUPPLY_SENSE_PIN)
			{
				exposer.running = 1;
			}
		}
		else
		{
			exposer.running = 0;
		}
}
//TODO: needed ?
//main supply sense
ISR(INT3_vect)
{
	if(!SUPPLY_SENSE_PIN)
	{
		exposer.powersupply = 1;
	}
	else
	{
		exposer.powersupply = 0;
	}
}

//TODO: needed ?
//start/stop button
ISR(INT1_vect)
{
	//start pressed
	if(!exposer.start)
	{
		exposer.start = 1;
	}
	else	//stop pressed
	{
		//goto idle mode
	}
}

void goto_sleep()
{
	ENABLE_INT0
	sei();
	POWER_DOWN
}