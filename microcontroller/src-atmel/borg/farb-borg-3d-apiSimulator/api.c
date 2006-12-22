#include "config.h"
#include "util.h"
#include "api.h"
 
unsigned char imag[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];

// Die pixmap soll au§erhalb des avrs liegen 
extern unsigned char pixmap[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];

// a voxel is compareabel to a pixel in 2D, with the differents, that it has a volume
void setVoxel(voxel pos, unsigned char red, unsigned char green, unsigned char blue) {
	unsigned char *im;
	if (pos.x < MAX_X && pos.y < MAX_Y && pos.z < MAX_Z) {
		im = (unsigned char *) imag[pos.z][pos.y][pos.x][0];
		*im++ = red;
		*im++ = green;
		*im   = blue;
	}
}

// too big for a real avr
void fade(unsigned char msProStep, unsigned char steps) {
	unsigned int s;
	signed short addColor[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];
	signed short *aC = (signed short*) addColor;
	signed short i, helpColor[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];
	signed short *help = (signed short *) helpColor;
	unsigned char *pix = (unsigned char*) pixmap, 
	              *im  = (unsigned char*) imag;
 	
	for (i = 0; i < MAX_Z*MAX_Y*MAX_X*COLOR_BYTES; i++) {
		*help = *pix++ << 8;
		*aC++ = ((*im++ << 8) - *help++)/steps;
	}

 	for (s = 0; s < steps; s++) {
		aC =(signed short*) addColor;
		help = (signed short *) helpColor;
		pix = (unsigned char*) pixmap;
		im = (unsigned char*) imag;

		for (i = 0; i < MAX_Z*MAX_Y*MAX_X*COLOR_BYTES; i++) {
			*help += *aC++;  
			*pix++ = (*help++ + 128) >> 8;
		}
		myWait(msProStep);
	}
	pix = (unsigned char*) pixmap;
	im = (unsigned char*) imag;
	for (i = 0; i < MAX_Z*MAX_Y*MAX_X*COLOR_BYTES; i++) {
		*pix++ = *im++;
	}
	myWait(msProStep);
}

void swapAndWait(unsigned char ms) {
	unsigned char *pix = (unsigned char *) pixmap, *im = (unsigned char *) imag;
	unsigned short i;
	for (i = 0; i < MAX_Z*MAX_Y*MAX_X*COLOR_BYTES; i++) {
		*pix++ = *im++;	
	}
	myWait(ms);
} 

void clearScreen(unsigned char red, unsigned char  green, unsigned char blue) {
	unsigned char *pix = (unsigned char *) pixmap, *im = (unsigned char *) imag;
	unsigned short i;
	for (i = 0; i < MAX_Z*MAX_Y*MAX_X; i++) {
		*pix++ = red;
		*im++  = red;
		*pix++ = green;
		*im++  = green;
		*pix++ = blue;
		*im++  = blue;
	}
}

#define BIT_S(var,b) ((var&(1<<b))?1:0)

unsigned char easyRandom() {
	static unsigned int muh = 0xAA;
	unsigned char x;
	for(x = 0; x < 8; x++) {
		muh = (muh<<1) ^ BIT_S(muh,1) ^ BIT_S(muh,8) ^ BIT_S(muh,9) ^ 
		      BIT_S(muh,13) ^ BIT_S(muh,15);
	}
	return (unsigned char) muh;
}

/*
unsigned char get_voxel(voxel p);
unsigned char get_next_voxel(voxel p, direction dir);

voxel next_voxel(voxel pix, direction dir);
*/

void shift(direction dir) {



}

/*
void blurX(unsigned char filter[3]) {
	unsigned char  x, y, z, i;
	unsigned short zeile[MAX_X][3], div = filter[0] + filter[1] + filter[2];
	for (z = 0; z < MAX_Z; z++) {
		for (y = 0; y < MAX_Y; y++) {
			// Filter auf Zeile anwenden
			for (x = 0; x < MAX_X; x++) {
				for (i = 0; i < 3; i++) {
					if ((x+i-1) < MAX_X) {
						zeile[x][0] = imag[z][y][x+i][0] * filter[i];
						zeile[x][1] = imag[z][y][x+i][1] * filter[i];
						zeile[x][2] = imag[z][y][x+i][2] * filter[i];
					}
				}
			}
			// Zeile zurŸck ins Bild kopieren
			for (x = 0; x < MAX_X; x++) {
				imag[z][y][x][0] = zeile[x][0]/div;
				imag[z][y][x][1] = zeile[x][1]/div;
				imag[z][y][x][2] = zeile[x][2]/div;
			}
		}
	}
}

void blurY(unsigned char filter[3]) {
	
}

void blurZ(unsigned char filter[3]) {
	
}
*/