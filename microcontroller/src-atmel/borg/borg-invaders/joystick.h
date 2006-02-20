#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <avr/io.h>

extern unsigned char waitForFire;
inline void joy_init();

#define PORTJOYDIRS PORTB
#define DDRJOYDIRS  DDRB
#define PINJOYDIRS  PINB

#define PORTJOYFIRE PORTD
#define DDRJOYFIRE  DDRD
#define PINJOYFIRE  PIND

#define BITUP    PB3
#define BITDOWN  PB2
#define BITLEFT  PB1
#define BITRIGHT PB0
#define BITFIRE  PD3

#define JOYISUP    (!(PINJOYDIRS & (1<<BITUP)))
#define JOYISDOWN  (!(PINJOYDIRS & (1<<BITDOWN)))
#define JOYISLEFT  (!(PINJOYDIRS & (1<<BITLEFT)))
#define JOYISRIGHT (!(PINJOYDIRS & (1<<BITRIGHT)))
#define JOYISFIRE  (!(PINJOYDIRS & (1<<BITFIRE)))

#endif // JOYSTICK_H
