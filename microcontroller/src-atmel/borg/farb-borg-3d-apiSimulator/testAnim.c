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
	for (z = 0; z < MAX_Z; z++) {
		for (y = 0; y < MAX_Y; y++) {
			for (x = 0; x < MAX_X; x++) {
				pixmap[z][y][x][0] = x * FAKTOR;
				pixmap[z][y][x][1] = y * FAKTOR;
				pixmap[z][y][x][2] = z * FAKTOR;
				myWait(10);
			}
		}
	}
}