#ifndef API_H
#define API_H

#include <stdio.h>
#include "util.h"
#include "config.h"

// Please read the comments of the c-file

/** These array stores the pixels with brithness informations.
 *  I start with the coordinatesystem:
 *
 *     z |   /
 *       |  / y           
 *  left | /   down       
 *     0 |/________
 *        0       x
 *         front
 *
 *  The brithness informaition is stored, using 3 equaly bitarrays.
 *  A pixel is off, if its not set in each of the 3 bitarrays
 *  Its 1 then its only in the first, 2 then its set in the first and 
 *  the secound and its 3 then its set in all bitarrays.
 *  Other possibilitys are not allowed, because there are different 
 *  handeled in the simulator and the real borg.
 *  These format is needed by the real one.
 *  
#ifdef NEW_GENERATION 
 *  pixmap[<bitarray number>][<z-coordinate>][<y-coordinate>] = x_bits
 *  x_bits & (1<<x-coordinate) => value of coordinate (x,y,z) 
#else
 *  pixmap[<bitarray number>][<x-coordinate>][<y-coordinate>] = z_bits
 *  z_bits & (1<<z-coordinate) => value of coordinate (x,y,z) 
#endif 
 */
extern unsigned char pixmap[NUM_LEVELS][LEN_Z][LEN_Y];

#ifndef PIXEL_C
extern unsigned short shl_table[];
#endif

/** stores a three-dimentional pixel */
typedef struct {
	unsigned char x;
	unsigned char y;
	unsigned char z;
//	unsigned char dummy;  // the optimation of the avr-gcc could be better with it
} voxel;

/** see the set_plane(..) comment in the c-file. */
typedef enum {right, left, forward, back, up, down} direction;

void clear_screen(unsigned char value);
void set_voxel(voxel p, unsigned char value);
/* be carefull clearpixel3d() is only a macro */ 
#define clear_voxel(p) set_voxel(p, 0);

unsigned char get_voxel(voxel p);
unsigned char get_next_voxel(voxel p, direction dir);

voxel next_pixel3d(voxel pix, direction dir);

direction direction_r(direction dir);
direction turn_right(direction dir);
direction turn_left(direction dir);
direction turn_up(direction dir);
direction turn_down(direction dir);

unsigned char easyRandom();

void shift3d(direction dir);
void set_plane(direction dir, unsigned char num, unsigned char color);
voxel mulMatrixPoint(char *mat, voxel *p);

char Sin(unsigned char a);
#define Cos(a) Sin((a)+16)

void scale(char sx, char sy, char sz, voxel* points, 
		   voxel* resPoints, int numPoint, voxel scaleP);
void rotate(char a, char b, char c, voxel* points, 
			voxel* resPoints, int numPoint, voxel rotP);
void drawLine3D(char px1, char py1, char pz1, 
 		char px2, char py2, char pz2, unsigned char value);

#endif // API_H
