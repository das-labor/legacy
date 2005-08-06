
#define PIXEL_C
#include "pixel.h"

#include "util.h"

unsigned char shl_table[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};


void clear_screen(unsigned char value){
	unsigned char x;
	unsigned char plane;
	for(plane=0; plane<NUMPLANE; plane++) {
		for(x=0;x<8;x++){
			pixmap[plane][x] = 0;
		}
	}
}

void setpixel(pixel p, unsigned char value ){
	signed char plane;

	for(plane=0; plane<NUMPLANE; plane++) {
		if ( plane < value )
			pixmap[plane][p.y%8] |= shl_table[p.x%8];
		else
			pixmap[plane][p.y%8] &= ~shl_table[p.x%8];
	}
}

void clearpixel(pixel p){
	unsigned char plane;
	for(plane=0; plane<NUMPLANE; plane++)
		pixmap[plane][p.y%8] &= ~shl_table[p.x%8];
}

unsigned char get_pixel(pixel p){

	if( (p.x > 7) || (p.y> 7 ) ){
		return 0xff;
	}else{
		return ( (pixmap[0][p.y] & shl_table[p.x]) ? 1:0);
	}
}


unsigned char get_next_pixel(pixel p, direction dir){
	pixel tmp;
	switch (dir){
		case right:
			tmp = (pixel){p.x+1, p.y};
			break;
		case left:
			tmp = (pixel){p.x-1, p.y};
			break;	
		case down:
			tmp = (pixel){p.x, p.y+1};
			break;
		case up:
			tmp = (pixel){p.x, p.y-1};
			break;
	}
	return get_pixel(tmp);
}

direction direction_r(direction dir){
	switch (dir){
			case right:
				return(down);
			case down:
				return(left);
			case left:
				return(up);
			case up:
				return (right);	
		}
	return(0);
}

void set_cursor(cursor* cur, pixel p){
	cur->pos = p;
	switch (cur->mode){
		case clear:
			clearpixel(p);
			break;
		case set:
			setpixel(p,3);
			break;
	}
}

pixel next_pixel(pixel pix, direction dir){
	switch (dir){
			case right:
				return((pixel){pix.x+1, pix.y});
				break;
			case left:
				return((pixel){pix.x-1, pix.y});
				break;
			case down:
				return((pixel){pix.x, pix.y+1});
				break;
			case up:
				return((pixel){pix.x, pix.y-1});
				break;
	
			}
	return (pixel){0,0};
}

void walk(cursor* cur, unsigned char steps, unsigned int delay){
	unsigned char x;
	for(x=0;x<steps;x++){
		set_cursor(cur, next_pixel(cur->pos, cur->dir));
		wait(delay);
	}
}
