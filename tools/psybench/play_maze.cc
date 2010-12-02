/* play_maze.cc */
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

#include <stdlib.h>
#include "play_maze.hh"

play_maze::play_maze(unsigned xsize, unsigned ysize)
: maze(xsize, ysize), player_set(false), target_set(false)
{
}

bool play_maze::valid_move(orientation_t o){
	if(!player_set){
		return false;
	}
	wallstate_t w;
	w = getWalls(player_x, player_y);
	return !(w&(1<<o));
}

bool play_maze::move(orientation_t o){
	if(!valid_move(o)){
		return false;
	}
	switch(o){
		case north_ot: player_y--; break;
		case south_ot: player_y++; break;
		case east_ot:  player_x++; break;
		case west_ot:  player_x--; break;
		default : break;
	}
	return true;
}

bool play_maze::game_finished(){
	if((!player_set)||(!target_set)){
		return false;
	}
	if((player_x == target_x)&&(player_y == target_y)){
		return true;
	}
	return false;
}

void play_maze::set_player_pos(int x, int y){
	if(x<0){
		x = random()%width;
	}
	if(y<0){
		y = random()%height;
	}
	player_x = x;
	player_y = y;
	player_set = true;
}

void play_maze::set_target_pos(int x, int y){
	if(x<0){
		x = random()%width;
	}
	if(y<0){
		y = random()%height;
	}
	target_x = x;
	target_y = y;
	target_set = true;
}
