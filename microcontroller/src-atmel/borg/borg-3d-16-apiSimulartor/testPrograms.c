#include "programs.h"

void test1() {
	unsigned char x, y, z;
	clear_screen(0);
	for (z = 0; z < LEN_Z; z++) {	
		for (y = 0; y < LEN_Y; y++) {
			for (x = 0; x < LEN_X; x++) {
				setpixel3d((pixel3d){x,y,z}, 3);
				wait(25);
			}
		}
	}
}

void test2() {
	unsigned char i, j, k;
	for (i = 0; i < LEN_Z; i++) {
		for (j = 0; j < LEN_Y; j++) {
			for (k = 0; k < LEN_X; k++) {
				setpixel3d((pixel3d){i,j,k}, 3);
			}
		}
		wait(40);
		for (j = 0; j < LEN_Y; j++) {
			for (k = 0; k < LEN_X; k++) {
				setpixel3d((pixel3d){i,j,k}, 0);
			}
		}
	}
	for (i = 0; i < LEN_Z; i++) {
		for (j = 0; j < LEN_Y; j++) {
			for (k = 0; k < LEN_X; k++) {
				setpixel3d((pixel3d){j,k,i}, 3);
			}
		}
		wait(40);
		for (j = 0; j < LEN_Y; j++) {
			for (k = 0; k < LEN_X; k++) {
				setpixel3d((pixel3d){j,k,i}, 0);
			}
		}
	}
	for (i = 0; i < LEN_Z; i++) {
		for (j = 0; j < LEN_Y; j++) {
			for (k = 0; k < LEN_X; k++) {//sets blah
				setpixel3d((pixel3d){k,i,j}, 3);
			}
		}
		wait(50);
		for (j = 0; j < LEN_Y; j++) {
			for (k = 0; k < LEN_X; k++) {
				setpixel3d((pixel3d){k,i,j}, 0);
			}
		}
	}
	for (i = LEN_X-1; i < LEN_X; i--) {
		for (j = 0; j < LEN_Y; j++) {
			for (k = 0; k < LEN_Z; k++) {
				setpixel3d((pixel3d){k,j,i}, 3);
			}
		}
		wait(40);
		for (j = 0; j < LEN_Y; j++) {
			for (k = 0; k < LEN_Z; k++) {
				setpixel3d((pixel3d){k,j,i}, 0);
			}
		}
	}
}


