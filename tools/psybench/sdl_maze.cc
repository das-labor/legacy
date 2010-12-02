/* sdl_maze.cc */
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

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdint.h>
#include "sdl_maze.hh"
#include "sdl_helper.h"

#define XCHG(a,b) (a)^=(b); (b)^=(a); (a)^=(b)

sdl_maze::sdl_maze(unsigned xsize, unsigned ysize, SDL_Surface *sf,
		           unsigned size, unsigned pensize,
		           uint32_t fgcolor, uint32_t bgcolor)
:	 	play_maze(xsize, ysize), size(size), pensize(pensize),
 	 	fgcolor(fgcolor), bgcolor(bgcolor)
{
	if(!sf){
		surface=SDL_CreateRGBSurface(SDL_HWSURFACE,width*(size+pensize)+pensize,height*(size+pensize)+pensize, 32, 0,0,0,0);
		if(!surface){
			fprintf(stderr,"Error: could not create surface (line %d in %s): %s", __LINE__, __FILE__, SDL_GetError());
			return;
		}
	}else{
		surface = sf;
	}

}

void sdl_maze::load_player_image_from_file(const char* filename){
	player_img = load_image(filename);
	if(player_img){
	//	SDL_SetColorKey(player_img,SDL_SRCCOLORKEY, SDL_MapRGB(player_img->format,0,0,0));
	}
}
void sdl_maze::load_target_image_from_file(const char* filename){
	target_img = load_image(filename);
	if(target_img){
	//	SDL_SetColorKey(player_img,SDL_SRCCOLORKEY, SDL_MapRGB(target_img->format,0,0,0));
	}
}
void sdl_maze::load_player_image_from_surface(SDL_Surface *sf){
	player_img = sf;
}
void sdl_maze::load_target_image_from_surface(SDL_Surface *sf){
	target_img = sf;
}

SDL_Surface *sdl_maze::getSurface(){
	return surface;
}


uint32_t sdl_maze::getfgcolor(){
	return fgcolor;
}

void sdl_maze::setfgcolor(uint32_t color){
	fgcolor = color;
}

uint32_t sdl_maze::getbgcolor(){
	return bgcolor;
}

void sdl_maze::setbgcolor(uint32_t color){
	bgcolor = color;
}

unsigned sdl_maze::getSize(){
	return size;
}

unsigned sdl_maze::getPensize(){
	return pensize;
}

void sdl_maze::render_maze(){
	unsigned x, y, i;
	SDL_Rect rect;
	/* draw outer frame */
	  /* define outer frame */
	SDL_Rect frame;
	frame.w = width*(size+pensize)+pensize;
	frame.h = height*(size+pensize)+pensize;
	frame.x = 0;
	frame.y = 0;
	SDL_FillRect(surface, &frame, bgcolor);
	  /* draw top line */
	rect.x = frame.x;
	rect.w = frame.w;
	rect.y = frame.y;
	rect.h = pensize;
	SDL_FillRect(surface, &rect, fgcolor);
	  /* draw bottom line */
	rect.y += frame.h - pensize;
	SDL_FillRect(surface, &rect, fgcolor);
      /* draw left line */
	rect.y = frame.y ;
	XCHG(rect.w, rect.h);
	SDL_FillRect(surface, &rect, fgcolor);
	  /* draw right line */
	rect.x += frame.w - pensize;
	SDL_FillRect(surface, &rect, fgcolor);
	/* draw maze */
	  /* draw vertical walls */
	rect.w = pensize;
	rect.h = size+pensize*2;
	i=0;
	for(y=0; y<height; ++y){
		rect.y = frame.y+y*(size+pensize);
		rect.x = frame.x + size + pensize;
		for(x=0; x<width-1; ++x){
			if(getVWall(i++)){
				SDL_FillRect(surface, &rect, fgcolor);
			}
			rect.x += size + pensize;
		}
	}
      /* draw horizontal walls */
	XCHG(rect.w, rect.h);
	i=0;
	for(y=1; y<height; ++y){
		rect.y = frame.y+y*(size+pensize);
		rect.x = frame.x;
		for(x=0; x<width; ++x){
			if(getHWall(i++)){
				SDL_FillRect(surface, &rect, fgcolor);
			}
			rect.x += size + pensize;
		}
	}
}

void sdl_maze::render_game(){
	SDL_Rect rect;
	if(player_img && player_set){
		rect.w = rect.h = size;
		rect.x=pensize+player_x*(size+pensize);
		rect.y=pensize+player_y*(size+pensize);
		SDL_BlitSurface(player_img, NULL, surface, &rect);
	}
	if(target_img && target_set){
		rect.w = rect.h = size;
		rect.x=pensize+target_x*(size+pensize);
		rect.y=pensize+target_y*(size+pensize);
		SDL_BlitSurface(target_img, NULL, surface, &rect);
	}
}

bool sdl_maze::move(orientation_t o){
	bool r;
	SDL_Rect rect;
	r=play_maze::valid_move(o);
	if(r){
		rect.w = rect.h = size;
		rect.x=pensize+player_x*(size+pensize);
		rect.y=pensize+player_y*(size+pensize);
		SDL_FillRect(surface, &rect, bgcolor);
		play_maze::move(o);
		render_game();
	}
	return r;
}
