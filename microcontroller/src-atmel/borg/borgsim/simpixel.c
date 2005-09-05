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
	return (char)get_pixel(tmp);
}
unsigned char get_pixel(pixel p){

	if( (p.x > (NUM_COLS-1)) || (p.y> (NUM_ROWS-1)) ){
		return 0xff;
	}else{
		int c = mvgetch(p.y,p.x);
		if (c == ' '){
			return 0;
		}
		if	(c == '.'){
			return 1;
		}
		if (c == 'o'){
			return 2;
		}
		if (c == 'O'){
			return 3;
		}
	}
}
