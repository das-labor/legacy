#include "config.h"
#include "util.h"

unsigned char pixmap[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];

#define FAKTOR ((64+2)/5)

void testAnim();

void *display_loop(void * unused)  {
	while (1) {
		testAnim();
	}
}

void testAnim() {
	unsigned char x, y, z;
	unsigned char i;
	for (i = 0; i < 150; i++) {
		for (z = 0; z < MAX_Z; z++) {
			for (y = 0; y < MAX_Y; y++) {
				for (x = 0; x < MAX_X; x++) {
					switch (i % 6) {
						case 0:
							pixmap[z][y][x][0] = x * FAKTOR;
							pixmap[z][y][x][1] = y * FAKTOR;
							pixmap[z][y][x][2] = z * FAKTOR;
							break;
						case 1:
							pixmap[z][y][x][0] = x * FAKTOR;
							pixmap[z][y][x][1] = z * FAKTOR;
							pixmap[z][y][x][2] = y * FAKTOR;
							break;
						case 2:
							pixmap[z][y][x][0] = y * FAKTOR;
							pixmap[z][y][x][1] = x * FAKTOR;
							pixmap[z][y][x][2] = z * FAKTOR;
							break;
						case 3:
							pixmap[z][y][x][0] = y * FAKTOR;
							pixmap[z][y][x][1] = z * FAKTOR;
							pixmap[z][y][x][2] = x * FAKTOR;
							break;
						case 4:
							pixmap[z][y][x][0] = z * FAKTOR;
							pixmap[z][y][x][1] = x * FAKTOR;
							pixmap[z][y][x][2] = y * FAKTOR;
							break;
						case 5:
							pixmap[z][y][x][0] = z * FAKTOR;
							pixmap[z][y][x][1] = y * FAKTOR;
							pixmap[z][y][x][2] = x * FAKTOR;
							break;
					} 
				}
			}
		}
		myWait(250);
	}
}