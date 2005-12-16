#include "pixel3d.h"

unsigned char shl_table[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

void clear_screen(unsigned char value){
	unsigned char p, x, y, v=0xFF;
	for(p=0; p<NUM_LEVELS; p++) {
		if(p == value) v=0;
		for(y=0;y<NUM_PLANES;y++){
			for(x=0;x<PLANEBYTES ;x++){
				pixmap[p][y][x] = v;
			}
		}
	}
}

void setpixel3d(pixel3d p, unsigned char value ){
	unsigned char plane;
	
	for (plane=0; plane < NUM_LEVELS; plane++) {
		if (plane < value)
			pixmap[plane][p.x%NUM_PLANES][p.y%PLANEBYTES] |=  shl_table[p.z%NUM_ROWS];
		else
			pixmap[plane][p.x%NUM_PLANES][p.y%PLANEBYTES] &= ~shl_table[p.z%NUM_ROWS];
	}
}

void shift3d(direction dir) {
     unsigned char i, j, k;
     switch (dir) {
     case right:
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
     case left:
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
     case forward:
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
     case back:
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
		case right:
			tmp = (pixel3d){p.x+1, p.y, p.z};
			break;
		case left:
			tmp = (pixel3d){p.x-1, p.y, p.z};
			break;
		case up:
			tmp = (pixel3d){p.x, p.y, p.z+1};
			break;	
		case down:
			tmp = (pixel3d){p.x, p.y, p.z-1};
			break;
		case forward:
			tmp = (pixel3d){p.x, p.y+1, p.z};
			break;
		case back:
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
		case right:
			return((pixel3d){pix.x+1, pix.y, pix.z});
			break;
		case left:
			return((pixel3d){pix.x-1, pix.y, pix.z});
			break;
		case down:
			return((pixel3d){pix.x, pix.y, pix.z-1});
			break;
		case up:
			return((pixel3d){pix.x, pix.y, pix.z+1});
			break;
		case back:
			return((pixel3d){pix.x, pix.y-1, pix.z});
			break;
		case forward:
			return((pixel3d){pix.x, pix.y+1, pix.z});
			break;
	}
	return (pixel3d){0,0,0};
}

void set_plane(direction dir, unsigned char num, unsigned char color)
{
	unsigned char pindex = 0;
	//Hardcore PORN!
	int p, y, x;
	unsigned char v = 0xFF;

	switch (dir) {
		
		//pixmap[p][rl][byte]
		case right:
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
			
		case left:
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
			
		case forward:
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
			
		case back:
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

// 127 = 90°
char sinTab[] = {0, 12, 25, 37, 49, 60, 71, 81, 90, 98, 
	             106, 112, 117, 122, 125, 126, 127};


char mySin(unsigned char a) {
	a %= 64;
	if (a < 17) {
		return sinTab[a];
	} else if (a < 33) {
		return sinTab[32-a];
	} else if (a < 49) {
		return -sinTab[a-32];
	} else {
		return -sinTab[64-a];
	}
}	

#define myCos(a) mySin((a)+16)

void rotate(unsigned char alpha, unsigned char beta, unsigned char gamma, unsigned char* obj) {

	
}
