#include "pixel.h"
#include "util.h"
#include "config.h"
#include "borg_hw.h"
#include "uart.h"
#include "joystick.h"
#include "scrolltext.h"
#include "programs.h"

#include <avr/io.h>

void drawPanel(unsigned char posy, unsigned char pos128x, unsigned char pos128z) {
	unsigned char i, j;
	for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            setpixel3d((pixel3d) {(pos128x+8)/16+i, posy,  (pos128z+8)/16+j}, 3);
        }
    }
}


#define LEN_Y 8

void pong() {
    unsigned char posx1 = 64, posz1 = 64, posx2 = 64, posz2 = 64, lives = 5, ballblink = 0;
	unsigned char counter = 8, joy0 = 0, joy1 = 0;
	pixel3d ballPos128 = {4*16, 4*16, 4*16}, ballPos, ballPosOld, ballDir = {2, 4, 1};
	waitForFire = 0;
	clear_screen(0);

	
	while (lives) {
		if (!joy0) {
			JOYUSE0();
			JOYUSE0();
			if (JOYISFIRE) {
				joy0 = 1;
			}
		}
		if (!joy1) {
			JOYUSE1();
			JOYUSE1();
			if (JOYISFIRE) {
				joy1 = 1;
			}
		}	
		
		
		if (!--counter) {
			ballPos.x = ballPos128.x / 16;
			ballPos.y = ballPos128.y / 16;
			ballPos.z = ballPos128.z / 16;
			if (ballPos128.y >= (LEN_Y*16-9) && !joy0) {
				ballDir.y = - ballDir.y;
			}
			if (ballPos128.y >= (LEN_Y*16-17) && joy0) {
				if (ballPos128.x >= posx1 && ballPos128.x < posx1+48 && 
					ballPos128.z >= posz1 && ballPos128.z < posz1+48) {
	
					ballDir.x += ballPos.x - (posx1+8)/16 + 1;					
					ballDir.z += ballPos.z - (posz1+8)/16 + 1;
						
					ballDir.y = - ballDir.y;
				} else {
					if (--lives) {
						set_plane(right, 0, 3);
		
						wait(30);
						set_plane(right, 0, 0);
						wait(30);
						set_plane(right, 0, 3);
						wait(20);
						set_plane(right, 0, 2);
						wait(20);
						set_plane(right, 0, 1);
						wait(20);
						set_plane(right, 0, 0);
						ballPos128 = (pixel3d) 	{4*16, 4*16, 4*16};
						ballDir = (pixel3d) {2, 4, 1};
						wait(800);
					} else {
						set_plane(right, 0, 3);
						wait(100);
						set_plane(right, 0, 0);
						wait(100);
						set_plane(right, 0, 3);
						wait(100);
						set_plane(right, 0, 0);
						wait(100);
					}
				}
			}
			if (ballPos128.y <= 8 && !joy1) 
				ballDir.y = - ballDir.y;
			if (ballPos128.y <= 16 && joy1) {
				if (ballPos128.x >= posx2 && ballPos128.x < posx2+48 && 
					ballPos128.z >= posz2 && ballPos128.z < posz2+48) {
	
					ballDir.x += ballPos.x - (posx2+8)/16 + 1;					
					ballDir.z += ballPos.z - (posz2+8)/16 + 1;
						
					ballDir.y = - ballDir.y;
				} else {
					if (--lives) {
						set_plane(left, 0, 3);
						wait(30);
						set_plane(left, 0, 0);
						wait(30);
						set_plane(left, 0, 3);
						wait(20);
						set_plane(left, 0, 2);
						wait(20);
						set_plane(left, 0, 1);
						wait(20);
						set_plane(left, 0, 0);
						ballPos128 = (pixel3d) 	{3*16, 4*16, 5*16};
						ballDir = (pixel3d) {4, 2, 1};
						wait(800);
					} else {
						set_plane(left, 0, 3);
						wait(100);
						set_plane(left, 0, 0);
						wait(100);
						set_plane(left, 0, 3);
						wait(100);
						set_plane(left, 0, 0);
						wait(100);
					}
				}
			}
			if (ballPos128.x <= 8 || ballPos128.x >= (LEN_Y*16-9) )
				ballDir.x = - ballDir.x;
			if (ballPos128.z <=8 || ballPos128.z >= (LEN_Y*16-9) )
				ballDir.z = - ballDir.z;
		
		}	
		
		
		if(ballblink > 4){
			setpixel3d(ballPos, 3);
		}

		if(++ballblink>15){
			ballblink = 0;
		}
			
		if (joy0) {
			JOYUSE0();
			JOYUSE0();
			
			if (JOYISUP) {
			   if (posz1 < 80) posz1++;
			}
			if (JOYISDOWN) {
			   if (posz1 > 0) posz1--;
			}             
			if (JOYISLEFT) {
				if (posx1 < 80) posx1++;
			}
			if (JOYISRIGHT) {
				if (posx1 > 0) posx1--;
			}
        	
			set_plane(right, 0, 0);
			drawPanel(LEN_Y-1, posx1, posz1);
		}
		if (joy1) {
			JOYUSE1();
			JOYUSE1();
		
			if (JOYISUP) {
			   if (posz2 < 80) posz2++;
			}
			if (JOYISDOWN) {
			   if (posz2 > 0) posz2--;
			}             
			if (JOYISRIGHT) {
			   if (posx2 < 80) posx2++; 
			}
			if (JOYISLEFT) {
			   if (posx2 > 0) posx2--;
			}
        	
			set_plane(left, 0, 0);
			drawPanel(0, posx2, posz2);
		}
		
        	wait(5);
		if (!counter) {
			ballPos128.x += ballDir.x;
			ballPos128.y += ballDir.y;
			ballPos128.z += ballDir.z;
			counter = 8;
		}
		clearpixel3d(ballPos);
		ballPosOld = ballPos;
	}
	waitForFire = 1;
}

