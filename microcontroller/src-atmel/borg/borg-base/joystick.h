#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <avr/io.h>

unsigned char waitForFire;
inline void joy_init();

//#define BITFIRE  PD0
//#define BITUP    PB0  
//#define BITDOWN  PB1
//#define BITLEFT  PB2
//#define BITRIGHT PB3   

#define JOYISFIRE  (!(PIND & (1<<BITFIRE)))
#define JOYISLEFT  (!(PINB & (1<<BITLEFT)))
#define JOYISRIGHT (!(PINB & (1<<BITRIGHT)))
#define JOYISDOWN  (!(PINB & (1<<BITDOWN)))    
#define JOYISUP    (!(PINB & (1<<BITUP)))

#endif // JOYSTICK_H
