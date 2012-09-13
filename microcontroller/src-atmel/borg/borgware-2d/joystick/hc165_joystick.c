#include <avr/io.h>
#include "../makros.h"
#include "../config.h"

#define		HC165_JOYSTICK_DDR_LOAD DDR(HC165_JOYSTICK_PORT_LOAD)
volatile volatile uint8_t hc165_joystick_val;  
// unsigned char waitForFire;

void joy_init(){
	HC165_JOYSTICK_PORT_LOAD |= (1<<HC165_JOYSTICK_BIT_LOAD);
	HC165_JOYSTICK_DDR_LOAD |= (1<<HC165_JOYSTICK_BIT_LOAD);
}


