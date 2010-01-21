#include "rebound.h"

#include <stdio.h>
static uint8_t rbpos;

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
