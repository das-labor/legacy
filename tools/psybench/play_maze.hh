/* play_maze.hh */
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

#ifndef PLAY_MAZE_HH_
#define PLAY_MAZE_HH_

#include "maze.hh"

class play_maze : public maze{
public:
	play_maze(unsigned xsize, unsigned ysize);
	bool game_finished();
	bool move(orientation_t o);
	bool valid_move(orientation_t o);
	void set_player_pos(int x=-1, int y=-1);
	void set_target_pos(int x=-1, int y=-1);
protected:
	unsigned player_x, player_y;
	unsigned target_x, target_y;
	bool player_set;
	bool target_set;
};

#endif /* PLAY_MAZE_HH_ */
