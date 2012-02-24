/* hardware-dependent & common configuration
 * currently this file only contains configuration data for
 * the 2nd revision hardware.
 * In case you want to adjust things to your reversion, you've
 * just found the right file to do so ;-)
 * Don't forget to edit rfm12_config.h as well.
 */

#include <avr/io.h>
#pragma once
#include "../../common/locodoco.h"

/* set the number of possible entries in the motor
 * stepping table. note that each table entry will consume
 * 2 bytes of system ram & eeprom
 */
#define MOTOR_ADC_STEPS 16

#define MOTOR_STEP_SIZE (8192/MOTOR_ADC_STEPS)
#define MOTOR_STEP_SHIFT (13-4)

/*** DECODER VERSION 2 ***/
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
	#define EEP_MYADDRESS ((const void *) 0x00)
	/* position of speed table */
	#define EEP_STEP_TABLE ((const void *) (EEP_MYADDRESS + sizeof(locodoco_addr_t)))

	/* timer to use for motor pwm (currently only tcnt1 supported,
	 * -> see motor.h
	 */
	#define MOTOR_TIMER 1
#else
	#error "no or unknown hardware revision (HWREF) defined!"
#endif

/* set the number of possible entries in the motor
 * stepping table. note that each table entry will consume
 * 2 bytes of system ram & eeprom
 */
#define MOTOR_ADC_STEPS 16

/*
 don't forget to adjust rfm12_config.h as well (if neccessary)
*/
