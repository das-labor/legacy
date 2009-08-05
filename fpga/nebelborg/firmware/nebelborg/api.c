#include "spike_hw.h"
#include "config.h"
#include "util.h"
#include "api.h"

color black = {  0,   0,   0};
color white = {255, 255, 255};
color red   = {255,   0,   0};
color green = {  0, 255,   0};
color blue  = {  0,   0, 255};

uint8_t imag[MAX_Y][MAX_X][COLOR_BYTES];

//Color order is GBR
uint32_t (*pixmap_p)[NUM_HW_ROWS][NUM_HW_COLS] = (void *)0xf0040000;

#define pixmap (*pixmap_p)



void initPwm(){
	int i, tmp;
	//init of the pwm table
	for (i = 0; i < 256; i++) {
		tmp = i/2;
		if (tmp < 14) 
			tmp = 14;
		PWMTABLE[i] = tmp;
	}
	for (i = 0; i < 1024; i++) {
		PIXMAP[i] = 0x10;
	}
	//magic turn-on command
	*((uint32_t *) 0xf0041d54) = 0x23;
}


void setPixel(pixel p, color c){
	uint8_t * im = (void *)&imag[p.y][p.x];
	*im++ = c.g;
	*im++ = c.b;
	*im = c.r;
}


void swap(){
	int x,y;
	uint8_t * im; uint32_t * pix;
	for(y=0;y<MAX_Y;y++){
		pix = (void*)&pixmap[y];
		im = (void*)&imag[y];
		for(x=0;x<(COLOR_BYTES*MAX_X); x++){
			*pix++ = *im++;
		}
	}
}


#if 0

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
#endif
