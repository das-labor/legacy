#ifndef JOYSTICK_H
#define JOYSTICK_H

extern char joy1_up, joy1_down, joy1_right , joy1_left, joy_en0;

#define JOYUSE0() joy_en0 = 1
#define JOYUSE1() joy_en0 = 0

int joyisup() {
	if (!joy_en0 || !joy1_up)
		return 0;
	joy1_up = 0;
	return 1;
}

int joyisdown() {
	if (!joy_en0 || !joy1_down)
		return 0;
	joy1_down = 0;
	return 1;
}	

int joyisright() {
	if (!joy_en0 || !joy1_right)
		return 0;
	joy1_right = 0;
	return 1;
}	

int joyisleft() {
	if (!joy_en0 || !joy1_left)
		return 0;
	joy1_left = 0;
	return 1;
}	

#define JOYISUP joyisup()
#define JOYISDOWN joyisdown()
#define JOYISLEFT joyisleft()
#define JOYISRIGHT joyisright()
#define JOYISFIRE 0

#endif // JOYSTICK_H
