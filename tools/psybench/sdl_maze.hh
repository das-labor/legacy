/* sdl_maze.hh */
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

#ifndef SDL_MAZE_HH_
#define SDL_MAZE_HH_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "play_maze.hh"
#include "sdl_helper.h"

class sdl_maze : public play_maze {
public:
	sdl_maze(unsigned xsize, unsigned ysize, SDL_Surface *sf=NULL,
			 unsigned size=20, unsigned pensize=5,  uint32_t fgcolor=0x000000FF, uint32_t bgcolor=0);
	SDL_Surface *getSurface();
	void load_player_image_from_file(const char* filename);
	void load_target_image_from_file(const char* filename);
	void load_player_image_from_surface(SDL_Surface *sf);
	void load_target_image_from_surface(SDL_Surface *sf);
	uint32_t getfgcolor();
	void setfgcolor(uint32_t color);
	uint32_t getbgcolor();
	void setbgcolor(uint32_t color);
	unsigned getSize();
	unsigned getPensize();
	void render_maze();
	void render_game();
	bool move(orientation_t o);
protected:
	SDL_Surface *surface;
	SDL_Surface *target_img, *player_img;
	const unsigned size, pensize;
	uint32_t fgcolor, bgcolor;
};

#endif /* SDL_MAZE_HH_ */