void test3() {
	unsigned char i, j, k;
	for (i = 0; i < LEN_X; i++) {
		for (j = 0; j < LEN_Y; j++) {
			for (k = 0; k < LEN_Z; k++) {
				setpixel3d((pixel3d){i,j,k}, 3);
			}
		}
		wait(40);
	}
	for (i = 0; i < LEN_X; i++) {
		for (j = 0; j < LEN_Y; j++) {
			for (k = 0; k < LEN_Z; k++) {
				setpixel3d((pixel3d){i,j,k}, 0);
			}
		}
		wait(40);
	}
	for (i = 0; i < LEN_Z; i++) {
		for (j = 0; j < LEN_X; j++) {
			for (k = 0; k < LEN_Y; k++) {
				setpixel3d((pixel3d){j,k,i}, 3);
			}
		}
		wait(40);
	}
	for (i = 0; i < LEN_Z; i++) {
		for (j = 0; j < LEN_Y; j++) {
			for (k = 0; k < LEN_X; k++) {
				setpixel3d((pixel3d){j,k,i}, 0);
			}
		}
		wait(40);
	}
	for (i = 0; i < LEN_Y; i++) {
		for (j = 0; j < LEN_Z; j++) {
			for (k = 0; k < LEN_X; k++) {
				setpixel3d((pixel3d){k,i,j}, 3);
			}
		}
		wait(50);
	}
	for (i = 0; i < LEN_Y; i++) {
		for (j = 0; j < LEN_Z; j++) {
			for (k = 0; k < LEN_X; k++) {
				setpixel3d((pixel3d){k,i,j}, 0);
			}
		}
		wait(50);
	}
	for (i = LEN_Z - 1; i < LEN_Z; i--) {
		for (j = 0; j < LEN_Y; j++) {
			for (k = 0; k < LEN_X; k++) {
				setpixel3d((pixel3d){k,j,i}, 3);
			}
		}
		wait(40);
	}
	for (i = LEN_Z-1; i < LEN_Z; i--) {
		for (j = 0; j < LEN_Y; j++) {
			for (k = 0; k < LEN_X; k++) {
				setpixel3d((pixel3d){k,j,i}, 0);
			}
		}
		wait(40);
	}
}
/*
void joystick_test() {
	clear_screen(0);
	setpixel3d((pixel3d) {5,5,5}, 3);
	while (1) {
		wait(1);
		JOYUSE0();
		if (JOYISUP) 
			setpixel3d((pixel3d){0,0,1}, 3); 
		else 
			setpixel3d((pixel3d){0,0,1}, 0);
		if (JOYISDOWN) 
			setpixel3d((pixel3d){0,2,1}, 3); 
		else 
			setpixel3d((pixel3d){0,2,1}, 0);
		if (JOYISLEFT) 
			setpixel3d((pixel3d){0,1,0}, 3); 
		else 
			setpixel3d((pixel3d){0,1,0}, 0);
		if (JOYISRIGHT) 
			setpixel3d((pixel3d){0,1,2}, 3); 
		else 
			setpixel3d((pixel3d){0,1,2}, 0);
		if (JOYISFIRE) 
			setpixel3d((pixel3d){0,1,1}, 3); 
		else 
			setpixel3d((pixel3d){0,1,1}, 0);
		
		JOYUSE1();
		if (JOYISUP) 
			setpixel3d((pixel3d){1,0,1}, 3); 
		else 
			setpixel3d((pixel3d){1,0,1}, 0);
		if (JOYISDOWN) 
			setpixel3d((pixel3d){1,2,1}, 3);
		else 
			setpixel3d((pixel3d){1,2,1}, 0);
		if (JOYISLEFT) 
			setpixel3d((pixel3d){1,1,0}, 3); 
		else 
			setpixel3d((pixel3d){1,1,0}, 0);
		if (JOYISRIGHT) 
			setpixel3d((pixel3d){1,1,2}, 3); 
		else 
			setpixel3d((pixel3d){1,1,2}, 0);
		if (JOYISFIRE) 
			setpixel3d((pixel3d){1,1,1}, 3); 
		else 
			setpixel3d((pixel3d){1,1,1}, 0);
	}
}
*/
void shift_test() {
	clear_screen(0);
	setpixel3d((pixel3d) {4,4,4}, 3);
	setpixel3d((pixel3d) {4,4,5}, 3);
	setpixel3d((pixel3d) {4,5,4}, 3);
	setpixel3d((pixel3d) {4,5,5}, 3);
	setpixel3d((pixel3d) {5,4,4}, 3);
	setpixel3d((pixel3d) {5,4,5}, 3);
	setpixel3d((pixel3d) {5,5,4}, 3);
	setpixel3d((pixel3d) {5,5,5}, 3);
	wait(2000);
	shift3d(up);
	wait(2000);
	shift3d(down);
	wait(2000);
	shift3d(right);
	wait(2000);
	shift3d(left);
	wait(2000);
	shift3d(forward);
	wait(2000);
	shift3d(back);
}

void setplane_test() {
	clear_screen(0);
	set_plane(up, 0, 3);
	wait(1000);
	set_plane(down, 0, 3);
	wait(1000);
	set_plane(forward, 0, 3);
	wait(1000);
	set_plane(right, 0, 3);
	wait(1000);
	set_plane(back, 0, 3);
	wait(1000);
	set_plane(left, 0, 3);
	wait(1000);
}

