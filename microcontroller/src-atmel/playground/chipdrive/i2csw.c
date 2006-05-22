/*! \file i2csw.c \brief Software I2C interface using port pins. */
//*****************************************************************************
//
// File Name	: 'i2csw.c'
// Title		: Software I2C interface using port pins
// Author		: Pascal Stang
// Created		: 11/22/2000
// Revised		: 5/2/2002
// Version		: 1.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <avr/io.h>
#include "i2csw.h"

// Standard I2C bit rates are:
// 100KHz for slow speed
// 400KHz for high speed

//#define QDEL	delay(5)		// i2c quarter-bit delay
//#define HDEL	delay(10)		// i2c half-bit delay

// i2c quarter-bit delay
//#define QDEL	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
// i2c half-bit delay
//#define HDEL	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");


void delay(){
	u16 x;
	for(x=0;x<2000;x++){
	    asm volatile("nop");
	}   
    
}


#define QDEL delay();
#define HDEL delay();

#define I2C_SDL_LO      sbi( SDADDR, SDA)
#define I2C_SDL_HI      cbi( SDADDR, SDA)

#define I2C_SCL_LO      sbi( SCLDDR, SCL); 
#define I2C_SCL_HI      cbi( SCLDDR, SCL); 

#define I2C_SCL_TOGGLE  HDEL; I2C_SCL_HI; HDEL; I2C_SCL_LO;
#define I2C_START       I2C_SDL_LO; QDEL; I2C_SCL_LO; HDEL; 
#define I2C_STOP        I2C_SDL_LO; HDEL; I2C_SCL_HI; QDEL; I2C_SDL_HI; HDEL;

// defines and constants
#define READ		0x01	// I2C READ bit


void i2cStart(){
	I2C_START;    
}

void i2cStop(){
	I2C_STOP;    
}


s16 i2cPutbyte(u08 b)
{
	u08 i, ack;
	
	for (i=0;i<8;i++)
	{
		if ( b & 0x80 ){
			I2C_SDL_HI;
		}else{
			I2C_SDL_LO;			// address bit
		}
		
		HDEL;
		I2C_SCL_HI;
		HDEL;
		I2C_SCL_LO;
		HDEL;
		b <<= 1;
	}

	I2C_SDL_HI;					// leave SDL HI
	HDEL;
	I2C_SCL_HI;					// clock back up
  	
	HDEL;
	ack = inb(SDAPIN) & (1<<SDA);	// get the ACK bit

	I2C_SCL_LO;					// not really ??
	HDEL;
	return (ack == 0);			// return ACK value
}


u08 i2cGetbyte(u08 last)
{
	u08 i;
	u08 c,b = 0;
    
	HDEL;
	I2C_SDL_HI;
    
    	for(i=0;i<8;i++)
	{
		HDEL;
		I2C_SCL_HI;				// clock HI
	  	HDEL;
	    	c = inb(SDAPIN) & (1<<SDA);  
		b <<= 1;
		if(c) b |= 1;
    		I2C_SCL_LO;				// clock LO
	}
  
	HDEL;
	if (last)
		I2C_SDL_HI;				// set NAK
	else
		I2C_SDL_LO;				// set ACK

	HDEL;
	I2C_SCL_HI;
	HDEL;
	I2C_SCL_LO;

	return b;					// return received byte
}


//************************
//* I2C public functions *
//************************

//! Initialize I2C communication
void i2cInit(void)
{
        I2C_SDL_HI;					// set I/O state and pull-ups
	I2C_SCL_HI;					// set I/O state and pull-ups
}

//! Send a byte sequence on the I2C bus
void i2cSend(u08 device, u08 subAddr, u08 length, u08 *data)
{
	I2C_START;      		// do start transition
	i2cPutbyte(device); 		// send DEVICE address
	i2cPutbyte(subAddr);		// and the subaddress

	// send the data
	while (length--)
		i2cPutbyte(*data++);

	I2C_SDL_LO;					// clear data line and
	I2C_STOP;					// send STOP transition
}

//! Retrieve a byte sequence on the I2C bus
void i2cReceive(u08 device, u08 subAddr, u08 length, u08 *data)
{
	int j = length;
	u08 *p = data;

	I2C_START;					// do start transition
	i2cPutbyte(device);			// send DEVICE address
	i2cPutbyte(subAddr);   		// and the subaddress
	HDEL;
    
	I2C_SCL_HI;      			// do a repeated START
    	//I2C_STOP;
    	HDEL;
    	I2C_START;					// transition

	i2cPutbyte(device | READ);	// resend DEVICE, with READ bit set

	// receive data bytes
	while (j--)
		*p++ = i2cGetbyte(j == 0);

	I2C_SDL_LO;					// clear data line and
	I2C_STOP;					// send STOP transition
}
