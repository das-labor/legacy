#include <avr/io.h>
#include "config.h"
#pragma once

#define MOTOR_FW() \
	PORT_MOTOR_RW &= ~(1<<PIN_MOTOR_RW); \
	PORT_MOTOR_FW |= (1<<PIN_MOTOR_FW)

#define MOTOR_RW() \
	PORT_MOTOR_FW &= ~(1<<PIN_MOTOR_FW); \
	PORT_MOTOR_RW |= (1<<PIN_MOTOR_RW)

#define MOTOR_OFF() \
	PORT_MOTOR_EN &= ~(1<<PIN_MOTOR_EN)

#define MOTOR_ON() \
	PORT_MOTOR_EN |= (1<<PIN_MOTOR_EN)

/* initialize
 */
inline void motor_init(void);

/* stop the motor, eventually disable other
 * hardware to save some power
 */
inline void motor_sleep(void);

inline void motor_awake(void);
