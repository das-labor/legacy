#ifndef JOYSTICK_H
#define JOYSTICK_H

extern char joy1_up, joy1_down, joy1_right , joy1_left, joy_en0;

#define JOYUSE0() joy_en0 = 1
#define JOYUSE1() joy_en0 = 0

int joyisup();
int joyisdown();
int joyisleft();
int joyisright();

#define JOYISUP joyisup()
#define JOYISDOWN joyisdown()
#define JOYISLEFT joyisleft()
#define JOYISRIGHT joyisright()
#define JOYISFIRE joy_en0

#endif // JOYSTICK_H
