#include "api.h"

typedef enum {
	Black,
	White,
	Red,
	Blue,
	Green,
	Yellow,
	C1,
	C2,
	C3
} beatColor;

typedef enum {
	chess,
	chessInv,
	all,
	fade1,
	cube0,
	cube1,
	cube2,
	cube3,
	blockUp,
	blockDown,
	blockRight,
	blockDiagonal
} beatPattern;

typedef struct {
	beatPattern pP;
	beatColor bC;
	unsigned char time;  // 128 is a full note - 32 is 1/4 - 16 is 1/8
} beat;

#define MAX_ENTRYS 46

static void drawCube(int size, voxel center, color curColor)
{
	voxel h;
	for (h.z = center.z - size; h.z <= center.z + size; h.z++) {
		for (h.y = center.y - size; h.y <= center.y + size; h.y++) {
			for (h.x = center.x - size; h.x <= center.x + size; h.x++) {
					setVoxel(h, curColor);
			}
		}
	}	
}

void funkyBeats() {
	beat beats[MAX_ENTRYS] = { // erster Takt
					  {blockDiagonal,   Red,     16},
					  {blockDown,       Blue,    16},
					  {blockUp,  		Green,   12},	
					  {cube0,      		Green, 	 12}, 
				  	  {cube1,   		Blue, 	 12},
					  {cube2,        	C3,	 	 16},
					  {blockDiagonal,   C2,      16},
					  {cube1,   		C1,      8},	
					  {cube0,      		C2,      16},
					  {cube1,      		Blue,    16},
					  {cube2,   		C2,      16},
					  {chess,      		C1,      8},
					  {chessInv,   		C2,      16},  
					  {chess,    		C3,  	 16},
					  {chessInv,       	White,   16},
					  {chess,      		White,   8},

					  {chessInv,      	White,   12}, 
				  	  {cube0,   		C2,      12},
					  {all,        		C1,	     12},
					  {cube1,     	    C2,      16},
					  {chessInv,   		Yellow,   8},	
					  {cube2,      		C3,       8},
					  {all,      		Yellow,  12},
					  {cube3,   		Red,      8},
					  {chess,      		Black,    8},
					  {blockDiagonal,   Red,      8},
					  {blockDown,       White,   12},
					  {cube1,  			Red,      8},
					  {chessInv,   		Blue,     8},  
					  {all,        		Green,    8},
					  {all,        		Black,   16},
					  {chess,      		Yellow,   8},

					  {all,       		Black,   8},
					  {cube1,  			White,  16},
					  {chess,      		Black,  16},
					  {chessInv,   		C3,     16},
					  {blockDiagonal, 	Green,   8},
					  {cube1,  			Black,   8},
					  {chess,      		Red,     8},
					  {all,        		Black,  12},
					  {cube1,  			Yellow, 12},
					  {all,        		Black,  12},
					  {fade1,       	C2,     16},
					  {cube1,  			White,   8},
					  {chessInv,   		Red,     8},
					  {blockUp,    		Green,   4},
					  {chess,      		C1,      8}  
					};

	unsigned char i, j, b, x, y, z;
	color curColor = white;
	uint32_t *im;
	voxel centerPos = {2, 2, 2};

	for (j = 0; j < 3; j++) {
		for (b = 0; b < MAX_ENTRYS; b++) {
			clearImage(black);
			switch (beats[b].bC) {
			
				case Black:
					curColor = black;
					break;
					
				case White:
					curColor = white;
					break;
					
				case Red:
					curColor = red;
					break;
					
				case Green:
					curColor = green;
					break;
					
				case Blue:
					curColor = blue;
					break;
					
				case Yellow:
					curColor = (color) {255, 255, 102};
					break;
					
				case C1:
					curColor = (color) {102, 102, 255};
					break;
					
				case C2:
					curColor = (color) {102,  70, 102};
					break;
					
				case C3:
					curColor = (color) {113, 120, 255};
					break;
			}
			switch (beats[b].pP) {
				case fade1:
				case all:
					clearImage(curColor);
					break;
					
				case chess:
					im = (uint32_t *) imag;
					for (i = 0; i < MAX_Z*MAX_Y*MAX_X; i++) {
						if (i & 1) {
							*im++ = curColor.r;
							*im++ = curColor.g;
							*im++ = curColor.b;
						} else {
							im += 3;
						}
					}	
					break;
					
				case chessInv:
					im = (uint32_t *) imag;
					for (i = 0; i < MAX_Z*MAX_Y*MAX_X; i++) {
						if (!(i & 1)) {
							*im++ = curColor.r;
							*im++ = curColor.g;
							*im++ = curColor.b;
						} else {
							im += 3;
						}
					}
					break;
				
				case cube0:
					drawCube(0, centerPos, curColor);
					break;
					
				case cube1:
					drawCube(1, centerPos, curColor);
					break;
				
				case cube2:
					drawCube(2, centerPos, curColor);
					break;
				
				case cube3:
					drawCube(3, centerPos, curColor);
					break;
					
				case blockUp:
					drawCube(1, (voxel) {1, 2, 3}, curColor);
					break;	
				
				case blockDown:
					drawCube(1, (voxel) {1, 2, 1}, curColor);
					break;
					
				case blockRight:
					drawCube(1, (voxel) {2, 3, 1}, curColor);
					break;
					
				case blockDiagonal:
					for (z = 0; z < 5; z++) {
						for (y = 0; y < 5; y++) {
							for (x = 0; x < 5; x++) {
								if ((x + y + z) < 6)
									setVoxel((voxel) {x, y, z}, curColor);
							}
						}
					}
					break;
			}
			
			//if (beats[b].pP == fade1)
			//	fade(16, beats[b].time);
			//else 
				fade(8, beats[b].time);
				swapAndWait(beats[b].time * 8);
				
		}
	}
}