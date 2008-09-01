/** This is the api of the borg-3d.
 *  The same api is used by the Simulator and the real one. LEN_X
 */
#include "api.h"

/** faster shifting (eat this, Fefe!)
 */
unsigned short shl_table[] = {0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,
							  0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};

/** sets the brightness value to alle pixels. Its fast because its works directly on 
 *  the image memory.
 */
void clear_screen(unsigned char value){
	unsigned char p, x, y;
	unsigned short v = 0xFFFF;
	for(p = 0; p < NUM_LEVELS; p++) {
		if (p == value) 
			v=0;
		for (y = 0; y < LEN_Z; y++){
			for (x = 0; x < LEN_Y; x++){
				pixmap[p][y][x] = v;
			}
		}
	}
}

/** sets a three-dimentional Point at the position p with the brithness value 
 */
void setpixel3d(pixel3d p, unsigned char value ){
	unsigned char plane;
	if (p.x < LEN_X && p.y < LEN_Y && p.z < LEN_Z) { 
		for (plane=0; plane < NUM_LEVELS; plane++) {
			if (plane < value)
				pixmap[plane][p.z][p.y] |=  shl_table[p.x];
			else
				pixmap[plane][p.z][p.y] &= ~shl_table[p.x];
		}
	}	
}

/** shifts all pixel to the direction dir. The pixels, who are shiftet out
 *  are lost. Its fast because it works directly with the image memory.
 */
void shift3d(direction dir) {
     unsigned char i, j, k;
     switch (dir) {
     case down:
          for (i = 1; i < LEN_Z; i ++) {
              for (j = 0; j < LEN_Y; j++) {
                  for (k = 0; k < NUM_LEVELS; k++) {
                      pixmap[k][i-1][j] = pixmap[k][i][j]; 
                  }
              }
          }
          for (j = 0; j < LEN_Y; j++) {
              for (k = 0; k < NUM_LEVELS; k++) {
                  pixmap[k][LEN_Z-1][j] = 0; 
              }
          }
          break;
		  
     case up:
          for (i = LEN_Z-2; i < LEN_Z; i--) {
              for (j = LEN_Y-1; j < LEN_Y; j--) {
                  for (k = 0; k < NUM_LEVELS; k++) {
                      pixmap[k][i+1][j] = pixmap[k][i][j]; 
                  }
              }
          }
          for (j = LEN_Y-1; j < LEN_Y; j--) {
              for (k = 0; k < NUM_LEVELS; k++) {
                  pixmap[k][0][j] = 0;
              }
          }
          break;
		  
     case forward:
          for (i = 0; i < LEN_Z; i ++) {
              for (j = 1; j < LEN_Y; j++) {
                  for (k = 0; k < NUM_LEVELS; k++) {
                      pixmap[k][i][j-1] = pixmap[k][i][j]; 
                  }
              }
          }
          for (j = 0; j < LEN_Z; j++) {
              for (k = 0; k < NUM_LEVELS; k++) {
                  pixmap[k][j][LEN_Z-1] = 0; 
              }
          }
          break;
		  
     case back:
          for (i = LEN_Z-1; i < LEN_Z; i--) {
              for (j = LEN_Y-2; j < LEN_Y; j--) {
                  for (k = 0; k < NUM_LEVELS; k++) {
                      pixmap[k][i][j+1] = pixmap[k][i][j]; 
                  }
              }
          }
          for (j = LEN_Z-1; j < LEN_Y; j--) {
              for (k = 0; k < NUM_LEVELS; k++) {
                  pixmap[k][j][0] = 0;
              }
          }
          break;
		  
     case right:
          for (i = 0; i < LEN_Z; i ++) {
              for (j = 0; j < LEN_Y; j++) {
                  for (k = 0; k < NUM_LEVELS; k++) {
                      pixmap[k][i][j] = pixmap[k][i][j] << 1; 
                  }
              }
          }
          break;
		  
     case left:
          for (i = 0; i < LEN_Z; i ++) {
              for (j = 0; j < LEN_Y; j++) {
                  for (k = 0; k < NUM_LEVELS; k++) {
                      pixmap[k][i][j] = pixmap[k][i][j] >> 1;                   
                  }
              }
          }
          break;
     }     
}

