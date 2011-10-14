#include "simpixel.h"
#include "config.h"
void setpixel(pixel p, unsigned char value ){
	if(value == 0){
		mvprintw(p.y,p.x," ");
	}
	if(value == 1){
		mvprintw(p.y,p.x,".");
	}
	if(value == 2){
		mvprintw(p.y,p.x,"o");
	}
	if(value == 3){
		mvprintw(p.y,p.x,"O");
	}

	refresh();
}
void clear_screen(unsigned char value){
	int x,y=0;
	if(value ==0){
		for(x=0;x<NUM_ROWS;x++){
			for(y=0;y<NUM_COLS;y++){
				mvprintw(y,x," ");
			}
		}
	}
	if(value == 1){
		for(x=0;x<NUM_ROWS;x++){
			for(y=0;y<NUM_COLS;y++){
				mvprintw(y,x,".");
			}
		}
	}
	if(value == 2){
		for(x=0;x<NUM_ROWS;x++){
			for(y=0;y<NUM_COLS;y++){
				mvprintw(y,x,"o");
			}
		}
	}
	if(value == 3){
		for(x=0;x<NUM_ROWS;x++){
			for(y=0;y<NUM_COLS;y++){
				mvprintw(y,x,"O");
			}
		}
	}

	refresh();
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
unsigned char get_pixel(pixel p){

	if( (p.x > (NUM_COLS-1)) || (p.y> (NUM_ROWS-1)) ){
		return 0xff;
	}else{
		int c = mvinch(p.y,p.x);
		if (c == ' '){
			return 0;
		}
		if	(c == '.'||c=='o'||c=='O'){
			return 1;
		}
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
