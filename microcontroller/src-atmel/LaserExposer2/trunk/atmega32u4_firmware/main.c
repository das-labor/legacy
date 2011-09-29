//Timer1 carriage driver control
//Timer4 prism motor control
//Timer0 laser_driver sync timeout
//Timer3 global prism_messurements

#include "config.h"
#include "laser_control.h"
#include "carriage_driver.h"
#include "prism_control.h"
#include "serial.h"
#include <inttypes.h>
#include <avr/eeprom.h>

#ifndef	ATMEGAEEPROM_H
#define ATMEGAEEPROM_H

// EEMEM wird bei aktuellen Versionen der avr-lib in eeprom.h definiert
// hier: definiere falls noch nicht bekannt ("alte" avr-libc)
#ifndef EEMEM
// alle Textstellen EEMEM im Quellcode durch __attribute__ ... ersetzen
#define EEMEM  __attribute__ ((section (".eeprom")))
#endif

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
	
	if(eeprom_read())
	{
		exposer.prism_freq = EEPROMconfig->prism_freq;
		exposer.buffer_length = EEPROMconfig->buffer_length;
		exposer.backspeed = EEPROMconfig->backspeed;
		exposer.endline = EEPROMconfig->endline;
		exposer.fastforwardspeed = EEPROMconfig->fastforwardspeed;
		exposer.plotstartpos = EEPROMconfig->plotstartpos;
		exposer.linesperrotaryenctick = EEPROMconfig->linespoerrotaryenctick;
		exposer.plotforwardspeed = EEPROMconfig->plotforwardspeed;
		exposer.status |=  EXPOSER_CONFIG;
	}
	laser_control_setbufsize(exposer.buffer_length);
	
	timer4_init();
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
		//wait for data
		if(usb_configured() && usb_serial_available())
			serial_handledata();
		POWER_DOWN		//go to sleep mode
	}
	
	//move carriage to start
	LED_READY_ON
	bts7741g_backwards(exposer.backspeed);		//move to start
	
	while(!EXPOSER_IS_RUNNING)
	{
		//wait for data
		if(usb_configured() && usb_serial_available())
			serial_handledata();
		POWER_DOWN		//go to sleep mode
	}
	
	//calibrate
	LED_READY_OFF
	LED_CALIBRATING_ON
	laser_control_setmode( LASER_MODE_INIT );
	
	if(EXPOSER_CONFIG_LOADED)
	{
		send_Pstr(PSTR("calibrating...\n\r"));
		exposer.prism_freq = prism_calibrate(buffer_length, 2);
		
		EEPROMconfig->prism_freq =exposer.prism_freq;
		EEPROMconfig->buffer_length =exposer.buffer_length;
		EEPROMconfig->backspeed =exposer.backspeed;
		EEPROMconfig->endline =exposer.endline;
		EEPROMconfig->fastforwardspeed= exposer.fastforwardspeed;
		EEPROMconfig->plotstartpos =exposer.plotstartpos;
		EEPROMconfig->linespoerrotaryenctick =exposer.linesperrotaryenctick;
		EEPROMconfig->plotforwardspeed = exposer.plotforwardspeed;

		eeprom_write();
	}
	else
	{
		send_Pstr(PSTR("using given prism_freq\n\r"));
	}
	
	prism_start();
	_sleep_ms(3000);
	//goto to idle mode
	laser_control_setmode( LASER_MODE_IDLE );


	
	
	
	serial_handledata();
	
	LED_CALIBRATING_OFF
	LED_RUNNING_ON
	
	//move carriage to startline
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
	
	prism_set_frequency(exposer.prism_freq);
	
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
		
		while(!EXPOSER_USB_READY)
		{
			//get new usb data
			while(!usb_configured())
				asm volatile ("nop");
			
			while(!usb_serial_available())
				asm volatile ("nop");
			
			serial_handledata();
		}
		exposer.status &=~EXPOSER_USB_GO;
		
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
	prism_stop();
	//move carriage to start, if possible
	if(SUPPLY_SENSE_PIN)
		carriage_tostart();
		
	//if this is true && !SUPPLY_SENSE_PIN then power has been switched off, goto error
	if(exposer.status && !SUPPLY_SENSE_PIN)
	{
		//error handler
		send_Pstr(PSTR("ERR:PowerOff\n\r"));
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

uint8_t EEMEM eeprom_status = 1;

//saves given struct to eeprom memory 
void eeprom_write()
{
	eeprom_write_block(s,&EEPROMconfig,sizeof(EEPROMconfig));	//write s to eeprom_struct in eeprom
	if(!(eeprom_read_byte(&eeprom_status) == 0xAA))
		eeprom_write_byte(&eeprom_status, 0xAA);
}

//loads struct from eeprom memory to given pointer
uint8_t eeprom_read()
{
	if(!(eeprom_read_byte(&eeprom_status) == 0xAA))
		return 0;
	eeprom_read_block(s,&EEPROMconfig,sizeof(EEPROMconfig));
	return 1;
}

