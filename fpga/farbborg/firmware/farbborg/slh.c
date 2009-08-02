#include "slh.h"

color randomColor() {
	color result;
	result.r = (easyRandom()%RAND_COL_STEPS)*(255/RAND_COL_STEPS);
	result.g = (easyRandom()%RAND_COL_STEPS)*(255/RAND_COL_STEPS);
	result.b = (easyRandom()%RAND_COL_STEPS)*(255/RAND_COL_STEPS);
	if (sameColor(result,black)) {
		result.r = result.g = result.b = 255;
	}
	return result;
}

voxel randomPos() {
	voxel result;
	result.x = easyRandom() % MAX_X;
	result.y = easyRandom() % MAX_Y;
	result.z = easyRandom() % MAX_Z;
	return result;
}

direction randomDirection() {
	return 1 + (direction)(easyRandom() % 6);
}

unsigned char sameColor(color param1, color param2) {
	if (param1.r != param2.r) return 0;
	if (param1.g != param2.g) return 0;
	if (param1.b != param2.b) return 0;
	return 1;
}

unsigned char sameCoord(voxel param1, voxel param2) {
	if (param1.x != param2.x) return 0;
	if (param1.y != param2.y) return 0;
	if (param1.z != param2.z) return 0;
	return 1;
}

color fadeBGR(color c) {
	if (c.b > FADE_BY) c.b -= FADE_BY;
	else if (c.g > FADE_BY) c.g -= FADE_BY;
	else if (c.r > FADE_BY) c.r -= FADE_BY;
	else c = black;
	return c;
}

color fadePixel(color c) {
	c.r = (c.r > FADE_MIN)?(c.r/2):0;
	c.g = (c.g > FADE_MIN)?(c.g/2):0;
	c.b = (c.b > FADE_MIN)?(c.b/2):0;
	return c;
}

unsigned char isVoxelValid(voxel pos) {
	// check if voxel exists
	if (pos.x < MAX_X && pos.y < MAX_Y && pos.z < MAX_Z) return 1;
	return 0;
}

