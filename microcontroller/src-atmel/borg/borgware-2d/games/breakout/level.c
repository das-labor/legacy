/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 * Author & Copyright (C) 2010: Soeren Heisrath (forename@surename.org)
 *
 */

#include "level.h"

static uint16_t maxscore;

/* real level definition */
game_field_t level_field (uint8_t in_x, uint8_t in_y, uint8_t in_lvl);
game_field_t level_field (uint8_t in_x, uint8_t in_y, uint8_t in_lvl)
{
	switch (in_lvl)
	{
		case 0:
			/* space for the lower half of the level */
			if (in_y > (NUM_ROWS / 3))
				return sp;

			return b1; /* b1-blocks for the rest */
		case 1:
			/* space for the lower half of the level */
			if (in_y > (NUM_ROWS / 2))
				return sp;
			
			/* type 2 bricks for 1/4th of the field */
			if (in_y <= (NUM_ROWS / 4))
				return b2;
			
			/* fill the rest with type 1 */
			return b1;
		break;
		
		case 3:
			/* add a row of solid bricks right in the middle of the field */
			if (in_y == (NUM_ROWS / 2) &&
				(in_x > (NUM_COLS / 4)) && (in_x < (NUM_COLS - (NUM_COLS / 4))))
				return bs;

			/* intentional fallthrough: the rest of level 3 is like level 2 */

		case 2:
			/* space for the lower third of the level */
			if (in_y > (NUM_ROWS / 3))
				return sp;
			
			/* type 3 bricks for 1/8th of the field */
			if (in_y <= (NUM_ROWS / 8))
				return b3;
			
			/* type 2 bricks for 1/4th of the field */
			if (in_y <= (NUM_ROWS / 4))
				return b2;
			
			/* fill the rest with type 1 */
			return b1;

		default: /* random level generation */
			/* space for the lower half of the level */
			if (in_y > (NUM_ROWS / 2))
				return sp;
			
			return random8() & 0x03; /* fill field with random bricks (and spaces) */
		break;
	}
}

void level_init (uint8_t in_levelnum)
{
	uint8_t x,y;
	game_field_t tmp;
	maxscore = 0;

	for (x=0;x<NUM_COLS;x++)
	{
		for (y=0;y<NUM_ROWS;y++)
		{
			tmp = level_field (x, y, in_levelnum);
			playfield_set (x,y, tmp);
			if (tmp <= b3)
				maxscore += tmp;
		}
	}

	maxscore += score_get();
}

uint16_t level_getscorediff ()
{
	return maxscore - score_get();
}
