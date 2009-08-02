#include "colorMatrix.h"

void colorMatrix() {
	pixel3d worker;
	unsigned char x,y,z, fade_out = 8;
	while (fade_out) {
		if (easyRandom()%80 && fade_out == 8) {
			x = (easyRandom()%3) + 1;
		} else {
			fade_out--;
			x = 0;
		}
		while(x) {
			x--;
			worker.pp = randomPos();
			worker.pp.z = 4;
			if (!isVoxelSet(worker.pp)) {
				worker.col = randomColor();
				MATRIX(worker.col.r = worker.col.g = worker.col.b = 255);
				setVoxel(worker.pp, worker.col);
			}
		}
		for (x = 0; x < MAX_X; x++) {
			worker.pp.x = x;
			for (y = 0; y < MAX_Y; y++) {
				worker.pp.y = y;
				for (z = 1; z <= MAX_Z; z++) {
					worker.pp.z = z;
					worker.col = getColor(worker.pp);
					worker.pp.z--;
					if (z == 1 || sameColor(getColor(worker.pp),black)) {
						setVoxel(worker.pp, worker.col);
					} else {
						worker.col = getColor(worker.pp);
						MATRIX(worker.col.r = worker.col.b = 0);
						setVoxel(worker.pp, fadePixel(worker.col));
					}
				}
			}
		}
		swapAndWait(155);
	}
	clearScreen(black);
	swapAndWait(255);
}

