/* maze.hh */
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

#ifndef MAZE_HH_
#define MAZE_HH_

#include <stdint.h>
#include <SDL/SDL.h>

typedef uint8_t wallstate_t;
typedef int    cellstate_t;
typedef enum {east_ot,north_ot,west_ot,south_ot} orientation_t;

class maze {
public:
	maze(unsigned xsize, unsigned ysize);
	~maze();
	unsigned getWidth();
	unsigned getHeight();
	wallstate_t getWalls(unsigned cx, unsigned cy);
	void setWalls(unsigned cx, unsigned cy, wallstate_t value);
	cellstate_t getCell(unsigned cx, unsigned cy);
	void setCell(unsigned cx, unsigned cy, cellstate_t value);
	void setRandom(unsigned cx, unsigned cy);
	void setRandom();
	void reset();
	void print(FILE* f);
	void printnum(FILE* f);
protected:
	const unsigned width, height;
	uint8_t *vwalls, *hwalls;
	cellstate_t *cells;
	bool getVWall(unsigned pos);
	bool getHWall(unsigned pos);
	void setVWall(unsigned pos, bool v);
	void setHWall(unsigned pos, bool v);
	bool getWall(unsigned cx, unsigned cy, orientation_t o);
	void setWall(unsigned cx, unsigned cy, orientation_t o, bool v);
};


#endif /* MAZE_HH_ */
