#include "api.h"

#define FAKTOR (255/5) 

void ansteuerungZeitlupe() 
{
	uint32_t tempImag[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];
	uint32_t *im, *im2, i, x, y, z;
	// Farbraum Bild erzeugen
	for (z = 0; z < MAX_Z; z++) {
		for (y = 0; y < MAX_Y; y++) {
			for (x = 0; x < MAX_X; x++) {
				tempImag[z][y][x][R] = z * FAKTOR;	
				tempImag[z][y][x][G] = y * FAKTOR;
				tempImag[z][y][x][B] = x * FAKTOR;
			}
		}
	}
	im  = (uint32_t*) imag;
	im2 = (uint32_t*) tempImag;
	for (i = 0; i < MAX_Z*MAX_Y*MAX_X*3; i++)
		*im++ = *im2++;
	swapAndWait(500);
	swapAndWait(500);
	swapAndWait(500);
	swapAndWait(500);
	swapAndWait(500);
	swapAndWait(500);
	
	for (z = 0; z < 5; z++) {
		clearScreen(black);
		for (debug_val_2 = 1; debug_val_2 < 256; debug_val_2++) {
			im2 = (uint32_t*) &tempImag[z][0][0][0];
			im  = (uint32_t*) &imag[z][0][0][0];
			for (i = 0; i < MAX_X*MAX_Y*3; i++) {
				if (debug_val_2 > *im2)
					*im = 0;
				else
					*im = 255;
				im++;
				im2++;
			}
			swapAndWait(debug_val_2);
		}
	}
}
