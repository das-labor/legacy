#include "pixel3d.h"

void drawBar(char quater, char value) {
	char minx, maxx, miny, maxy, x, y, z;
	
	switch (quater) {
	case 0:
		minx = 0, maxx = 4;
		miny = 0, maxy = 4;
		break;
	case 1:
		minx = 4, maxx = 8;
		miny = 0, maxy = 4;
		break;
	case 2:
		minx = 0, maxx = 4;
		miny = 4, maxy = 8;
		break;
	case 3:
		minx = 4, maxx = 8;
		miny = 4, maxy = 8;
		break;
	}
	for (x = minx; x < maxx; x++) {
		for (y = miny; y < maxy; y++) {
			for (z = 0; z < value; z++) {
				setpixel3d((pixel3d){x,y,z}, 3);
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


void growingCubeFilled(unsigned char value) {
	unsigned char x, y, z;
	unsigned char min = 4 - value/2 - value%2;
	unsigned char max = 3 + value/2 + value%2;
	for (x = min; x <= max; x++) {
		for (y = min; y <= max; y++) {
			for (z = min; z <= max; z++) {
				setpixel3d((pixel3d){x,y,z}, myrandom()%3);
			}
		}
	}

}
