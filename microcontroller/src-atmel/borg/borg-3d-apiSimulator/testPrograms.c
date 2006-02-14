#ifdef AVR
#  include "config.h"
#  include "borg_hw.h"
#  include "uart.h"
#  include "pixel.h"
#  include "scrolltext.h"
#  include <avr/io.h>
#else
#  include "pixel3d.h"
#  include "scrolltext2.h"
#endif

#include "util.h"
#include "joystick.h"
#include "programs.h"

void test1() {
	unsigned char x, y, z;
	clear_screen(0);
	for (z = 0; z < NUM_COLS; z++) {	
		for (y = 0; y < NUM_ROWS; y++) {
			for (x = 0; x < NUM_PLANES; x++) {
				setpixel3d((pixel3d){x,y,z}, 3);
				wait(15);
			}
		}
	}
}

void test2() {
	unsigned char i, j, k;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){i,j,k}, 3);
			}
		}
		wait(40);
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){i,j,k}, 0);
			}
		}
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){j,k,i}, 3);
			}
		}
		wait(40);
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){j,k,i}, 0);
			}
		}
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {//sets blah
				setpixel3d((pixel3d){k,i,j}, 3);
			}
		}
		wait(50);
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){k,i,j}, 0);
			}
		}
	}
	for (i = 7; i < 8; i--) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){k,j,i}, 3);
			}
		}
		wait(40);
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){k,j,i}, 0);
			}
		}
	}
}


void test3() {
	unsigned char i, j, k;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){i,j,k}, 3);
			}
		}
		wait(40);
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){i,j,k}, 0);
			}
		}
		wait(40);
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){j,k,i}, 3);
			}
		}
		wait(40);
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){j,k,i}, 0);
			}
		}
		wait(40);
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){k,i,j}, 3);
			}
		}
		wait(50);
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){k,i,j}, 0);
			}
		}
		wait(50);
	}
	for (i = 7; i < 8; i--) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){k,j,i}, 3);
			}
		}
		wait(40);
	}
	for (i = 7; i < 8; i--) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){k,j,i}, 0);
			}
		}
		wait(40);
	}
}

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

void setplane_test() {
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
