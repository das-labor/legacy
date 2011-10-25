/* hardware-dependent configuration
 * currently this file only contains configuration data for
 * the 2nd revision hardware.
 */

#include <avr/io.h>
#pragma once

/*** DECODER VERSION 2
 ***
 ***/
#if HWREF == 2
	#define PIN_MOTOR_EN 2
	#define PORT_MOTOR_EN PORTB
	#define DDR_MOTOR_EN DDRB

	#define PIN_MOTOR_FW 0
	#define PORT_MOTOR_FW PORTB
	#define DDR_MOTOR_FW DDRB
	
	#define PIN_MOTOR_RW 7
	#define PORT_MOTOR_RW PORTD
	#define DDR_MOTOR_RW DDRD
	
	/* eeprom address of our locodoco addr
	 */
	#define EEP_MYADDRESS 0x00
#else
	#error "no or unknown hardware revision (HWREF) defined!"
#endif


/*
 don't forget to adjust rfm12_config.h as well (if neccessary)
*/
