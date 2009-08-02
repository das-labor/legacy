#include <math.h>
#include <stdio.h>
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
extern unsigned int pixmap[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];
uint32_t pixmap_readback[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];

void uart_putstr(char *str) {
	puts(str);
}

void uart_putchar(char c) {
	putchar(c);
}

// a voxel is compareabel to a pixel in 2D, with the differents, that it has 
// a volume
void setVoxel(voxel pos, color c) {
	uint32_t *im;
	if (pos.x < MAX_X && pos.y < MAX_Y && pos.z < MAX_Z) {
		im = &imag[pos.z][pos.y][pos.x][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
	}
}

void setVoxelH_f(int x, int y, int z, float h)
{
	h -= floor(h);
	setVoxel((voxel) {x, y, z}, HtoRGB(h*49152));
}

void setVoxelH(int x, int y, int z, int h)
{
	setVoxel((voxel) {x, y, z}, HtoRGB((h*49152)/32768));
}

color getColor(voxel pos) {
	color result;
	if (pos.x < MAX_X && pos.y < MAX_Y && pos.z < MAX_Z) {
		result.r = imag[pos.z][pos.y][pos.x][R];
		result.g = imag[pos.z][pos.y][pos.x][G];
		result.b = imag[pos.z][pos.y][pos.x][B];
	} else 
		result = black;
	return result;
}

void setSymetricVoxel(voxel pos, color c) {
	uint32_t *im;
	if (pos.x < (MAX_X+1)/2 && pos.y < (MAX_Y+1)/2 && pos.z < (MAX_Z+1)/2) {
		im = &imag[pos.z+2][pos.y+2][pos.x+2][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = &imag[2-pos.z][pos.y+2][pos.x+2][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = &imag[pos.z+2][2-pos.y][pos.x+2][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = &imag[2-pos.z][2-pos.y][pos.x+2][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = &imag[pos.z+2][pos.y+2][2-pos.x][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = &imag[2-pos.z][pos.y+2][2-pos.x][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = &imag[pos.z+2][2-pos.y][2-pos.x][0];
		*im++ = c.r;
		*im++ = c.g;
		*im   = c.b;
		im = &imag[2-pos.z][2-pos.y][2-pos.x][0];
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

void fade(unsigned int msProStep, unsigned int steps) {
	uint32_t s;
	int32_t  addColor[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];
	int32_t  *aC = (int32_t *) addColor;
	int32_t  i, z, helpColor[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];
	int32_t  *help = (int32_t *) helpColor, temp;
	uint32_t *pix, val; 
	uint32_t *im  = (uint32_t *) imag; 
	uint32_t *pixr = (uint32_t *) pixmap_readback;
	
	for (i = 0; i < MAX_Z*MAX_Y*MAX_X*COLOR_BYTES; i++) {
		*help = *pixr * 1024;
		temp = ((*im & 0xff) * 1024) - *help;
		*aC   = temp/(int32_t) steps;
		pixr++;
		help++;
		aC++;
		im++;
	}

 	for (s = 0; s < steps; s++) {
		aC   = (int32_t *) addColor;
		help = (int32_t *) helpColor;
		pix  = (uint32_t *) pixmap;
		pixr = (uint32_t *) pixmap_readback;
		im   = (uint32_t *) imag;
		for (z = 0; z < MAX_Z; z++) {
			//pix = &PIXMAP[128*z];
			for (i = 0; i < MAX_Y*MAX_X*COLOR_BYTES; i++) {
				*help += *aC++; 
				val  = (*help++ + 512) / 1024;
				*pix++  = val;
				*pixr++ = val;
			}
		}
		myWait(msProStep);
	}
	swapAndWait(msProStep);
}

void swapAndWait(unsigned int ms) {
	uint32_t *pix = (uint32_t* ) pixmap, *im = (uint32_t *) imag;
	uint32_t *pixr = (uint32_t *) pixmap_readback;
	uint32_t i, z, help;
	for (z = 0; z < MAX_Z; z++) {
		for (i = 0; i < MAX_Y*MAX_X*COLOR_BYTES; i++) {
			help = (*im++) & 0xff;
			*pix++  = help;
			*pixr++ = help;	
		}
	}
	myWait(ms);
}

void clearScreen(color c) {
	uint32_t *im = (uint32_t *) imag, *pixr = (uint32_t *) pixmap_readback;
	uint32_t *pix = (uint32_t *) pixmap;
	unsigned char i, z;
	for (z = 0; z < MAX_Z; z++) {
		for (i = 0; i < MAX_Y*MAX_X; i++) {
			*pix++ = c.r;
			*pixr++ = c.r; 
			*im++  = c.r;
			*pix++ = c.g;
			*pixr++ = c.g;
			*im++  = c.g;
			*pix++ = c.b;
			*pixr++ = c.b;
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
	for (x = 0; x < 8; x++) {
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

color HtoRGB(int h31bit)
{
    color rgb;
	unsigned char sextant;
	int   q;
	
	h31bit %= 49152;
	if (h31bit < 0)
		h31bit += 49152;
	sextant    = h31bit / 8192;	
	h31bit     = h31bit % 8192;
	q          = 8191 - h31bit;	
	
	switch(sextant) {
	    case 0:
			rgb.r = 255;
			rgb.g = h31bit / 32; 
			rgb.b = 0;
			break;
	    case 1:
			rgb.r = q / 32;
			rgb.g = 255;
			rgb.b = 0;
			break;
	    case 2:
			rgb.r = 0;
			rgb.g = 255;
			rgb.b = h31bit / 32;
			break;
	    case 3:
			rgb.r = 0;
			rgb.g = q / 32;
			rgb.b = 255;
			break;
	    case 4:
			rgb.r = h31bit / 32;
			rgb.g = 0;
			rgb.b = 255;
			break;
	    default:
			rgb.r = 255;
			rgb.g = 0;
			rgb.b = q / 32;
			break;
   	}
    return rgb;
}

const int16_t sin_table[66] =
{
      0,   804,  1608,  2410,  3212,  4011,  4808,  5602,
   6393,  7179,  7962,  8739,  9512, 10278, 11039, 11793,
  12539, 13279, 14010, 14732, 15446, 16151, 16846, 17530,
  18204, 18868, 19519, 20159, 20787, 21403, 22005, 22594,
  23170, 23731, 24279, 24811, 25329, 25832, 26319, 26790,
  27245, 27683, 28105, 28510, 28898, 29268, 29621, 29956,
  30273, 30571, 30852, 31113, 31356, 31580, 31785, 31971, 
  32137, 32285, 32412, 32521, 32609, 32678, 32728, 32757,
  32767, 32757
};

int32_t Sine(int32_t phase)
{
	int16_t s0;
	uint16_t tmp_phase, tmp_phase_hi;

	tmp_phase = phase & 0x7fff;

	if (tmp_phase & 0x4000) 
		tmp_phase = 0x8000 - tmp_phase;

	tmp_phase_hi = tmp_phase >> 8; // 0...64

	s0 = sin_table[tmp_phase_hi];

	s0 += ((int16_t)((((int32_t)(sin_table[tmp_phase_hi+1] - s0))*(tmp_phase&0xff))>>8));

	if (phase & 0x8000) {
		s0 = -s0;
	}
	
	return s0;
}

int32_t Cosi(int32_t phase)
{
	return Sine(phase + 0x4000);
}

/* by Jim Ulery  http://www.azillionmonkeys.com/qed/ulerysqroot.pdf  */
unsigned isqrt(unsigned long val) {
	unsigned long temp, g=0, b = 0x8000, bshft = 15;
	do {
		if (val >= (temp = (((g << 1) + b)<<bshft--))) {
		   g += b;
		   val -= temp;
		}
	} while (b >>= 1);
	return g;
}

void shift(direction dir) {
	uint32_t i, z;
	uint32_t *fromIm, *toIm;
	
	switch (dir) {
		case up:
			for (z = 4; z > 0 ; z--) {
				fromIm = &imag[z-1][0][0][0];
				toIm   = &imag[z][0][0][0];
				for (i = 0; i < MAX_Y*MAX_X*COLOR_BYTES; i++) {
					*toIm = *fromIm;
					fromIm++;
					toIm++; 
				}
			}
			toIm = (uint32_t *) imag;
			for (i = 0; i < MAX_Y*MAX_X*COLOR_BYTES; i++) {
				*toIm++ = 0; 
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

// 16 = sin(90¡) = 1
char sinTab[] = {0, 6, 12, 19, 24, 30, 36, 41, 45, 49, 53, 56, 59, 61, 63, 64, 64};
		 
char Sin(unsigned char a) {
	a %= 64;
	if (a < 17) {
		return  sinTab[a];
	} else if (a < 33) {
		return  sinTab[32-a];
	} else if (a < 49) {
		return -sinTab[a-32];
	} else {
		return -sinTab[64-a];
	}
}	


voxel mulMatrixPoint(char *mat, voxel *p) {
	return (voxel) {
		(mat[0]*(char)p->x)/64 + (mat[1]*(char)p->y)/64 + (mat[2]*(char)p->z)/64 + mat[3],
		(mat[4]*(char)p->x)/64 + (mat[5]*(char)p->y)/64 + (mat[6]*(char)p->z)/64 + mat[7],
		(mat[8]*(char)p->x)/64 + (mat[9]*(char)p->y)/64 + (mat[10]*(char)p->z)/64 + mat[11]
	};
}

/*
Matrix Format

  0  1  2  3
  4  5  6  7  
  8  9 10 11
(12 13 14 15) not exist  because normally 0 0 0 1
              but works intern with homogen coordiantes
*/
void rotate(char a, char b, char c, voxel* points, 
			voxel* resPoints, int numPoint, voxel rotP) {
	char mat[12];
	unsigned char i;
					
	// Initialiesierung der Rotationsmatrix				
	mat[0] = (Cos(b)*Cos(c))/64;
	mat[1] = (-Cos(b)*Sin(c))/64;
	mat[2] = -Sin(b);
		
	mat[4] = (((-Sin(a)*Sin(b))/64)*Cos(c))/64 + (Cos(a)*Sin(c))/64;
	mat[5] = (((Sin(a)*Sin(b))/64)*Sin(c))/64 + (Cos(a)*Cos(c))/64;
	mat[6] = (-Sin(a)*Cos(b))/64;
		
	mat[8]  = (((Cos(a)*Sin(b))/64)*Cos(c))/64 + (Sin(a)*Sin(c))/64;
	mat[9]  = (((-Cos(a)*Sin(b))/64)*Sin(c))/64 + (Sin(a)*Cos(c))/64;
	mat[10] = (Cos(a)*Cos(b))/64;
					
	if (rotP.x == 0 && rotP.y == 0 && rotP.z == 0) {
		mat[3]  = 0;
		mat[7]  = 0;
		mat[11] = 0;
	} else {
		mat[3]  = rotP.x - ((mat[0]*(char)rotP.x)/64 + (mat[1]*(char)rotP.y)/64 + (mat[2]*(char)rotP.z)/64);
		mat[7]  = rotP.y - ((mat[4]*(char)rotP.x)/64 + (mat[5]*(char)rotP.y)/64 + (mat[6]*(char)rotP.z)/64);
		mat[11] = rotP.z - ((mat[8]*(char)rotP.x)/64 + (mat[9]*(char)rotP.y)/64 + (mat[10]*(char)rotP.z)/64);
	}
	/*
	for (i = 0; i < 3; i++) {
		printf("%d\t%d\t%d\t%d\n", mat[(i*4)], mat[(i*4)+1], mat[(i*4)+2], mat[(i*4)+3]);
	}*/
	
	for (i = 0; i < numPoint; i++) {
		resPoints[i] = mulMatrixPoint(mat, &points[i]);
	}	
}

void scale(char sx, char sy, char sz, voxel* points, 
			voxel* resPoints, int numPoint, voxel scaleP) {
	char mat[12] = {sx,  0,  0,  scaleP.x - (sx*scaleP.x)/64,
					 0, sy,  0,  scaleP.y - (sy*scaleP.y)/64,
					 0,  0, sz,  scaleP.z - (sz*scaleP.z)/64};
	unsigned char i;
 	for (i = 0; i < numPoint; i++) {
		resPoints[i] = mulMatrixPoint(mat, &points[i]);
	}			
}					

char testAusgabe;
void dP(char* txt, int32_t val) {
	if (!testAusgabe)
		return;
	printf("%s = %x\n", txt, val);
}	

void normalize() {
	int r, g, b, i, max_r = 4, max_g = 4, max_b = 4;
	uint32_t *im = (uint32_t *) imag;
	for (i = 0; i < MAX_Z*MAX_Y*MAX_X; i++) {
		r = *im++;
		g = *im++;
		b = *im++;
		r = ((r & 0xff)*256) + (r >> 24);
		g = ((g & 0xff)*256) + (g >> 24);
		b = ((b & 0xff)*256) + (b >> 24);
			
		if (r > max_r)
			max_r = r;
		if (g > max_g)
			max_g = g;
		if (b > max_b)
			max_b = b;
	}
	if (max_r >= max_g && max_g >= max_b) {
		max_g = max_r;
		max_b = max_r;
	} else if (max_g >= max_r && max_r >= max_b) {
		max_r = max_g;
		max_b = max_g;
	} else {
		max_r = max_b;
		max_g = max_b;
	}
	im = (uint32_t *) imag;
	for (i = 0; i < MAX_Z*MAX_Y*MAX_X; i++) {
		*im++ = (((*im & 0xff) * 256 + (*im>>24))*255) / max_r;
		*im++ = (((*im & 0xff) * 256 + (*im>>24))*255) / max_g;
		*im++ = (((*im & 0xff) * 256 + (*im>>24))*255) / max_b;
	}
}


// Dreidimensionales weichzeichnen mittels Faltung
// Dazu muss das bild weches weichgezeichnet werden soll zurerst ges
void blur() {
	unsigned char filter[3][3][3] = {{{0, 1, 0}, {1, 2, 1}, {0, 1, 0}}, 
									 {{1, 2, 1}, {2, 8, 2}, {1, 2, 1}},
									 {{0, 1, 0}, {1, 2, 1}, {0, 1, 0}}
									}; 
	uint32_t help_imag[MAX_Z][MAX_Y][MAX_X][COLOR_BYTES];
	uint32_t *im = (uint32_t *) imag, *hi = (uint32_t *) help_imag;									
	int32_t x, y, z, i, j, k, l, m, n, c, curVoxelColor, temp, test;

	for (z = 0; z < 5; z++) {
		for (y = 0; y < 5; y ++) {
			for (x = 0; x < 5; x++) {
				for (c = 0; c < 3; c++) { // colors r, g, b
					if (z == 0 && y == 0 && x == 0)
						testAusgabe = 1;
					else
						testAusgabe = 0;
					curVoxelColor = 0; // operate filter on one Voxelcolor
					for (i = 0; i < 3; i++) {
						for (j = 0; j < 3; j++) {
							for (k = 0; k < 3; k++) {
								l = x + i - 1;
								m = y + j - 1;
								n = z + k - 1;
								if (l >= 0 && l < 5 && m >= 0 && m < 5 && n >= 0 && n < 5)
								{
								   temp = ((imag[l][m][n][c]*256) + (imag[l][m][n][c] >> 24)) * filter[i][j][k];
								   test = imag[l][m][n][c];
								   if (test)
									test++;
								   /*dP("c", c);
								   dP("l", l);
								   dP("m", m);
								   dP("n", n);
								   dP("filter", filter[i][j][k]);	
								   dP("imag", imag[l][m][n][c]);
								   dP("add", temp); */
								   curVoxelColor += temp;
								   //printf("hb (imag[l][m][n][c]*256) = %x   ");
								   //dP("curVoxelColor", curVoxelColor);
								}
							}
						}
					}
					curVoxelColor /= 32;
					help_imag[z][y][x][c]  =  curVoxelColor / 256;
					help_imag[z][y][x][c] |= (curVoxelColor % 256) << 24; 
				}
			}
		}
	}
	for (i = 0; i < MAX_Z*MAX_Y*MAX_X; i++) {
		*im++ = *hi++;
		*im++ = *hi++;
		*im++ = *hi++;
	}
}
