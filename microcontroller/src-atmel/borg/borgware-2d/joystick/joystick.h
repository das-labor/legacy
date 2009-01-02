#ifndef JOYSTICK_H
#define JOYSTICK_H

extern unsigned char waitForFire;
void joy_init();

#ifdef AVR

#include <avr/io.h>

#define JOYISUP    (!(JOYSTICK_PIN_UP    & (1<<JOYSTICK_BIT_UP   )))
#define JOYISDOWN  (!(JOYSTICK_PIN_DOWN  & (1<<JOYSTICK_BIT_DOWN )))
#define JOYISLEFT  (!(JOYSTICK_PIN_LEFT  & (1<<JOYSTICK_BIT_LEFT )))
#define JOYISRIGHT (!(JOYSTICK_PIN_RIGHT & (1<<JOYSTICK_BIT_RIGHT)))
#define JOYISFIRE  (!(JOYSTICK_PIN_FIRE  & (1<<JOYSTICK_BIT_FIRE )))

#else

extern unsigned char fakeport;

#define JOYISUP    (0x10 & fakeport)
#define JOYISDOWN  (0x08 & fakeport)    
#define JOYISLEFT  (0x02 & fakeport)
#define JOYISRIGHT (0x04 & fakeport)
#define JOYISFIRE  (0x01 & fakeport)

#endif

#endif // JOYSTICK_H
