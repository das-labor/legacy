//Timer1 carriage driver control
//Timer4 prism motor control
//Timer0 laser_driver sync timeout
//Timer3 global prism_messurements

#include "config.h"
#include "laser_control.h"
#include "carriage_driver.h"
#include "prism_control.h"
#include "serial.h"

exposer_t exposer;

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
	LED_USBON_OFF
	LED_RUNNING_OFF
	LED_CALIBRATING_OFF
	LED_READY_OFF
	laser_control_setmode(LASER_MODE_OFF);
		
	POWER_REDUCE	//shut down all hardware that isn't needed
	usb_init();	//enable hardware usb interface
	while (!usb_configured()) /* wait */ ;
	{	
	POWER_DOWN		//TODO: check if this is working
	}
	//sei();
	LED_USBON_ON
	
start:
	LED_RUNNING_OFF
	LED_READY_OFF
	LED_CALIBRATING_OFF
	exposer.status = 0;
	bts7741g_stop();
	
	// wait for the user to run their terminal emulator program
	// which sets DTR to indicate it is ready to receive.
	while (!(usb_serial_get_control() & USB_SERIAL_DTR)) /* wait */ ;
	usb_serial_flush_input();
	// print a nice welcome message
	serial_welcomemsg();
	
	while(!EXPOSER_IS_ON)
	{
		POWER_DOWN		//go to sleep mode
	}
	
	//move carriage to start
	LED_READY_ON
	bts7741g_backwards(exposer.backspeed);		//move to start
	
	while(!EXPOSER_IS_RUNNING)
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
	while(!usb_configured())
		asm volatile ("nop");
	
	while(!usb_serial_available())
		asm volatile ("nop");
	
	serial_handledata();
		
	LED_CALIBRATING_OFF
	LED_RUNNING_ON
	
	//active carriage motor
	bts7741g_forward(exposer.fastforwardspeed);
	//wait i rotaryencoder ticks (drive to plot start position)
	for(uint16_t i=0;i<exposer.plotstartpos;i++)
	{
		CLEAR_ROT1_PIN
		while(!ROT1_PIN)	//wait until rotary encoder tick
			asm volatile("nop");
	}
	//stop carriage motor
	bts7741g_stop();	
	
	//send current line
	serial_sendcurrentline(exposer.plotstartpos);

	//plot
	int16_t lineloops=0;
	while(exposer.status && SUPPLY_SENSE_PIN){
		
		//check if stop button has been pressed
		if(!EXPOSER_IS_RUNNING)
		{
			//don't plot, but idle
			laser_control_setmode( LASER_MODE_IDLE );
		
			while(!EXPOSER_IS_RUNNING)	//if stop button is pressed wait
				asm volatile("nop");
		}
		
		//get new usb data
		while(!usb_configured())
			asm volatile ("nop");
		
		while(!usb_serial_available())
			asm volatile ("nop");
		
		serial_handledata();
		
		//update exposing time counter
		lineloops = exposer.linesperrotaryenctick;
		
		//disable interrupts that are not neccessary
		//needed: Timer0, SPI, INT2
		disable_noncritical_ints();
		
		//active carriage motor
		bts7741g_forward(exposer.plotforwardspeed);		//move forward
		
		//back to plotting mode
		laser_control_setmode( LASER_MODE_RUNNING );
		
		CLEAR_ROT1_PIN
		while(!ROT1_PIN)	//wait until rotary encoder tick
		{
			//is plotting here
			asm volatile("nop");
		}
		//stop carriage motor
		bts7741g_stop();
		laser_control_setmode( LASER_MODE_IDLE );
		
		#if 0
		//TODO: drive backwards until lineloops >= exposer.linesperrotaryenctick
		lineloops -= laser_control_readloopcounter();
		if(lineloops > 0)
		#endif
		
		//enable interrupts that may be neccessary
		enable_noncritical_ints();
			
		//increment the line counter
		exposer.currentline++;
		
		//check if we reached the end of pcb
		if(exposer.currentline >= exposer.endline)
			exposer.status &= ~(EXPOSER_RUNNING|EXPOSER_ON);
			
		//check the light barriers
		
	}
	
	//shut down laser, in any case
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
		if(!EXPOSER_IS_ON)	//is off, turn on
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

//TODO: needed ?
//start/stop button
ISR(INT1_vect)
{
	//start pressed
	if(EXPOSER_IS_ON)
	{
		if(!EXPOSER_IS_RUNNING)
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

void disable_noncritical_ints()
{
	DISABLE_INT0
	DISABLE_INT1
	DISABLE_USB_INT
}

void enable_noncritical_ints()
{
	ENABLE_INT0
	ENABLE_INT1
	ENABLE_USB
}


