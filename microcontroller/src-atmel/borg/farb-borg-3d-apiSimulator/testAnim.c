#include "config.h"
#include "util.h"
#include "api.h"

void testAnim();
void fnordLicht();
void testBlur();
void movingCubes();
	
void *display_loop(void * unused)  {
	while (1) {
		movingCubes();
		testBlur();
		testAnim();
		fnordLicht();
	}
}

#define FAKTOR ((255+2)/5)

void testAnim() {
	unsigned char x, y, z, i;
	for (i = 0; i < 250; i++) {
		for (z = 0; z < MAX_Z; z++) {
			for (y = 0; y < MAX_Y; y++) {
				for (x = 0; x < MAX_X; x++) {
					switch (i % 6) {
						case 0:
							imag[z][y][x][R] = x * FAKTOR;
							imag[z][y][x][G] = y * FAKTOR;
							imag[z][y][x][B] = z * FAKTOR;
							break;
						case 1:
							imag[z][y][x][0] = x * FAKTOR;
							imag[z][y][x][1] = z * FAKTOR;
							imag[z][y][x][2] = y * FAKTOR;
							break;
						case 2:
							imag[z][y][x][0] = y * FAKTOR;
							imag[z][y][x][1] = x * FAKTOR;
							imag[z][y][x][2] = z * FAKTOR;
							break;
						case 3:
							imag[z][y][x][0] = y * FAKTOR;
							imag[z][y][x][1] = z * FAKTOR;
							imag[z][y][x][2] = x * FAKTOR;
							break;
						case 4:
							imag[z][y][x][0] = z * FAKTOR;
							imag[z][y][x][1] = x * FAKTOR;
							imag[z][y][x][2] = y * FAKTOR;
							break;
						case 5:
							imag[z][y][x][0] = z * FAKTOR;
							imag[z][y][x][1] = y * FAKTOR;
							imag[z][y][x][2] = x * FAKTOR;
							break;
					} 
				}
			}
		}
		if (i > 150) 
			swapAndWait(250);
		else 
			fade(10, 50);	
	}
}

void fnordLicht() {
	unsigned char *im, i, j;
	unsigned short k;

	for (j = 0; j < 5; j++) {
		for (i = 0; i < 255; i++) {
			im = (unsigned char *) imag;
			for (k = 0; k < MAX_Z*MAX_Y*MAX_X; k++) {
				*im++ = 63 - i;
				*im++ = i;
				*im++ = 0;
			}
			swapAndWait(20);
		}
		for (i = 0; i < 255; i++) {
			im = (unsigned char *) imag;
			for (k = 0; k < MAX_Z*MAX_Y*MAX_X; k++) {
				*im++;
				*im++ = 63 - i;
				*im++ = i;
			}
			swapAndWait(20);
		}
		for (i = 0; i < 255; i++) {
			im = (unsigned char *) imag;
			for (k = 0; k < MAX_Z*MAX_Y*MAX_X; k++) {
				*im++ = i;
				*im++;
				*im++ = 63 - i;
			}
			swapAndWait(20);
		}
	}
}

static unsigned char filter[3] = {1, 2, 1};

void testBlur() {
	unsigned char  i, j, *im = (unsigned char*) imag;
	unsigned short value;
	
	for (j = 0; j < 40; ) {
		clearScreen(0, 0, 0);
		for (i = 0; i < 10; i++) {
			value = (easyRandom() % 125) * 3;
			im[value]   = easyRandom();
			im[value+1] = easyRandom();
			im[value+2] = easyRandom();
		}
		//blurX(&filter);
		swapAndWait(800);
		//fade(20, 30);
	}
}

void drawCube(voxel pos, unsigned char color) {
	imag[pos.x][pos.y][pos.z][color]     = 255;
	imag[pos.x+1][pos.y][pos.z][color]   = 255;
	imag[pos.x][pos.y+1][pos.z][color]   = 255;
	imag[pos.x+1][pos.y+1][pos.z][color] = 255;
	imag[pos.x][pos.y][pos.z+1][color]     = 255;
	imag[pos.x+1][pos.y][pos.z+1][color]   = 255;
	imag[pos.x][pos.y+1][pos.z+1][color]   = 255;
	imag[pos.x+1][pos.y+1][pos.z+1][color] = 255;
}

void movingCubes() {
	voxel cube1 = {0, 0, 0}, cube2 = {3, 3, 3};	
	while(1) {
		drawCube(cube1, 0);
		drawCube(cube2, 1);
		fade(50, 4);
		clearScreen(0,0,0);
		cube1.x = (cube1.x + 1) % 4;
		cube2.y = (cube2.y - 1) % 4;
		drawCube(cube1, 0);
		drawCube(cube2, 1);
		fade(50, 4);
		clearScreen(0,0,0);
		cube1.x = (cube1.y + 1) % 4;
		cube2.y = (cube2.x + 1) % 4;
		drawCube(cube1, 0);
		drawCube(cube2, 1);
		fade(50, 4);
		clearScreen(0,0,0);
		cube1.z = (cube1.z + 1) % 4;
		cube2.y = (cube2.y - 1) % 4;
		drawCube(cube1, 0);
		drawCube(cube2, 1);
		fade(50, 4);
		clearScreen(0,0,0);
		cube1.x = (cube1.x + 1) % 4;
		cube2.z = (cube2.z + 1) % 4;	}
}

/*

#define SNAKE_LEN 256

void snake3d() {
	pixel3d pixels[SNAKE_LEN]; 
	pixel3d *head = &pixels[1];
	pixel3d *tail = &pixels[0];
	pixel3d old_head;

	pixel3d apples[10];
	unsigned char apple_num = 0;
	pixels[0].x = 1; 
	pixels[0].y = 1;
	pixels[0].z = 0;
	pixels[1].x = 1; 
	pixels[1].y = 2;
	pixels[1].z = 0;
	
	direction dir = forward;

	clear_screen(0);

	unsigned char x = 0, dead = 0;
	while (1) {
		x++;
		old_head = *head;
		if (++head == pixels + SNAKE_LEN) 
			head = pixels;
		
		unsigned char dead_cnt=0;
		
		unsigned char apple_found = 0, j;
		for (j = 0; j < apple_num; j++) {
			unsigned char i;
			for (i = 0; i < 6; i++) {
				if ((next_pixel3d(old_head, (direction)i).x == apples[j].x) && 
					(next_pixel3d(old_head, (direction)i).y == apples[j].y) &&
				    (next_pixel3d(old_head, (direction)i).z == apples[j].z)) {
					apple_found = 1;
					dir = (direction)i; 
					for(; j < apple_num-1; j++) {
						apples[j] = apples[j+1];
					}
					apple_num--;
					goto apple_se;
				}
			}
		}
		apple_se:
		if (!apple_found) {
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
			if (easyRandom() < 80) {
				dir = (direction) (easyRandom() % 6);
			}
			if((apple_num<10) && (easyRandom()<10)) {
				pixel3d new_apple = (pixel3d){easyRandom()%NUM_PLANES,
					                          easyRandom()%NUM_ROWS,
					                          easyRandom()%NUM_COLS};
				if (!get_pixel3d(new_apple)){
					apples[apple_num++] = new_apple;
				}
			}
			if (!apple_found) {
				clearpixel3d(*tail);
				if (++tail == pixels + SNAKE_LEN) tail = pixels;
			}
		} else {
			while (tail != head) {
				clearpixel3d(*tail);
				if ((++tail) > pixels+SNAKE_LEN) 
					tail = pixels;
				wait(60);
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
		wait(10);
	}
}


*/

