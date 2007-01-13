#include "config.h"

#define R 0
#define G 1
#define B 2

typedef struct {
	unsigned char x, y, z;
} voxel; 

typedef struct {
	unsigned char r, g, b;
} color; 


typedef enum {
	noDirection = 0,
	up, down, 
	right, left,
	forward, back
} direction;

extern color black, white, red, green, blue;

extern unsigned char imag[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];

void clearScreen(color c);
void clearImage(color c);

void setVoxel(voxel pos, color c);
void setSymetricVoxel(voxel pos, color c);

unsigned char isVoxelSet(voxel pos);
voxel getNextVoxel(voxel pos, direction d);

direction direction_r(direction dir);

void shift(direction dir);

void fade(unsigned char msProStep, unsigned char steps);
void swapAndWait(unsigned char ms);

unsigned char easyRandom();

void drawLine3D(char px1, char py1, char pz1, 
 			    char px2, char py2, char pz2, color value);

/* not jet implementet

void blurX(unsigned char filter[3]);
void blurY(unsigned char filter[3]);
void blurZ(unsigned char filter[3]);

*/