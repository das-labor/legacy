#include "pixel3d.h"

void clear_screen(unsigned char value){
	int x;
	for (x = 0; x < BORGLEDS; x++) {
		data[x] = value;
	}
}

void setpixel3d(pixel3d p, unsigned char value ) {
	data[p.x+BORGSIZE*p.y+BORGSIZE*BORGSIZE*p.z] = value;
}

void clearpixel3d(pixel3d p){
	data[p.x+BORGSIZE*p.y+BORGSIZE*BORGSIZE*p.z] = 0;
}

unsigned char get_pixel3d(pixel3d p){ // XXX
	return data[p.x+BORGSIZE*p.y+BORGSIZE*BORGSIZE*p.z];	
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
	if ((tmp.x>BORGSIZE-1) || (tmp.y>BORGSIZE-1) || (tmp.z>BORGSIZE-1)) {
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

void wait(unsigned int ms) {
	usleep(ms*1000);
}
