#include "config.h"
#include "util.h"
#include "api.h"

// Prototyen
void test1();
void testAnim();
void fnordLicht();
void symetricRandom();
void movingCubes();
void snake();

// Playlist
void *display_loop(void * unused)  {
	while (1) {
		//test1();
		snake(); 
		movingCubes();
		symetricRandom();
		testAnim();
		fnordLicht();
	}
}

#define FAKTOR (255/5) 

void test1() {
	unsigned char x, y, z, c;
	for (z = 0; z < MAX_Z; z++) {
		for (y = 0; y < MAX_Y; y++) {
			for (x = 0; x < MAX_X; x++) {
				for (c = 0; c < COLOR_BYTES; c++) {
					imag[z][y][x][c] = 255;
					swapAndWait(100);
				}
			}
		}
	}
}


void testAnim() {
	unsigned char x, y, z, i;
	for (i = 0; i < 50; i++) {
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
		if (i > 25) 
			swapAndWait(250);
		else 
			fade(20, 40);	
	}
}

void fnordLicht() {
	unsigned char *im, i, j;
	unsigned short k;

	for (j = 0; j < 5; j++) {
		for (i = 0; i < 255; i++) {
			im = (unsigned char *) imag;
			for (k = 0; k < MAX_Z*MAX_Y*MAX_X; k++) {
				*im++ = 255 - i;
				*im++ = i;
				*im++ = 0;
			}
			swapAndWait(20);
		}
		for (i = 0; i < 255; i++) {
			im = (unsigned char *) imag;
			for (k = 0; k < MAX_Z*MAX_Y*MAX_X; k++) {
				*im++;
				*im++ = 255 - i;
				*im++ = i;
			}
			swapAndWait(20);
		}
		for (i = 0; i < 255; i++) {
			im = (unsigned char *) imag;
			for (k = 0; k < MAX_Z*MAX_Y*MAX_X; k++) {
				*im++ = i;
				*im++;
				*im++ = 255 - i;
			}
			swapAndWait(20);
		}
	}
}

void symetricRandom() {
	unsigned char  i, j;
	voxel pos; 
	color col;
	for (j = 0; j < 254; j++) {
		clearScreen(black);
		for (i = 0; i < 12; i++) {
			pos.x = easyRandom() % 3;
			pos.y = easyRandom() % 3;
			pos.z = easyRandom() % 3;
			col.r = easyRandom();
			col.g = easyRandom();
			col.b = easyRandom();
			setSymetricVoxel(pos, col);
		}
		swapAndWait(20);
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
	// Startpoint of the cube
	voxel cube1 = {0, 0, 0}, cube2 = {3, 3, 3}, cube3 = {0, 3, 3};	
	direction way[] = {up, right, up, right, up, right, 
					   forward, forward, forward,
					   down, left, back, down, back, back,
					   down, left, left, 0}; 
	unsigned char i, j; 
	for (j = 0; j < 10; j++) {
		i = 0;
		while(way[i]) {
			switch (way[i++]) {
				case up:      cube1.z++; 
							  cube2.x--;
							  cube3.z--;
							  if (cube1.z > MAX_Z-1) 
								cube1.z = 0;
							  if (cube2.x > MAX_X-1) 
								cube2.x = 0; 
							  if (cube3.z > MAX_Z-1) 
								cube3.z = 0;
							  break;
				case down:    cube1.z--; 
							  cube2.x++;
							  cube3.z++;
							  if (cube1.z > MAX_Z-1) 
								cube1.z = 0;
							  if (cube2.x > MAX_X-1) 
								cube2.x = 0; 
							  if (cube3.z > MAX_Z-1) 
								cube3.z = 0; 
							  break;
				case right:   cube1.x++; 
							  cube2.z--;
							  cube3.x++;
							  if (cube1.x > MAX_X-1) 
								cube1.z = 0;
							  if (cube2.z > MAX_Z-1) 
								cube2.x = 0; 
							  if (cube3.x > MAX_X-1) 
								cube3.x = 0;
							  break;
				case left:    cube1.x--; 
							  cube2.z++;
							  cube3.x--;
							  if (cube1.x > MAX_X-1) 
								cube1.x = 0;
							  if (cube2.z > MAX_Z-1) 
								cube2.z = 0; 
							  if (cube3.x > MAX_X-1) 
								cube3.x = 0;
							  break;
				case forward: cube1.y++; 
							  cube2.y--;
							  cube3.y--;
							  if (cube1.y > MAX_Y-1) 
								cube1.y = 0;
							  if (cube2.y > MAX_Y-1) 
								cube2.y = 0; 
							  if (cube3.y > MAX_Y-1) 
								cube3.y = 0;
							  break;
				case back:    cube1.y--; 
							  cube2.y++;
							  cube3.y++;
							  if (cube1.y > MAX_Y-1) 
								cube1.y = 0;
							  if (cube2.y > MAX_Y-1) 
								cube2.y = 0;
							  if (cube3.y > MAX_Y-1) 
								cube3.y = 0;
							  break;
				default: break;
			}
			
			drawCube(cube1, 0);
			drawCube(cube2, 1);
			drawCube(cube3, 2);
			swapAndWait(110);
			clearScreen(black);			
		}
	}
}



#define SNAKE_LEN 100

void snake() {
	voxel pixels[SNAKE_LEN]; 
	voxel *head = &pixels[1];
	voxel *tail = &pixels[0];
	voxel old_head;
	color snakeColor = {156, 200, 73};
	voxel apples[10];
	unsigned char apple_num = 0;
	
	pixels[0].x = 1; 
	pixels[0].y = 1;
	pixels[0].z = 0;
	pixels[1].x = 1; 
	pixels[1].y = 2;
	pixels[1].z = 0;
	
	direction dir = forward;

	clearScreen(black);

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
			for (i = 1; i < 7; i++) {
				if ((getNextVoxel(old_head, (direction)i).x == apples[j].x) && 
					(getNextVoxel(old_head, (direction)i).y == apples[j].y) &&
				    (getNextVoxel(old_head, (direction)i).z == apples[j].z)) {
					apple_found = 1;
					dir = (direction)i+1; 
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
			while (isVoxelSet(getNextVoxel(old_head, dir))) {
				if ((dead_cnt++) == 4) {
					dead = 1;
					break;
				}
				dir = direction_r(dir);
			}
		}
		if (!dead) {
			*head = getNextVoxel(old_head, dir);
			setVoxel(*head, snakeColor);
			if (easyRandom() < 80) {
				dir = 1 + (direction) (easyRandom() % 6);
			}
			if ((apple_num<10) && (easyRandom()<10)) {
				voxel new_apple = (voxel) {easyRandom() % MAX_X,
										   easyRandom() % MAX_Y,
										   easyRandom() % MAX_Z};
				if (!isVoxelSet(new_apple)){
					apples[apple_num++] = new_apple;
				}
			}
			if (!apple_found) {
				setVoxel(*tail, black);
				if (++tail == pixels + SNAKE_LEN) 
					tail = pixels;
			}
		} else {
			while (tail != head) {
				setVoxel(*tail, black);
				if ((++tail) > pixels+SNAKE_LEN) 
					tail = pixels;
				wait(60);
			}
			break;
		}
		for (j = 0; j < apple_num; j++) {
			if (x % 2) {
				setVoxel(apples[j], snakeColor);
			} else {
				setVoxel(apples[j], black);
			}
		}
		snakeColor.r += 1;
		snakeColor.g += 2;
		snakeColor.b += 4;
		swapAndWait(60);
	}
}