/** returns 1 if the pixel p is not off. 0 if its off and 0xff if the pixel is not valid.
 *  FIXME: Does it really make sense to return 0xff in this case.
 */
unsigned char get_pixel3d(pixel3d p) {
	if ((p.x > (LEN_X-1)) || (p.y > (LEN_Y-1)) || (p.z > (LEN_Z-1))) {
		return 0xff;
	} else {
		return (pixmap[0][p.z][p.y] & shl_table[p.x]) ? 1:0;
	}
}

/** Gets the pixel, witch lays in the direction dir of the pixel p. The return values 
 *  are the same as unsigned char get_pixel3d(pixel3d p).
 */
unsigned char get_next_pixel3d(pixel3d p, direction dir) {
	pixel3d tmp;
	switch (dir) {
		case back:
			tmp = (pixel3d){p.x+1, p.y, p.z};
			break;
		case forward:
			tmp = (pixel3d){p.x-1, p.y, p.z};
			break;
		case up:
			tmp = (pixel3d){p.x, p.y, p.z+1};
			break;	
		case down:
			tmp = (pixel3d){p.x, p.y, p.z-1};
			break;
		case right:
			tmp = (pixel3d){p.x, p.y+1, p.z};
			break;
		case left:
			tmp = (pixel3d){p.x, p.y-1, p.z};
			break;			
	}
	return get_pixel3d(tmp);
}

/** Lets the direction turn right. Its programed for snake3d.
 *  Its also turns right in a circle.
 */
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
		default:
			return right;					
	}
}

/** gets the pixel, which is in the directions dir next to pixel pix
 *
 */
pixel3d next_pixel3d(pixel3d pix, direction dir) {
	switch (dir){
		case back:
			return((pixel3d){pix.x+1, pix.y, pix.z});
			break;
		case forward:
			return((pixel3d){pix.x-1, pix.y, pix.z});
			break;
		case down:
			return((pixel3d){pix.x, pix.y, pix.z-1});
			break;
		case up:
			return((pixel3d){pix.x, pix.y, pix.z+1});
			break;
		case left:
			return((pixel3d){pix.x, pix.y-1, pix.z});
			break;
		case right:
			return((pixel3d){pix.x, pix.y+1, pix.z});
			break;
	}
	return (pixel3d){0,0,0};
}

/** turns the dir right */
direction turn_right(direction dir){
	switch (dir) {
		case right:
			return back;
		case down:
			return right;
		case left:
			return forward;
		case up:
			return left;	
		case back:
			return left;
		default:
			return right;					
	}
}

/** turns the dir left */
direction turn_left(direction dir){
	switch (dir) {
		case right:
			return forward;
		case down:
			return left;
		case left:
			return back;
		case up:
			return left;	
		case back:
			return right;
		default:
			return left;					
	}
}

/** turns the dir up */
direction turn_up(direction dir){
	switch (dir) {
		case right:
			return up;
		case down:
			return forward;
		case left:
			return up;
		case up:
			return back;	
		case back:
			return up;
		default:
			return up;					
	}
}

/** turns the dir down */
direction turn_down(direction dir) {
	switch (dir) {
		case right:
			return down;
		case down:
			return back;
		case left:
			return down;
		case up:
			return forward;	
		case back:
			return down;
		default:
			return down;					
	}
}

/** sets a whole plane to the brightness color. 
 *  The 64 LED of the plane, which is defined by the direction dir and the 
 *  num.
 *           back
 *         -------
 *        / up  / |             |_______________|     
 *       -------  | right        0 1 2 3 4 5 6 7
 * left  |front|  |             dir
 *       |     | /
 *       -------
 *         down
 */
