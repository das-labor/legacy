
#include "pixel.h"
#include "util.h"
#include "config.h"
#include "borg_hw.h"

#define BORGSIZE 8

void test1() {
	unsigned char x, y, z;
	clear_screen(0);
	for (z = 0; z < BORGSIZE; z++) {	
		for (y = 0; y < BORGSIZE; y++) {
			for (x = 0; x < BORGSIZE; x++) {
				setpixel3d((pixel3d){x,y,z}, 3);
				mWait(50);
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
		mWait(40);
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
		mWait(40);
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){j,k,i}, 0);
			}
		}
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){k,i,j}, 3);
			}
		}
		mWait(50);
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
		mWait(40);
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){k,j,i}, 0);
			}
		}
	}
}

#define BIT_S(var,b) ((var&(1<<b))?1:0)

unsigned char myrandom(){
	static unsigned int muh = 0xAA;
	unsigned char x;
	for (x=0; x<8; x++) {
		muh = (muh<<1) ^ BIT_S(muh,1) ^ BIT_S(muh,8) ^ BIT_S(muh,9) ^ BIT_S(muh,13) ^ BIT_S(muh,15);
	}
	return (unsigned char) muh;

}

void snake3d(){
	pixel3d pixels[64] = {{1, 1, 0},{1, 2, 0}};
	pixel3d *head = &pixels[1];
	pixel3d *tail = &pixels[0];
	pixel3d old_head;

	pixel3d apples[10];
	unsigned char apple_num = 0;
	
	direction dir = forward;

	clear_screen(0);

	unsigned char x = 0, dead = 0;
	while (1) {
		x++;
		old_head = *head;
		if(++head == pixels + 64) head = pixels;
		
		unsigned char dead_cnt=0;
		
		unsigned char apple_found = 0, j;
		for(j=0;j<apple_num;j++){
			unsigned char i;
			for(i=0;i<6;i++) {
				if ((next_pixel3d(old_head, i).x == apples[j].x) && 
					(next_pixel3d(old_head, i).y == apples[j].y) &&
				    (next_pixel3d(old_head, i).z == apples[j].z)) {
					apple_found = 1;
					dir = i;
					for(; j < apple_num-1; j++){
						apples[j] = apples[j+1];
					}
					apple_num--;
					goto apple_se;
				}
			}
		}
		apple_se:

		if (apple_found){
		
		} else {
			while (get_next_pixel3d(old_head, dir)) {
				if ((dead_cnt++) == 4) {
					dead = 1;
					break;
				}
				dir = direction_r(dir);
			}
		}

		if (!dead) {
			*head = next_pixel3d(old_head, dir);
			setpixel3d(*head, 3);
		
			if (myrandom() < 80) {
				dir = myrandom() % 6;
			}

			if ((apple_num<10) && (myrandom()<10)) {
				pixel3d new_apple = (pixel3d){myrandom()%BORGSIZE,
					                          myrandom()%BORGSIZE,
					                          myrandom()%BORGSIZE};
				if (!get_pixel3d(new_apple)) {
					apples[apple_num++] = new_apple;
				}
			}

			if  (!apple_found) {
				clearpixel3d(*tail);
				if(++tail == pixels + 64) tail = pixels;
			}
		} else {
			while (tail != head) {
				clearpixel3d(*tail);
				if ((++tail)>pixels+64) 
					tail = pixels;
				mWait(60);
			}
			break;
		}
		
		for (j = 0; j < apple_num; j++) {
			if (x % 2) {
				setpixel3d(apples[j], 3);
			} else {
				clearpixel3d(apples[j]);
			}
		}
		mWait(100);
	}

}
