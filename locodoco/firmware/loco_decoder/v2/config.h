/* hardware-dependent configuration
 * currently this file only contains configuration data for
 * the 2nd revision hardware.
 */

#include <avr/io.h>
#pragma once

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
#else
	#error "no or unknown hardware revision (HWREF) defined!"
#endif
