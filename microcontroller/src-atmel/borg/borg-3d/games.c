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
            setpixel3d((pixel3d) {(pos128x+8)/16+i, posy, (pos128z+8)/16+j}, 3);
        }
    }
}

typedef struct {
	signed int x, y, z;
} ball;

#define LEN_Y 8
#define INIT_DIR {28, 56, 14} 
#define RES 128
#define RES2 (RES/3)
#define RES3 ((RES*3)/4)
#define AGRESSIVENES 40 // lower <=> agressiver
void pong() {
    unsigned char posx0 = 64, posz0 = 64, posx1 = 64, posz1 = 64;
	unsigned char lives0 = 5, lives1 = 5, score0 = 0, score1 = 0;
	unsigned char counter = 8, joy0 = 0, joy1 = 0, ballV, i;
	ball ballPos128 = {4*128, 2*128, 4*128};
	pixel3d helpDir, ballDir = INIT_DIR, ballPos;
	waitForFire = 0;
	clear_screen(0);
	
	while (lives0 && lives1) {
		if (!joy0) {
			JOYUSE0();
			JOYUSE0();
			if (JOYISFIRE) {
				joy0 = 1;
				lives0 = lives1;
			}
		}
		if (!joy1) {
			JOYUSE1();
			JOYUSE1();
			if (JOYISFIRE) {
				joy1 = 1;
				lives1 = lives0;
			}
		}	
		if((!joy0) && (!joy1))
			break;//no player, so there was a glitch on the fire line.
		ballV = 6 + (score0 > score1 ? score0 : score1)/3;
		if (!--counter) {
			ballPos.x = (ballPos128.x + 64) / 128;
			ballPos.y = (ballPos128.y + 64) / 128;
			ballPos.z = (ballPos128.z + 64) / 128;
			if (ballPos128.y >= ((LEN_Y-1)*128-RES2) && !joy0) {
				if (ballPos128.y >= (LEN_Y-1)*128)
					ballPos128.y = (LEN_Y-1)*128 - RES2;
				ballDir.y = (char)-ballDir.y;
			}
			if (ballPos128.y >= ((LEN_Y-1)*128-RES3) && joy0) {
				if (ballPos128.y >= (LEN_Y-1)*128)
					ballPos128.y = (LEN_Y-1)*128 - RES2;
				if ((ballPos128.x+4)/8 >= posx0 - 8 && (ballPos128.x+4)/8 < posx0+56  && 
					(ballPos128.z+4)/8 >= posz0 - 8 && (ballPos128.z+4)/8 < posz0+56) {
					rotate((8-(ballPos128.z+4)/8 - ((char)posz1+8))/AGRESSIVENES, 0, 
						   (8-(ballPos128.x+4)/8 - ((char)posx1+8))/AGRESSIVENES,
						   &ballDir, &helpDir, 1, (pixel3d) {0, 0, 0});
					ballDir = helpDir;						   
					ballDir.y = (char) -ballDir.y;
					score0++;
				} else {
					if (--lives0) {
					    for (i = lives0+1; i; --i) {
							set_plane(right, 0, 3);
							wait(50);
							set_plane(right, 0, 0);
							wait(110);
						}
						ballPos128 = (ball) {4*128, 2*128, 4*128};
						ballDir = (pixel3d) INIT_DIR;
						wait(800);
					} else {
						set_plane(right, 0, 3);
						wait(200);
						set_plane(right, 0, 0);
						wait(100);
						set_plane(right, 0, 3);
						wait(200);
						set_plane(right, 0, 0);
						wait(100);
					}
				}
			}
			if (ballPos128.y <= RES2 && !joy1) {
				if (ballPos128.y < 0) 
					ballPos128.y = RES2;
				ballDir.y = (char)-ballDir.y;
			}
			if (ballPos128.y <= RES3 && joy1) {
				if (ballPos128.y < 0) 
					ballPos128.y = RES2;
				if ((ballPos128.x+4)/8 >= posx1 - 8	&& (ballPos128.x+4)/8 < posx1+56 && 
					(ballPos128.z+4)/8 >= posz1 - 8 && (ballPos128.z+4)/8 < posz1+56) {			
					rotate((8-(ballPos128.z+4)/8 - ((char)posz1+8))/AGRESSIVENES, 0, 
						   (8-(ballPos128.x+4)/8 - ((char)posx1+8))/AGRESSIVENES,
						   &ballDir, &helpDir, 1, (pixel3d) {0, 0, 0});
					ballDir = helpDir;
				    ballDir.y = (char) -ballDir.y;
					score1++;
				} else {
					if (--lives1) {
					    for (i = lives1+1; i; --i) {
							set_plane(left, 0, 3);
							wait(50);
							set_plane(left, 0, 0);
							wait(110);
						}
						ballPos128 = (ball) {4*128, 2*128, 4*128};
						ballDir = (pixel3d) INIT_DIR;
						wait(800);
					} else {
						set_plane(left, 0, 3);
						wait(200);
						set_plane(left, 0, 0);
						wait(100);
						set_plane(left, 0, 3);
						wait(200);
						set_plane(left, 0, 0);
						wait(100);
					}
				}
			}
			if (ballPos128.x <= RES2) { 
				ballDir.x = (char) -ballDir.x;
				if (ballPos128.x < 0)
					ballPos128.x = RES2;
			} else if (ballPos128.x >= ((LEN_Y-1)*128 - RES2)) {
				ballDir.x = (char) -ballDir.x;
				if (ballPos128.x >= (LEN_Y-1)*128)
					ballPos128.x = (LEN_Y-1)*128 - RES2;
			}
			
			if (ballPos128.z <= RES2) { 
				ballDir.z = (char) -ballDir.z;
				if (ballPos128.z < 0)
					ballPos128.z = RES2;
			} else if (ballPos128.z >= ((LEN_Y-1)*128 - RES2)) {
				ballDir.z = (char) -ballDir.z;
				if (ballPos128.z >= (LEN_Y-1)*128)
					ballPos128.z = (LEN_Y-1)*128 - RES2;
			}
		}	
		
		//if (ballblink > 4) {
			setpixel3d(ballPos, 3);
		//}

		//if (++ballblink > 15) {
		//	ballblink = 0;
		//}
			
		if (joy0) {
			JOYUSE0();
			JOYUSE0();
			
			if (JOYISUP) {
			   if (posz0 < 80) posz0++;
			}
			if (JOYISDOWN) {
			   if (posz0 > 0) posz0--;
			}             
			if (JOYISLEFT) {
				if (posx0 < 80) posx0++;
			}
			if (JOYISRIGHT) {
				if (posx0 > 0) posx0--;
			}
        	
			set_plane(right, 0, 0);
			drawPanel(LEN_Y-1, posx0, posz0);
		}
		if (joy1) {
			JOYUSE1();
			JOYUSE1();
		
			if (JOYISUP) {
			   if (posz1 < 80) posz1++;
			}
			if (JOYISDOWN) {
			   if (posz1 > 0) posz1--;
			}             
			if (JOYISRIGHT) {
			   if (posx1 < 80) posx1++; 
			}
			if (JOYISLEFT) {
			   if (posx1 > 0) posx1--;
			}
			set_plane(left, 0, 0);
			drawPanel(0, posx1, posz1);
		}
		
        wait(6);
		if (!counter) {
			/*printf("%d %d %d pos %d %d %d dir %d %d %d addDirV %d %d %d v %d\n", 
					ballPos.x, ballPos.y, ballPos.z, ballPos128.x, ballPos128.y, ballPos128.z, 
					ballDir.x, ballDir.y, ballDir.z, ((char)ballDir.x * ballV)/4,
					((char)ballDir.y * ballV)/4, ((char)ballDir.z * ballV)/4, ballV);*/
			ballPos128.x += ((char)ballDir.x * ballV)/4;
			ballPos128.y += ((char)ballDir.y * ballV)/4;
			ballPos128.z += ((char)ballDir.z * ballV)/4;
			counter = 24;
		}
		clearpixel3d(ballPos);
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
