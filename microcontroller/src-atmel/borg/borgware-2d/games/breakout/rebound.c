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
#include "rebound.h"

static uint8_t rbpos;

void rebound_reflect (ball_t *b, int8_t in_x)
{
	uint8_t tmpidx;

	tmpidx = (in_x - rbpos) +1;

	printf("bounce idx %i\n", tmpidx);
	
	b->dir_x += rebound_reflection[tmpidx][0];
	b->dir_y += rebound_reflection[tmpidx][1];
}

uint8_t rebound_getpos ()
{
	return (rbpos + (REBOUND_SIZE / 2));
}

void rebound_init()
{
	rbpos = (NUM_ROWS / 2) - (REBOUND_SIZE / 2);
	rebound_draw();
}

void rebound_draw ()
{
	uint8_t i;

	for (i=rbpos;i<rbpos + REBOUND_SIZE;i++)
	{
		playfield_set (i, NUM_ROWS-1, rb); /* set rebound pixel */
	}
}

void rebound_tick()
{
	/* directions are inverted (JOYISLEFT means RIGHT) */
	if (JOYISRIGHT && rbpos)
	{
		playfield_set (rbpos + REBOUND_SIZE, NUM_ROWS-1, sp); /* clear rebound pixel */
		rbpos--;
		playfield_set (rbpos, NUM_ROWS-1, rb); /* set rebound pixel */
	}

	if (JOYISLEFT && rbpos < (NUM_COLS - (REBOUND_SIZE+1)))
	{
		playfield_set (rbpos, NUM_ROWS-1, sp); /* clear rebound pixel */
		rbpos++;
		playfield_set (rbpos + REBOUND_SIZE, NUM_ROWS-1, rb); /* set rebound pixel */
	}
	rebound_draw();
}