/*
#define JOYUP		PD0	
#define JOYDOWN		PD1
#define JOYRIGHT	PD2
#define JOYLEFT		PD3
#define JOYFIRE		PD4
#define JOYPIN	 	PIND
#define JOYPORT	 	PORTD
#define JOYDDR  	DDRD

void snake3dJoystick(){
	pixel3d pixels[64];
	pixel3d *head = &pixels[1];
	pixel3d *tail = &pixels[0];
	pixel3d old_head;
	
	JOYPORT = 0xFF;
	JOYDDR = 0x00;
	

	pixel3d apples[10];
	unsigned char apple_num = 0, tmp, j;
	unsigned char x = 0, dead = 0;
	unsigned char apple_found = 0;

	pixels[0].x = 1; 
	pixels[0].y = 1;
	pixels[0].z = 0;
	pixels[1].x = 1; 
	pixels[1].y = 2;
	pixels[1].z = 0;

	direction dir = forward;

	clear_screen(0);

	while (1) {
		x++;
		old_head = *head;
		if (++head == pixels + 64) 
			head = pixels;
		apple_found = 0;

		tmp = ~JOYPIN;
		if (tmp & shl_table[JOYUP]) {
			dir = turn_up(dir);
		} else if (tmp & shl_table[JOYDOWN]) {
			dir = turn_down(dir);
		} else if (tmp & shl_table[JOYRIGHT]) {
			dir = turn_left(dir);
		} else if (tmp & shl_table[JOYLEFT]) {
			dir = turn_right(dir);
		}
		if (!dead) {
			*head = next_pixel3d(old_head, dir);
			setpixel3d(*head, 3);
			for(j = 0; j < apple_num; j++) {
				if ((next_pixel3d(old_head, dir).x == apples[j].x) && 
					(next_pixel3d(old_head, dir).y == apples[j].y) &&
					(next_pixel3d(old_head, dir).z == apples[j].z)) {
					apple_found = 1;
				}
			}

			if ((apple_num<10) && (myrandom()<10)) {
				pixel3d new_apple = (pixel3d){myrandom()%NUM_PLANES,
					                          myrandom()%NUM_ROWS,
					                          myrandom()%NUM_COLS};
				if (!get_pixel3d(new_apple)){
					apples[apple_num++] = new_apple;
				}
			}
			if (!apple_found){
				clearpixel3d(*tail);
				if (++tail == pixels + 64) tail = pixels;
			}
		} else {
			while (tail != head){
				clearpixel3d(*tail);
				if ((++tail)>pixels+64) tail = pixels;
				wait(60);
			}
			break;
		}
		for (j = 0; j < apple_num; j++) {
			if (x % 2){
				setpixel3d(apples[j], 3);
			} else {
				clearpixel3d(apples[j]);
			}
		}
		wait(200);
	}
}
*/
