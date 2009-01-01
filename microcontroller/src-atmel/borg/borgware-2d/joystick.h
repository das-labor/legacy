#ifndef JOYSTICK_H
#define JOYSTICK_H

extern unsigned char waitForFire;
void joy_init();

#ifdef AVR

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

#else

extern unsigned char fakeport;

#define JOYISFIRE  (0x01 & fakeport)
#define JOYISLEFT  (0x02 & fakeport)
#define JOYISRIGHT (0x04 & fakeport)
#define JOYISDOWN  (0x08 & fakeport)    
#define JOYISUP    (0x10 & fakeport)

#endif

#endif // JOYSTICK_H
