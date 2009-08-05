#include "spike_hw.h"
#include "config.h"
#include "util.h"
#include "api.h"

color black = {  0,   0,   0};
color white = {255, 255, 255};
color red   = {255,   0,   0};
color green = {  0, 255,   0};
color blue  = {  0,   0, 255};

uint32_t imag[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];





// Die pixmap soll au§erhalb dses avrs liegen 
//extern uint32_t pixmap[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];

uint32_t (*pixmap_p)[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES] = 0xf0040000;



// a voxel is compareabel to a pixel in 2D, with the differents, that it has 
// a volume
void setVoxel(voxel pos, color c) {
	uint32_t *im;
	if (pos.x < MAX_X && pos.y < MAX_Y && pos.z < MAX_Z) {
		im = (uint32_t *) &imag[pos.z][pos.y][pos.x][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
	}
}

void setSymetricVoxel(voxel pos, color c) {
	uint32_t *im;
	if (pos.x < (MAX_X+1)/2 && pos.y < (MAX_Y+1)/2 && pos.z < (MAX_Z+1)/2) {
		im = (uint32_t *) &imag[pos.z+2][pos.y+2][pos.x+2][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = (uint32_t *) &imag[2-pos.z][pos.y+2][pos.x+2][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = (uint32_t *) &imag[pos.z+2][2-pos.y][pos.x+2][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = (uint32_t *) &imag[2-pos.z][2-pos.y][pos.x+2][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = (uint32_t *) &imag[pos.z+2][pos.y+2][2-pos.x][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = (uint32_t *) &imag[2-pos.z][pos.y+2][2-pos.x][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = (uint32_t *) &imag[pos.z+2][2-pos.y][2-pos.x][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = (uint32_t *) &imag[2-pos.z][2-pos.y][2-pos.x][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
	}
}

unsigned char isVoxelSet(voxel pos) {
	// check if voxel exists
	if (pos.x < MAX_X && pos.y < MAX_Y && pos.z < MAX_Z) {
		if (imag[pos.z][pos.y][pos.x][R] ||
			imag[pos.z][pos.y][pos.x][G] ||
			imag[pos.z][pos.y][pos.x][B]) {
			return 1; // set color != black
		}
		return 0;     // is black 
	} 
	return 2;         // is outside image
}

voxel getNextVoxel(voxel pos, direction d) {
	switch (d) {
		case up:	  pos.z++; break;
		case down:	  pos.z--; break;
		case back:	  pos.y++; break;
		case forward: pos.y--; break;
		case right:	  pos.x++; break;
		case left:	  pos.x--; break;
		default: break; 
	}
	return pos;
}

direction direction_r(direction dir) {
	switch (dir) {
		case right:
			return back;
		case down:
			return left;
		case left:
			return up;
		case up:
			return forward;	
		case back:
			return down;
		case forward:
			return right;
		default:
			return dir;
	}
}

// too big for a real avr
void fade(unsigned char msProStep, unsigned char steps) {
	unsigned int s;
	signed short addColor[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];
	signed short *aC = (signed short*) addColor;
	signed short helpColor[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];
	signed short *help = (signed short *) helpColor;
	unsigned char *pix, z, i, 
	              *im  = (unsigned char*) imag;
 	
	for (z = 0; z < MAX_Z; z++) {
		pix = &PIXMAP[128*z];
		for (i = 0; i < MAX_Y*MAX_X*COLOR_BYTES; i++) {
			*help = *pix++ << 7;
			*aC++ = ((*im++ << 7) - *help++)/steps;
				 }
	}
 	for (s = 0; s < steps; s++) {
		aC   = (signed short*) addColor;
		help = (signed short *) helpColor;
		im   = (unsigned char*) imag;
		for (z = 0; z < MAX_Z; z++) {
			pix = &PIXMAP[128*z];
			for (i = 0; i < MAX_Y*MAX_X*COLOR_BYTES; i++) {
				*help += *aC++;  
				*pix++ = (*help++ + 64) >> 7;
			}
		}
		myWait(msProStep);
	}
	swapAndWait(msProStep);
}

void swapAndWait(unsigned char ms) {
	uint32_t *pix, *im = (uint32_t *) imag;
	unsigned char i, z;
	for (z = 0; z < MAX_Z; z++) {
		pix = (uint32_t *) &PIXMAP[128*z];
		for (i = 0; i < MAX_Y*MAX_X*COLOR_BYTES; i++) {
			*pix++ = *im++;	
		}
	}
	myWait(ms);
} 

void clearScreen(color c) {
	uint32_t *pix = (uint32_t *) pixmap, *im = (uint32_t *) imag;
	unsigned char i, z;
	for (z = 0; z < MAX_Z; z++) {
		pix = (uint32_t *) &PIXMAP[128*z];
		for (i = 0; i < MAX_Y*MAX_X; i++) {
			*pix++ = c.r;
			*im++  = c.r;
			*pix++ = c.g;
			*im++  = c.g;
			*pix++ = c.b;
			*im++  = c.b;
		}
	}
}

void clearImage(color c) {
	uint32_t *im = (uint32_t *) imag;
	unsigned short i;
	for (i = 0; i < MAX_Z*MAX_Y*MAX_X; i++) {
		*im++  = c.r;
		*im++  = c.g;
		*im++  = c.b;
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
	unsigned char i, z;
	uint32_t *im  = (uint32_t*) imag;
	
	switch (dir) {
		case up:
			for (z = 4; z < MAX_Z; z--) {
				im = (uint32_t *) &imag[z][0][0][0];
				for (i = 0; i < MAX_Y*MAX_X*COLOR_BYTES; i++) {
					im[MAX_Y*MAX_X*COLOR_BYTES] = *im++; 
				}
			}
			im = (uint32_t *) imag;
			for (i = 0; i < MAX_Y*MAX_X*COLOR_BYTES; i++) {
				*im++ = 0; 
			}
			break;
		case down:
			break;
		case forward:
			break;
		case back:
			break;
		case right:
			break;
		case left:
			break;
		default:
			break;
	}
}



/** Draws a thredimentional line with the bressenham 3d algrorthmus form the point
  * px1, py1, xz1 to the point px2, py2, pz2 with the brightness value.
  */
void drawLine3D(char px1, char py1, char pz1, 
 			    char px2, char py2, char pz2, color value) {
    int8_t i, dx, dy, dz, l, m, n, x_inc, y_inc, z_inc, err_1, err_2, dx2, dy2, dz2;
    int8_t curx = px1, cury = py1, curz = pz1;
    dx = (px2 - px1);
    dy = (py2 - py1);
    dz = (pz2 - pz1);
    x_inc = (dx < 0) ? -1 : 1;	// sign
    l = dx >= 0 ? dx : -dx;    	// abs
    y_inc = (dy < 0) ? -1 : 1;  
    m = dy >= 0 ? dy : -dy;
    z_inc = (dz < 0) ? -1 : 1;
    n = dz >= 0 ? dz : -dz;
    dx2 = l << 1;
    dy2 = m << 1;
    dz2 = n << 1;

    if ((l >= m) && (l >= n)) {
        err_1 = dy2 - l;
        err_2 = dz2 - l;
        for (i = 0; i < l; i++) {
            setVoxel((voxel) {curx, cury, curz}, value);
            if (err_1 > 0) {
                cury += y_inc;
                err_1 -= dx2;
            }
            if (err_2 > 0) {
                curz += z_inc;
                err_2 -= dx2;
            }
            err_1 += dy2;
            err_2 += dz2;
            curx += x_inc;
        }
    } else if ((m >= l) && (m >= n)) {
        err_1 = dx2 - m;
        err_2 = dz2 - m;
        for (i = 0; i < m; i++) {
            setVoxel((voxel) {curx, cury, curz}, value);
            if (err_1 > 0) {
                curx += x_inc;
                err_1 -= dy2;
            }
            if (err_2 > 0) {
                curz += z_inc;
                err_2 -= dy2;
            }
            err_1 += dx2;
            err_2 += dz2;
            cury += y_inc;
        }
    } else {
        err_1 = dy2 - n;
        err_2 = dx2 - n;
        for (i = 0; i < n; i++) {
            setVoxel((voxel) {curx, cury, curz}, value);
            if (err_1 > 0) {
                cury += y_inc;
                err_1 -= dz2;
            }
            if (err_2 > 0) {
                curx += x_inc;
                err_2 -= dz2;
            }
            err_1 += dy2;
            err_2 += dx2;
            curz += z_inc;
        }
    }
   	setVoxel((voxel) {curx, cury, curz}, value);
}
