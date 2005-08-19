
#define PIXEL_C
#include "pixel.h"

#include "borg_hw.h"

#include "util.h"
#include "config.h"

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

//shifts pixmap left. It is really shifted right, but because col0 is left in the Display it's left.
// may be incorect or not useful for borg-3d   not tested
void shift_pixmap_l(){
	unsigned char plane, row, byte;
	
	for(plane=0; plane<NUM_LEVELS; plane++){
		for(row=0;row<NUM_ROWS; row++){
			for(byte=0;byte<PLANEBYTES-1;byte++){
				pixmap[plane][row][byte] >>= 1;
				pixmap[plane][row][byte] |= (pixmap[plane][row][byte+1] & 0x01) * 0x80;;
			}
			pixmap[plane][row][PLANEBYTES-1] >>= 1;
		}
	}
}


unsigned char get_pixel3d(pixel3d p){

	if ((p.x > (NUM_COLS-1)) || (p.y> (NUM_ROWS-1))) {
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
	if ((tmp.x>NUM_ROWS-1) || (tmp.y>NUM_ROWS-1) || (tmp.z>NUM_ROWS-1)) {
		return 0xFF;
	} else {
		return get_pixel3d(tmp);
	}
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
		case forward:
			return right;					
	}
	return 0;
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
