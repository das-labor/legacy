//laser_control.c
//used hardware:
//* Timer0
//* Timer3
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"
#include "laser_control.h"

laser_driver_t laser_driver;

// HSYNC
ISR(INT2_vect)
{
	//got this interrupt, this means the exposer is on
	//check if laser is in
	//* idle
	//* running (if its not idle it must be running)
	
	START_TIMER0 	//set Timer0  to activate laser a few µs before it's crossing the detector again
	
	if(laser_driver.idle){ 
		FORCE_SPI_MOSI_LOW
		return;
	}
	//laser is running
	
	//increment loop counter
	laser_driver.loops++;
	//start plotting
	INIT_SPI
	ENABLE_SPI_INT
	laser_driver.addr=laser_driver.start_addr;	//copy buffer start address
	SPDR=*laser_driver.addr;	//send out using spi
	laser_driver.addr++;	//increment buffer address
}

//send the next byte over spi
ISR(SPI_vect){
	SPDR=*laser_driver.addr;
	laser_driver.addr++;

	/*	
	asm volatile (
		"L_dl1%=:" "\n\t"
		"ld r2,  %a[addr]+"   "\n\t"	//r2 = *addr; addr++;
		"out %[spi], r2" "\n\t"		//SPDR=r2;

		 : [spi] "M" (_SFR_IO_ADDR (SPDR))	//spi data register
		 : [cnt] "r" (bytestosend), [addr] "e" (addr)	
		 : "r2", "memory"
	   );*/
}

//this interrupt activates the laser  a few µs before it's crossing the detector again
//only active in idle mode
ISR(TIMER0_COMPA_vect)
{
	DISABLE_SPI_INT		//stop sending over SPI
	LASER_ALWAYS_ON		//turn on laser
	//Timer0 off
	STOP_TIMER0
}

void init_timer0()
{
	OCR0A = (laser_driver.buffer_length>>2);	//devide by 4, since Timer0 ist 4 times slower than SPI
	TCCR0A=_BV(WGM01);	//no output change, simple CTC (OCRA clears TCNT)
	TIMSK0=_BV(OCIE0A);	//enable OCR0A interrupt
}

void laser_control_setmode(uint8_t mode)
{
	switch(mode)
	{
		case LASER_MODE_OFF:
			FORCE_SPI_MOSI_LOW	//turn laser off
			DISABLE_SPI_INT
			DISABLE_INT2
			laser_driver.idle = 0;
			laser_driver.mode = LASER_MODE_OFF;
		break;
		case LASER_MODE_INIT:
			//LASER_ALWAYS_ON	//turn laser on
			DISABLE_SPI_INT
			ENABLE_SPI
			DISABLE_INT2
			laser_driver.idle = 0;
			laser_driver.mode = LASER_MODE_INIT;
		break;
		case LASER_MODE_IDLE:
			if(laser_driver.mode == LASER_MODE_INIT||laser_driver.mode == LASER_MODE_RUNNING){
				laser_driver.idle = 1;
				init_timer0();
				ENABLE_INT2
				laser_driver.mode = LASER_MODE_IDLE;
			}
		break;
		case LASER_MODE_RUNNING:
			if(laser_driver.mode == LASER_MODE_INIT||laser_driver.mode == LASER_MODE_IDLE){
				laser_driver.idle = 0;
				init_timer0();
				ENABLE_INT2
				laser_driver.mode = LASER_MODE_RUNNING;
			}
		break;
	}
}

uint8_t laser_control_getmode()
{
	return laser_driver.mode;
}

void laser_control_setaddr(uint8_t *address)
{
	laser_driver.start_addr = address;
}

uint8_t laser_control_readloopcounter(void)
{
	uint8_t loops;
	loops = laser_driver.loops;
	laser_driver.loops = 0;
	return loops;
}

void laser_control_setbufsize(uint16_t buf)
{
	laser_driver.buffer_length = buf;
}
