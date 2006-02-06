#include "pixel3d.h"

unsigned char shl_table[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

void clear_screen(unsigned char value){
	unsigned char p, x, y, v=0xFF;
	for(p=0; p<NUM_LEVELS; p++) {
		if(p == value) v=0;
		for(y=0; y<NUM_PLANES; y++){
			for(x=0; x<PLANEBYTES; x++){
				pixmap[p][y][x] = v;
			}
		}
	}
}

void setpixel3d(pixel3d p, unsigned char value ){
	unsigned char plane;
	if (p.x < 8 && p.y < 8 && p.z < 8) { 
		for (plane=0; plane < NUM_LEVELS; plane++) {
			if (plane < value)
				pixmap[plane][p.x][p.y] |=  shl_table[p.z];
			else
				pixmap[plane][p.x][p.y] &= ~shl_table[p.z];
		}
	}	
}

void shift3d(direction dir) {
     unsigned char i, j, k;
     switch (dir) {
     case back:
          for (i = 1; i < NUM_PLANES; i ++) {
              for (j = 0; j < NUM_ROWS; j++) {
                  for (k = 0; k < NUM_LEVELS; k++) {
                      pixmap[k][i-1][j] = pixmap[k][i][j]; 
                  }
              }
          }
          for (j = 0; j < NUM_ROWS; j++) {
              for (k = 0; k < NUM_LEVELS; k++) {
                  pixmap[k][NUM_PLANES-1][j] = 0; 
              }
          }
          break;
     case forward:
          for (i = NUM_PLANES-2; i < NUM_PLANES; i--) {
              for (j = NUM_ROWS-1; j < NUM_ROWS; j--) {
                  for (k = 0; k < NUM_LEVELS; k++) {
                      pixmap[k][i+1][j] = pixmap[k][i][j]; 
                  }
              }
          }
          for (j = NUM_ROWS-1; j < NUM_ROWS; j--) {
              for (k = 0; k < NUM_LEVELS; k++) {
                  pixmap[k][0][j] = 0;
              }
          }
          break;
     case right:
          for (i = 0; i < NUM_PLANES; i ++) {
              for (j = 1; j < NUM_ROWS; j++) {
                  for (k = 0; k < NUM_LEVELS; k++) {
                      pixmap[k][i][j-1] = pixmap[k][i][j]; 
                  }
              }
          }
          for (j = 0; j < NUM_PLANES; j++) {
              for (k = 0; k < NUM_LEVELS; k++) {
                  pixmap[k][j][NUM_PLANES-1] = 0; 
              }
          }
          break;
     case left:
          for (i = NUM_PLANES-1; i < NUM_PLANES; i--) {
              for (j = NUM_ROWS-2; j < NUM_ROWS; j--) {
                  for (k = 0; k < NUM_LEVELS; k++) {
                      pixmap[k][i][j+1] = pixmap[k][i][j]; 
                  }
              }
          }
          for (j = NUM_PLANES-1; j < NUM_ROWS; j--) {
              for (k = 0; k < NUM_LEVELS; k++) {
                  pixmap[k][j][0] = 0;
              }
          }
          break;
     case up:
          for (i = 0; i < NUM_PLANES; i ++) {
              for (j = 0; j < NUM_ROWS; j++) {
                  for (k = 0; k < NUM_LEVELS; k++) {
                      pixmap[k][i][j] = pixmap[k][i][j] << 1; 
                  }
              }
          }
          break;
     case down:
          for (i = 0; i < NUM_PLANES; i ++) {
              for (j = 0; j < NUM_ROWS; j++) {
                  for (k = 0; k < NUM_LEVELS; k++) {
                      pixmap[k][i][j] = pixmap[k][i][j] >> 1;                   
                  }
              }
          }
          break;
     }     
}

unsigned char get_pixel3d(pixel3d p){

	if ((p.x > (NUM_ROWS-1)) || (p.y > (NUM_ROWS-1)) || (p.z > (NUM_ROWS-1))) {
		return 0xff;
	} else {
		return (pixmap[0][p.x%NUM_PLANES][p.y%PLANEBYTES] & shl_table[p.z%8]) ? 1:0;
	}
}

unsigned char get_next_pixel3d(pixel3d p, direction dir){
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

direction direction_r(direction dir){
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

pixel3d next_pixel3d(pixel3d pix, direction dir){
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

direction turn_down(direction dir){
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

void set_plane(direction dir, unsigned char num, unsigned char color)
{
	unsigned char pindex = 0;
	int p, y, x;
	unsigned char v = 0xFF;

	switch (dir) {
		
		//pixmap[p][rl][byte]
		case back:
			pindex = NUM_PLANES-(num+1);
			for(x=0;x<PLANEBYTES ;x++) {
				for(p=0; p<NUM_LEVELS; p++) {
					if ( p < color)
						pixmap[p][pindex][x] = v;
					else
						pixmap[p][pindex][x] &= ~v;
				}
			 }			
			break;
			
		case forward:
			 pindex = num;			 
			 for(x=0;x<PLANEBYTES ;x++) {
				for(p=0; p<NUM_LEVELS; p++) {
					if ( p < color)
						pixmap[p][pindex][x] = v;
					else
						pixmap[p][pindex][x] &= ~v;
				}
			 }
			break;
			
		case right:
			pindex = NUM_PLANES-(num+1);
			for(y=0;y<NUM_PLANES ;y++) {
				for(p=0; p<NUM_LEVELS; p++) {
					if ( p < color)
						pixmap[p][y][pindex] = v;
					else
						pixmap[p][y][pindex] &= ~v;				
				}
			 }
			break;
			
		case left:
			pindex = num;
			for(y=0;y<NUM_PLANES ;y++) {
				for(p=0; p<NUM_LEVELS; p++) {
					if ( p < color)
						pixmap[p][y][pindex] = v;
					else
						pixmap[p][y][pindex] &= ~v;				
				}
			 }
			break;
			
		case down:
			v = shl_table[NUM_ROWS - (num+1)];
			for(p=0; p<NUM_LEVELS; p++) {
					for(y=0; y<NUM_PLANES ;y++) {
						for(x=0; x<PLANEBYTES ;x++) {
							if ( p < color)
								pixmap[p][y][x] |= v;
							else
								pixmap[p][y][x] &= ~v;				
					}
				}
			}
			break;
			
		case up:
			v = shl_table[num];
			for(p=0; p<NUM_LEVELS; p++) {
					for(y=0; y<NUM_PLANES ;y++) {
						for(x=0; x<PLANEBYTES ;x++) {
							if ( p < color)
								pixmap[p][y][x] |= v;
							else
								pixmap[p][y][x] &= ~v;				
					}
				}
			}
			break;
	} //end switch(dir)
}

// 64 = sin(90°) = 1
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


pixel3d mulMatrixPoint(char *mat, pixel3d *p) {
	return (pixel3d) {
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
void rotate(char a, char b, char c, pixel3d* points, 
			pixel3d* resPoints, int numPoint, pixel3d rotP) {
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

unsigned char easyRandom() {
	static unsigned int muh = 0xAA;
	unsigned char x;
	for(x=0;x<8;x++){
		muh = (muh<<1) ^ BIT_S(muh,1) ^ BIT_S(muh,8) ^ BIT_S(muh,9) ^ BIT_S(muh,13) ^ BIT_S(muh,15);
	}
	return (unsigned char) muh;
}

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
