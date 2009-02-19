#define PIXEL_C

#include "config.h"

#include "pixel.h"
#include "borg_hw/borg_hw.h"

unsigned char shl_table[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

void clear_screen(unsigned char value){
	unsigned char p,*pix,v=0xff;
	unsigned int i;
	for(p=0;p<NUMPLANE;p++){
		pix=&pixmap[p][0][0];
		if(p==value)
			v=0;
		for(i=0;i<NUM_ROWS*LINEBYTES;i++)
			pix[i]=v;
	}
}

void setpixel(pixel p, unsigned char value ){
	p.x %= NUM_COLS;
	if(value>NUMPLANE)
		value=NUMPLANE;
	unsigned char pos = (p.y%NUM_ROWS)*LINEBYTES + (p.x/8);
	unsigned char mask = shl_table[p.x%8];
	unsigned char plane;
	for(plane=0;plane<value;plane++)
		pixmap[plane][0][pos]|=mask;
	mask ^=0xff;
	for(;plane<NUMPLANE;plane++)
		pixmap[plane][0][pos]&=mask;
}


void line(pixel p1, pixel p2 ,unsigned char value){
	char dx, dy, stepx, stepy, fraction;
    
        dy = p2.y - p1.y;
    	dx = p2.x - p1.x;

    if ( dy < 0 )
    {
        dy    = -dy;
        stepy = -1;
    }
    else
    {
        stepy = 1;
    }

    if ( dx < 0 )
    {
        dx    = -dx;
        stepx = -1;
    }
    else
    {
        stepx = 1;
    }

    dx <<= 1;
    dy <<= 1;

     setpixel(p1 , value );
    if ( dx > dy )
    {
        fraction = dy - (dx >> 1);
        while ( p1.x != p2.x )
        {
            if ( fraction >= 0 )
            {
                p1.y += stepy;
                fraction -= dx;
            }
            p1.x += stepx;
            fraction += dy;
            setpixel(p1 , value );
        }
    }
    else
    {
        fraction = dx - (dy >> 1);
        while ( p1.y != p2.y )
        {
            if ( fraction >= 0 )
            {
                p1.x += stepx;
                fraction -= dy;
            }
            p1.y += stepy;
            fraction += dx;
             setpixel(p1 , value );
            
        }
    }
}


void filled_rectangle(pixel p1, unsigned char w, unsigned char h ,unsigned char value){

unsigned char y;

for (y=p1.y;y<(p1.y+h);y++){
    line((pixel){p1.x,y}, (pixel){(p1.x+w-1),y} ,value);

  }

}


//shifts pixmap left. It is really shifted right, but because col0 is left in the Display it's left.
void shift_pixmap_l(){
	unsigned char plane, row, byte;
	
	for(plane=0; plane<NUMPLANE; plane++){
		for(row=0;row<NUM_ROWS; row++){
			for(byte=0;byte<(LINEBYTES-1);byte++){
				pixmap[plane][row][byte] >>= 1;
				pixmap[plane][row][byte] |= (pixmap[plane][row][byte+1] & 0x01) * 0x80;;
			}
				pixmap[plane][row][LINEBYTES-1] >>= 1;
		}
	}
}


unsigned char get_pixel(pixel p){

	if( (p.x >= NUM_COLS) || (p.y >= NUM_ROWS) ){
		return 0xff;
	}else{
		return 0!= (pixmap[0][p.y][p.x/8] & shl_table[p.x%8]);
	}
}


unsigned char get_next_pixel(pixel p, direction dir){
	pixel tmp;
	switch (dir){
		case right:
			tmp = (pixel){p.x-1, p.y};
			break;
		case left:
			tmp = (pixel){p.x+1, p.y};
			break;	
		case down:
			tmp = (pixel){p.x, p.y+1};
			break;
		case up:
			tmp = (pixel){p.x, p.y-1};
			break;
		default:
			tmp = p;
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
				return((pixel){pix.x-1, pix.y});
				break;
			case left:
				return((pixel){pix.x+1, pix.y});
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