void set_plane(direction dir, unsigned char num, unsigned char color)
{
	unsigned char pindex = 0;
	int p, y, x;
	unsigned short v = 0xFFFF;
	
	switch (dir) {
		case back:
			pindex = LEN_Z - (num+1);
			for (x = 0; x < LEN_Y; x++) {
				for (p = 0; p < NUM_LEVELS; p++) {
					if (p < color)
						pixmap[p][x][pindex] = v;
					else
						pixmap[p][x][pindex] &= ~v;
				}
			 }			
			break;
			
		case forward:
			 pindex = num;			 
			 for (x = 0; x < LEN_Y; x++) {
				for (p = 0; p < NUM_LEVELS; p++) {
					if (p < color)
						pixmap[p][x][pindex] = v;
					else
						pixmap[p][x][pindex] &= ~v;
				}
			 }
			break;
			
		case up:
			pindex = LEN_Z-(num+1);
			for (y = 0; y < LEN_Z; y++) {
				for (p = 0; p < NUM_LEVELS; p++) {
					if ( p < color)
						pixmap[p][pindex][y] =  v;
					else
						pixmap[p][pindex][y] &= ~v;				
				}
			 }
			break;
			
		case down:
			pindex = num;
			for (y = 0; y < LEN_Z; y++) {
				for (p = 0; p < NUM_LEVELS; p++) {
					if (p < color)
						pixmap[p][pindex][y] = v;
					else
						pixmap[p][pindex][y] &= ~v;				
				}
			 }
			break;
			
		case right:
			v = shl_table[LEN_Y - (num+1)];
			for (p = 0; p < NUM_LEVELS; p++) {
				for (y = 0; y < LEN_Z; y++) {
					for (x = 0; x < LEN_Y; x++) {
						if ( p < color)
							pixmap[p][y][x] |= v;
						else
							pixmap[p][y][x] &= ~v;				
					}
				}
			}
			break;

		case left:
			v = shl_table[num];
			for (p = 0; p < NUM_LEVELS; p++) {
				for (y = 0; y < LEN_Z; y++) {
					for (x = 0; x < LEN_Y; x++) {
						if (p < color)
							pixmap[p][y][x] |= v;
						else
							pixmap[p][y][x] &= ~v;				
					}
				}
			}
			break;
	} //end switch(dir)
}

/** Table for the calculation of sinus. It only has one half wave, because
 *  the others can be generated of it.
 *  
 *  64 = sin(90 grad) = 1.0 = sinTab[16]
 */
char sinTab[] = {0, 6, 12, 19, 24, 30, 36, 41, 45, 49, 53, 56, 59, 61, 63, 64, 64};

/** Sin(64) = sin(360 grad) and sin(90 grad) = Sin(16) = 64. Its using the one halfwave
 *  of sinTab to generate a whole sinus. It e
 */		 
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

/** Multiplicates a point p with the tranformationmatrix mat.
 *  The matrix must be a char array with the following format:
 *    0  1  2  3
 *    4  5  6  7
 *    8  9 10 11
 *  (12 13 14 15) not exist because normally 0 0 0 1
 *				  but works intern with homogen coordiantes
 *   all coordinates are converted to signed ones. 
 *   64 represents 1.0.
 */
pixel3d mulMatrixPoint(char *mat, pixel3d *p) {
	return (pixel3d) {
		(mat[0]*(char)p->x)/64 + (mat[1]*(char)p->y)/64 + (mat[2]*(char)p->z)/64 + mat[3],
		(mat[4]*(char)p->x)/64 + (mat[5]*(char)p->y)/64 + (mat[6]*(char)p->z)/64 + mat[7],
		(mat[8]*(char)p->x)/64 + (mat[9]*(char)p->y)/64 + (mat[10]*(char)p->z)/64 + mat[11]
	};
}

/** Rotateds numPoints many 3d-points of points with the angel a arount the x axis, the
 *  angle b arount the y-axis and with the angle c arount the z-axis. (The angeles are
 *  documentated in Sin(..) dokumentation.). The result is stored in resPoints.
 *  64 = 1.0
 *  The point rotP is the centrum of rotation.
 */
