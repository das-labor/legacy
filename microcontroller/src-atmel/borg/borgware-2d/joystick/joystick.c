
#include "../config.h"
#include "../makros.h"
#include "joystick.h"
#include <avr/io.h>

inline void joy_init(){

	PORT_FROM_PIN(JOYSTICK_PIN_UP   ) |= (1<<JOYSTICK_BIT_UP   );
	PORT_FROM_PIN(JOYSTICK_PIN_DOWN ) |= (1<<JOYSTICK_BIT_DOWN );
	PORT_FROM_PIN(JOYSTICK_PIN_LEFT ) |= (1<<JOYSTICK_BIT_LEFT );
	PORT_FROM_PIN(JOYSTICK_PIN_RIGHT) |= (1<<JOYSTICK_BIT_RIGHT);
	PORT_FROM_PIN(JOYSTICK_PIN_FIRE ) |= (1<<JOYSTICK_BIT_FIRE );
}
