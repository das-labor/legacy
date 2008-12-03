#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <avr/io.h>

unsigned char waitForFire;
void joy_init();

#define BITFIRE  PD3
#define BITRIGHT PB3
#define BITLEFT  PB2
#define BITDOWN  PB1
#define BITUP    PB0

#define JOYISFIRE  (!(PIND & (1<<BITFIRE)))
#define JOYISLEFT  (!(PINB & (1<<BITLEFT)))
#define JOYISRIGHT (!(PINB & (1<<BITRIGHT)))
#define JOYISDOWN  (!(PINB & (1<<BITDOWN)))
#define JOYISUP    (!(PINB & (1<<BITUP)))

#endif // JOYSTICK_H
