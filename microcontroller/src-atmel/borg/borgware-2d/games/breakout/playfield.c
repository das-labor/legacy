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

#include "playfield.h"
game_field_t (*playfield)[NUM_COLS][NUM_ROWS];

/* internal functions */

/* this is the actual draw function for a single field
 */
static void draw_single_field (uint8_t in_x, uint8_t in_y, game_field_t in_f)
{
	pixel tmp;
	uint8_t b;
	switch (in_f)
	{
		case b1:
			b = 1;
		break;

		case rb:
		case b2:
			b = 2;
		break;

		case b3:
		case bl:
		case bs:
			b = 3;
		break;

		default: /* this includes freespace */
			b = 0;
		break;

	}
	tmp.x = in_x;
	tmp.y = in_y;
	setpixel (tmp, b);
}

static void brick_damage (int8_t in_x, int8_t in_y)
{
	if ((*playfield)[in_x][in_y] >= bs || (*playfield)[in_x][in_y] == 0)
		return;

	(*playfield)[in_x][in_y] -= 1;
	score_add (1);
}

/* interface functions */

void playfield_set (uint8_t in_x, uint8_t in_y, game_field_t in_field)
{
	if (in_x >= NUM_ROWS || in_y >= NUM_COLS)
	{
		return;
	}
	(*playfield)[in_x][in_y] = in_field;
}

int8_t check_bounce (int8_t in_x, int8_t in_y)
{
	int8_t ov = 0;
	/* overflow check */
	if (in_x >= NUM_ROWS || in_x < 0)
		ov |= BOUNCE_X;
	
	if (in_y >= NUM_COLS || in_y < 0)
		ov |= BOUNCE_Y;
	
	if (ov)
	{
		return ov;
	}

	/* collisions with real objects */
	switch ((*playfield)[abs(in_x)][abs(in_y)])
	{
		case b2:
		case b3:
		case b1:
			brick_damage (in_x, in_y);
		/* intentional fallthrough */
		case bs:
			ov |= BOUNCE_BRICK;
			break;
		
		/* bouncing on the rebound needs special care */
		case rb:
			ov |= BOUNCE_Y | BOUNCE_REBOUND;
			break;
		
		case sp:
		case bl:
		default:
			break;
		
	}
	return ov;
}

void playfield_draw ()
{
	uint8_t x,y;

	for (x=0;x<NUM_ROWS;x++)
	{
		for (y=0;y<NUM_COLS;y++)
		{
			draw_single_field (x,y, (*playfield)[x][y]);
		}
	}
}
