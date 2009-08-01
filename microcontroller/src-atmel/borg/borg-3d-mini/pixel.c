#define PIXEL_C
#include "pixel.h"
#include "util.h"

unsigned char shl_table[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

void clear_screen(unsigned char value){
	unsigned char z, y;
	unsigned char brith;
	for (brith = 0; brith < 3; brith++) {
		for (z = 0; z < 3; z++) {
			for (y = 0; y < 6; y++) {
				pixmap[brith][z][y] = 0;
			}
		}
	}
}

void setpixel3d(pixel3d p, unsigned char value ) {
	signed char brith;

	for (brith = 0; brith < 3; brith++) {
		if ( brith < value ) { 
			pixmap[brith][p.z][p.y] |=  shl_table[p.x];
		} else {
			pixmap[brith][p.z][p.y] &= ~shl_table[p.x];
		}
	}
}

void clearpixel3d(pixel3d p){
	unsigned char plane;
	for (plane=0; plane<NUMPLANE; plane++)
		pixmap[plane][p.z][p.y] &= ~shl_table[p.x];
}

unsigned char get_pixel3d(pixel3d p){ // XXX
	return (pixmap[0][p.z][p.y] & shl_table[p.x])?1:0;
}

unsigned char get_next_pixel3d(pixel3d p, direction dir) {
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
	if ((tmp.x>2) || (tmp.y>2) || (tmp.z>2)) {
		return 0xFF;
	} else {
		return get_pixel3d(tmp);
	}
}

direction direction_r(direction dir){
	switch (dir) {
		case right:
			return(back);
		case down:
			return(left);
		case left:
			return(up);
		case up:
			return (forward);	
		case back:
			return (down);
		case forward:
			return (right);				
	}
	return(0);
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

