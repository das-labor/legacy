#ifndef JOYSTICK_H_
#define JOYSTICK_H_

extern unsigned char fakeport;

#define JOYISFIRE  (0x01 & fakeport)
#define JOYISLEFT  (0x02 & fakeport)
#define JOYISRIGHT (0x04 & fakeport)
#define JOYISDOWN  (0x08 & fakeport)    
#define JOYISUP    (0x10 & fakeport)

unsigned char waitForFire;


#endif /*JOYSTICK_H_*/
