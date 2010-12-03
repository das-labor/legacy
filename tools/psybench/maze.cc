/* maze.cc */
/*
    This file is part of the PsyBench.
    Copyright (C) 2010-2010 Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "maze.hh"

#define CELL_UNSET   -1
#define CELL_INVALID -2

/*
     1
    +-+
  2 | | 0
    +-+
     3
*/

#define WALL_LEFTBORDER   0x44
#define WALL_RIGHTBORDER  0x11
#define WALL_TOPBORDER    0x22
#define WALL_BOTTOMBORDER 0x88

#define XCHG(a,b) (a)^=(b); (b)^=(a); (a)^=(b)

void maze::reset(){
	memset(cells,  0xFF, width*height*sizeof(cellstate_t));
	memset(vwalls, 0xFF, (height*(width-1)+7)/8*sizeof(uint8_t));
	memset(hwalls, 0xFF, (width*(height-1)+7)/8*sizeof(uint8_t));
}

maze::maze(unsigned xsize, unsigned ysize)
: width(xsize), height(ysize)
{
	cells =  new cellstate_t [xsize*ysize];
	vwalls = new uint8_t [(ysize*(xsize-1)+7)/8];
	hwalls = new uint8_t [(xsize*(ysize-1)+7)/8];
	if(!cells){
		fprintf(stderr, "Error: could not allocate memory for maze cells\n");
	}
	if(!vwalls){
		fprintf(stderr, "Error: could not allocate memory for maze vertical walls\n");
	}
	if(!hwalls){
		fprintf(stderr, "Error: could not allocate memory for maze horizontal walls\n");
	}
	reset();
}

maze::~maze(){
	delete[] cells;
	delete[] vwalls;
	delete[] hwalls;
}

unsigned maze::getHeight(){
	return height;
}

unsigned maze::getWidth(){
	return width;
}

cellstate_t maze::getCell(unsigned cx, unsigned cy){
	if(cx>=width || cy>=height){
		return CELL_INVALID;
	}
	return cells[cx+cy*width];
}

void maze::setCell(unsigned cx, unsigned cy, cellstate_t value){
	if(cx>=width || cy>=height){
		return;
	}
	cells[cx+cy*width]=value;
}

bool maze::getHWall(unsigned pos){
	uint8_t t;
	t=hwalls[pos/8]&(1<<(pos%8));
	if(t){
		return true;
	}else{
		return false;
	}
}

void maze::setHWall(unsigned pos, bool v){
	if(v){
		hwalls[pos/8] |= 1<<(pos%8);
	}else{
		hwalls[pos/8] &= ~(1<<(pos%8));
	}
}

bool maze::getVWall(unsigned pos){
	uint8_t t;
	t=vwalls[pos/8]&(1<<(pos%8));
	if(t){
		return true;
	}else{
		return false;
	}
}

void maze::setVWall(unsigned pos, bool v){
	if(v){
		vwalls[pos/8] |= 1<<(pos%8);
	}else{
		vwalls[pos/8] &= ~(1<<(pos%8));
	}
}

bool maze::getWall(unsigned cx, unsigned cy, orientation_t o){
	if(cx==0 && o==west_ot){
		return true;
	}
	if(cx==width-1 && o==east_ot){
		return true;
	}
	if(cy==0 && o==north_ot){
		return true;
	}
	if(cy==height-1 && o==south_ot){
		return true;
	}
	if(o==north_ot){
		return getHWall((cy-1)*width+cx);
	}
	if(o==south_ot){
		return getHWall(cy*width+cx);
	}
	if(o==west_ot){
		return getVWall(cy*(width-1)+cx-1);
	}
	if(o==east_ot){
		return getVWall(cy*(width-1)+cx);
	}
	fprintf(stderr, "Error: invalid read at (%d, %d, %d)",cx,cy,o);
	return true;
}

void maze::setWall(unsigned cx, unsigned cy, orientation_t o, bool v){
	if(cx==0 && o==west_ot){
		return;
	}
	if(cx==width-1 && o==east_ot){
		return;
	}
	if(cy==0 && o==north_ot){
		return;
	}
	if(cy==height-1 && o==south_ot){
		return;
	}
	if(o==north_ot){
		return setHWall((cy-1)*width+cx,v);
	}
	if(o==south_ot){
		return setHWall(cy*width+cx, v);
	}
	if(o==west_ot){
		return setVWall(cy*(width-1)+cx-1, v);
	}
	if(o==east_ot){
		return setVWall(cy*(width-1)+cx, v);
	}
	fprintf(stderr, "Error: invalid write at (%d, %d, %d)",cx,cy,o);
}

