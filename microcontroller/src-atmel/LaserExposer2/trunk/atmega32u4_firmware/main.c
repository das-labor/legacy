//Timer1 carriage driver control
//Timer4 prism motor control
//Timer0 laser_driver sync timeout
//Timer3 global prism_messurements

#define EXPOSER_OFF 0
#define EXPOSER_RUNNING _BV(1)
#define EXPOSER_ON _BV(2)

uint8_t exposer.status 
		exposer.powersupply
		exposer.start
		exposer.currentline
		exposer.endline
		
void main(){
	CONFIGURE_LED_RUNNING		//set DDR
	CONFIGURE_LED_USBON		//set DDR
	CONFIGURE_LED_CALIBRATING	//set DDR
	CONFIGURE_LED_READY		//set DDR
	CONFIGURE_KEY_ON	//enable int0 pullup
	CONFIGURE_KEY_START	//enable int1 pullup
	CONFIGURE_INT0
	ENABLE_INT0
	//CONFIGURE_INT3
	//ENABLE_INT3
	CONFIGURE_INT6
	
	laser_control_setmode(LASER_MODE_OFF);
	
	POWER_REDUCE	//shut down all hardware that isn't needed
	sei();			//global allow interrupts
	
start:
	LED_RUNNING_OFF
	LED_READY_OFF
	LED_CALIBRATING_OFF
	exposer.status = 0;
	
	while(!(exposer.status & EXPOSER_ON))
	{
		POWER_DOWN		//go to sleep mode
	}
	
	//move carriage to start
	LED_READY_ON
	
	while(!(exposer.status & EXPOSER_RUNNING))
	{
		POWER_DOWN		//go to sleep mode
	}
	
	//calibrate
	LED_READY_OFF
	LED_CALIBRATING_ON
	laser_control_setmode( LASER_MODE_INIT );
	
	//goto to idle mode
	laser_control_setmode( LASER_MODE_IDLE );
	
	//move carriage to startline
	
	//wait for data
	
	LED_CALIBRATING_OFF
	LED_RUNNING_ON
	laser_control_setmode( LASER_MODE_RUNNING );
	//plot
	while(exposer.status && SUPPLY_SENSE_PIN){
		
		//get new usb data
		
		//active carriage motor
		
		CLEAR_ROT1_PIN
		while(!ROT1_PIN)	//wait until 
		{
			for(uint8_t lineloops=0; lineloops < exposer.linerepeatcount; lineloops++)
			{
				//plot
			
				//if(ROT1_PIN)	//error, carriage driver is to fast
					
			}
		
		}
		//stop carriage motor
		
		exposer.currentline++;
		
		while(!(exposer.status & EXPOSER_RUNNING))	//if stop button is pressed wait
			asm volatile("nop");
			
		laser_control_setmode( LASER_MODE_RUNNING );
		
		//check if we reached the end
		if(exposer.currentline >= exposer.endline)
			exposer.status &= ~(EXPOSER_RUNNING|EXPOSER_ON);
	}
	
	laser_control_setmode(LASER_MODE_OFF);
	//move carriage to start, if possible
	if(SUPPLY_SENSE_PIN)
		carriage_tostart();
	//if this is true && !SUPPLY_SENSE_PIN then power has been switched off, goto error
	if(exposer.status && !SUPPLY_SENSE_PIN)
	{
		//error handler
		
	}
	
	LED_RUNNING_OFF
	goto start;
}

//Key On/Off pressed
ISR(INT0_vect)
{
		if(!(exposer.status & EXPOSER_ON))	//is off, turn on
		{
			if(SUPPLY_SENSE_PIN)
			{
				exposer.status |= EXPOSER_ON;
			}
		}
		else
		{
			exposer.status &= ~EXPOSER_OFF;
		}
}
#if 0
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
#endif

//TODO: needed ?
//start/stop button
ISR(INT1_vect)
{
	//start pressed
	if(exposer.status & EXPOSER_ON)
	{
		if(!(exposer.status & EXPOSER_RUNNING))
		{
			exposer.status |= EXPOSER_RUNNING;
		}
		else	//stop pressed
		{
			//goto idle mode
			exposer.status &= ~EXPOSER_RUNNING;
			laser_control_setmode( LASER_MODE_IDLE );
		}
	}
}

void goto_sleep()
{
	ENABLE_INT0
	sei();
	POWER_DOWN
}