#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "../config.h"

extern unsigned char waitForFire;
void joy_init();


#ifdef AVR

#	include <avr/io.h>
#	ifdef NES_PAD_SUPPORT

		extern volatile unsigned char fakeport;

#		define JOYISUP    (!! ((1<<4) & fakeport))
#		define JOYISDOWN  (!! ((1<<5) & fakeport))
#		define JOYISLEFT  (!! ((1<<6) & fakeport))
#		define JOYISRIGHT (!! ((1<<7) & fakeport))
#		define JOYISFIRE  (!! ((1<<0) & fakeport))

#	elif defined(RFM12_JOYSTICK_SUPPORT)

		extern volatile uint8_t rfm12_joystick_val;

#		define JOYISUP    (!! ((1<<0) & rfm12_joystick_val))
#		define JOYISDOWN  (!! ((1<<1) & rfm12_joystick_val))
#		define JOYISLEFT  (!! ((1<<2) & rfm12_joystick_val))
#		define JOYISRIGHT (!! ((1<<3) & rfm12_joystick_val))
#		define JOYISFIRE  (!! ((1<<4) & rfm12_joystick_val))

#	else

#		define JOYISUP    (!(JOYSTICK_PIN_UP    & (1<<JOYSTICK_BIT_UP   )))
#		define JOYISDOWN  (!(JOYSTICK_PIN_DOWN  & (1<<JOYSTICK_BIT_DOWN )))
#		define JOYISLEFT  (!(JOYSTICK_PIN_LEFT  & (1<<JOYSTICK_BIT_LEFT )))
#		define JOYISRIGHT (!(JOYSTICK_PIN_RIGHT & (1<<JOYSTICK_BIT_RIGHT)))
#		define JOYISFIRE  (!(JOYSTICK_PIN_FIRE  & (1<<JOYSTICK_BIT_FIRE )))

#	endif
#else

	extern volatile unsigned char fakeport;

#	define JOYISUP    (0x10 & fakeport)
#	define JOYISDOWN  (0x08 & fakeport)    
#	define JOYISLEFT  (0x02 & fakeport)
#	define JOYISRIGHT (0x04 & fakeport)
#	define JOYISFIRE  (0x01 & fakeport)

#endif

#endif // JOYSTICK_H
