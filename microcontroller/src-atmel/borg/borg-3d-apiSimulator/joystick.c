#include "joystick.h"

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