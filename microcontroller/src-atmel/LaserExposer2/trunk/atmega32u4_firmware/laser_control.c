typedef struct{
	uint8_t idle;
	uint8_t mode;
}laser_driver_t;

laser_driver_t laser_driver;

// HSYNC
ISR(INT2_vect)
{
	//got this interrupt, this means the exposer is on
	//check if laser is in
	//* idle
	//* running (if its not idle it must be running)
	if(laser_driver.idle){ 
		//set Timer0  to activate laser a few µs before it's crossing the detector again
		START_TIMER0 
		LASER_OFF;
		return;
	}
	//laser is running
	//TODO: start plotting ?

}

//this interrupt activates the laser  a few µs before it's crossing the detector again
//only active in idle mode
ISR(TIMER0_COMPA_vect)
{
	LASER_ALWAYS_ON;
	//Timer0 off
	STOP_TIMER0
}

void init_timer0()
{
	TCCR0A=_BV(WGM1);	//no output change, simple CTC (OCRA clears TCNT)
	TIMSK0=_BV(OCIE0A);	//enable OCR0A interrupt
}

#define ENABLE_INT2 EIMSK |=_BV(INT2);
#define DISABLE_INT2 EIMSK &=~BV(INT2);
#define CONFIGURE_INT2 EICRA|=_BV(ISC21);
 
#define LASER_MODE_OFF 0
#define LASER_MODE_INIT 1
#define LASER_MODE_IDLE 2
#define LASER_MODE_RUNNING 4

void laser_control_setmode(uint8_t mode)
{
	switch(mode)
	{
		case LASER_MODE_OFF:
			LASER_OFF	//turn laser off
			DISABLE_INT2
			laser_driver.idle = 0;
			laser_driver.mode = LASER_MODE_OFF;
		break;
		case LASER_MODE_INIT:
			LASER_ALWAYS_ON	//turn laser on
			DISABLE_INT2
			laser_driver.idle = 0;
			laser_driver.mode = LASER_MODE_INIT;
		break;
		case LASER_MODE_IDLE:
			if(laser_driver.mode == LASER_MODE_INIT||laser_driver.mode == LASER_MODE_RUNNING){
				laser_driver.idle = 1;
				init_timer0();
				ENABLE_INT2
				laser_driver.mode == LASER_MODE_IDLE;
			}
		break;
		case LASER_MODE_RUNNING:
			if(laser_driver.mode == LASER_MODE_INIT||laser_driver.mode == LASER_MODE_IDLE){
				laser_driver.idle = 0;
				init_timer0();
				ENABLE_INT2
				laser_driver.mode == LASER_MODE_RUNNING;
			}
		break;
	}
}

uint8_t laser_control_getmode()
{
	return laser_driver.mode;
}

#define START_TIMER0 TCCR0B=0;	//clock select
#define STOP_TIMER0 TCCR0B=0;
#define LASER_ALWAYS_ON {PORTB|=_BV(PB2);}	//TODO: use SPI modes
#define LASER_OFF {PORTB&=_BV(PB2);}