wallstate_t maze::getWalls(unsigned cx, unsigned cy){
	wallstate_t r=0;
	if(cy>=width || cy>=height){
		return -1;
	}
	if(cx==0){
		r |= WALL_LEFTBORDER;
		if(getWall(cx,cy,east_ot)){
			r |= WALL_RIGHTBORDER & 0x0F;
		}
	}else{
		if(cx==width-1){
			r |= WALL_RIGHTBORDER;
			if(getWall(cx, cy, west_ot)){
				r |= WALL_LEFTBORDER & 0x0F;
			}
		}else{
			/* the general case */
			if(getWall(cx, cy, east_ot)){
				r |= WALL_RIGHTBORDER & 0x0F;
			}
			if(getWall(cx, cy, west_ot)){
				r |= WALL_LEFTBORDER & 0x0F;
			}
		}
	}
	if(cy==0){
		r |= WALL_TOPBORDER;
		if(getWall(cx, cy, south_ot)){
			r |= WALL_BOTTOMBORDER & 0x0F;
		}
	}else{
		if(cy==height-1){
			r |= WALL_BOTTOMBORDER;
			if(getWall(cx, cy, north_ot)){
				r |= WALL_TOPBORDER & 0x0F;
			}
		}else{
			/* the general case */
			if(getWall(cx, cy, south_ot)){
				r |= WALL_BOTTOMBORDER & 0x0F;
			}
			if(getWall(cx, cy, north_ot)){
				r |= WALL_TOPBORDER & 0x0F;
			}
		}
	}

	return r;
}

void maze::setWalls(unsigned cx, unsigned cy, wallstate_t value){
	if(cy>=width || cy>=height){
		return;
	}
	if(cx==0){
		setWall(cx, cy, east_ot, WALL_RIGHTBORDER & 0x0F & value);
	}else{
		if(cx==width-1){
			setWall(cx, cy, west_ot, WALL_LEFTBORDER & 0x0F & value);
		}else{
			setWall(cx, cy, east_ot, WALL_RIGHTBORDER & 0x0F & value);
			setWall(cx, cy, west_ot, WALL_LEFTBORDER & 0x0F & value);
		}
	}
	if(cy==0){
		setWall(cx, cy, south_ot, WALL_BOTTOMBORDER & 0x0F & value);
	}else{
		if(cy==height-1){
			setWall(cx, cy, north_ot, WALL_TOPBORDER & 0x0F & value);
		}else{
			/* the general case */
			setWall(cx, cy, south_ot, WALL_BOTTOMBORDER & 0x0F & value);
			setWall(cx, cy, north_ot, WALL_TOPBORDER & 0x0F & value);
		}
	}
}

void maze::setRandom(unsigned cx, unsigned cy){
	uint8_t w, t;
	setCell(cx,cy, 1);
	w = getWalls(cx,cy);
	t = w &(~(w>>4)) & 0x0F;
	if(t==0){
		return;
	}
	uint8_t order[]={0x00, 0x01, 0x02, 0x03};
	uint8_t rand;
	unsigned i,x,y;
	rand = random();
	if(rand&1){
		order[0] = 0x02;
		order[1] = 0x03;
		order[2] = 0x00;
		order[3] = 0x01;
	}
	rand >>= 1;
	if(rand&1){
		XCHG(order[0], order[1]);
	}
	rand >>= 1;
	if(rand&1){
		XCHG(order[2], order[3]);
	}
	rand >>= 1;
	if(rand&1){
		XCHG(order[1], order[2]);
	}
	rand >>= 1;
	if(rand&1){
		XCHG(order[0], order[3]);
	}
	for(i=0; i<4; ++i){
		x = cx; y = cy;
		w = getWalls(cx,cy);
		t = w &(~(w>>4)) & 0x0F;
		if(t&(1<<order[i])){
			if((order[i]&1)==0){
				x -= (order[i]&2)-1;
			}else{
				y += (order[i]&2)-1;
			}
			if(getCell(x,y)==-1){
				w &= ~(1<<order[i]);
				setWalls(cx,cy, w);
				setRandom(x,y);
			}
		}
	}
}

void maze::setRandom(){
	unsigned x,y;
	x = random() % width;
	y = random() % height;
	setRandom(x,y);
}

/*
     _ _ _ _ _ _ _
    |_|_|
    |
    |

*/
void maze::print(FILE* f){
	unsigned i=0,j=0,x,y;
	fprintf(f, "\nmaze:\n ");
	for(x=0;x<width;++x){
		fputc('_',f);
		fputc(' ',f);
	}
	fputc('\n', f);
	for(y=0; y<height-1; ++y){
		fputc('|', f);
		for(x=0; x<width-1; ++x){
			fputc(getHWall(i++)?'_':' ', f);
			fputc(getVWall(j++)?'|':' ', f);
		}
		fputc(getHWall(i++)?'_':' ', f);
		fputs("|\n", f);
	}
	fputc('|', f);
	for(x=0; x<width-1; ++x){
		fputc('_', f);
		fputc(getVWall(j++)?'|':' ', f);
	}
	fputc('_', f);
	fputs("|\n", f);
	fputc('\n', f);
	fflush(f);
}

void maze::printnum(FILE* f){
	unsigned x,y;
	wallstate_t w;
	static char hextab[]={'0', '1', '2', '3', '4', '5', '6', '7',
			              '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	fprintf(f, "\nmaze (numeric):\n");
	for(y=0; y<height; ++y){
		for(x=0; x<width; ++x){
			w = getWalls(x,y)&0x0F;
			fputc(hextab[w], f);
		}
		fputc('\n', f);
	}
}