void rotate(char a, char b, char c, pixel3d* points, 
			pixel3d* resPoints, int numPoint, pixel3d rotP) {
	char mat[12];
	unsigned char i;
					
	// init the rotationmatrix			
	mat[0] = (Cos(b)*Cos(c))/64;
	mat[1] = (-Cos(b)*Sin(c))/64;
	mat[2] = -Sin(b);
		
	mat[4] = (((-Sin(a)*Sin(b))/64)*Cos(c))/64 + (Cos(a)*Sin(c))/64;
	mat[5] = (((Sin(a)*Sin(b))/64)*Sin(c))/64 + (Cos(a)*Cos(c))/64;
	mat[6] = (-Sin(a)*Cos(b))/64;
		
	mat[8]  = (((Cos(a)*Sin(b))/64)*Cos(c))/64 + (Sin(a)*Sin(c))/64;
	mat[9]  = (((-Cos(a)*Sin(b))/64)*Sin(c))/64 + (Sin(a)*Cos(c))/64;
	mat[10] = (Cos(a)*Cos(b))/64;
	// skippes the calculation of the translation part				
	if (rotP.x == 0 && rotP.y == 0 && rotP.z == 0) {
		mat[3]  = 0;
		mat[7]  = 0;
		mat[11] = 0;
	} else {
		mat[3]  = rotP.x - ((mat[0]*(char)rotP.x)/64 + (mat[1]*(char)rotP.y)/64 + (mat[2]*(char)rotP.z)/64);
		mat[7]  = rotP.y - ((mat[4]*(char)rotP.x)/64 + (mat[5]*(char)rotP.y)/64 + (mat[6]*(char)rotP.z)/64);
		mat[11] = rotP.z - ((mat[8]*(char)rotP.x)/64 + (mat[9]*(char)rotP.y)/64 + (mat[10]*(char)rotP.z)/64);
	}
	
	for (i = 0; i < numPoint; i++) {
		resPoints[i] = mulMatrixPoint(mat, &points[i]);
	}	
}

/** Scales the numPoints many points points with the factors sx/64, sy/64, sz/64.
 *  Its scaled around the point scaleP and the result is stored in resPoints.
 */
void scale(char sx, char sy, char sz, pixel3d* points, 
			pixel3d* resPoints, int numPoint, pixel3d scaleP) {
	char mat[12] = {sx,  0,  0,  scaleP.x - (sx*scaleP.x)/64,
					 0, sy,  0,  scaleP.y - (sy*scaleP.y)/64,
					 0,  0, sz,  scaleP.z - (sz*scaleP.z)/64};
	unsigned char i;
 	for (i = 0; i < numPoint; i++) {
		resPoints[i] = mulMatrixPoint(mat, &points[i]);
	}			
}				

#define BIT_S(var,b) ((var&(1<<b))?1:0)

/** An very easy but useful randomgenrator for randomAnimaions.
 *  It returns a pseudorandom number between 0 and 255.
 */
unsigned char easyRandom() {
	static unsigned int muh = 0xAA;
	unsigned char x;
	for (x = 0; x < 8 ;x++) {
		muh = (muh<<1) ^ BIT_S(muh,1) ^ BIT_S(muh,8) ^ BIT_S(muh,9) ^ BIT_S(muh,13) ^ BIT_S(muh,15);
	}
	return (unsigned char) muh;
}

/** Draws a thredimentional line with the bressenham 3d algrorthmus form the point
  * px1, py1, xz1 to the point px2, py2, pz2 with the brightness value.
  */
void drawLine3D(char px1, char py1, char pz1, 
 			    char px2, char py2, char pz2, unsigned char value) {
    char i, dx, dy, dz, l, m, n, x_inc, y_inc, z_inc, err_1, err_2, dx2, dy2, dz2;
    char curx = px1, cury = py1, curz = pz1;
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
            setpixel3d((pixel3d) {curx, cury, curz}, value);
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
            setpixel3d((pixel3d) {curx, cury, curz}, value);
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
            setpixel3d((pixel3d) {curx, cury, curz}, value);
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
   	setpixel3d((pixel3d) {curx, cury, curz}, value);
}
