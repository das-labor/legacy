#include <assert.h>
#include "invaders2.h"

/*--------------------double buffered graphics-------------------*/

static void setOffScreenPixel(offScreen_t offScreen, unsigned char x,
		unsigned char y, unsigned char color)
{
	assert(offScreen != 0);
	assert(x < NUM_COLS);
	assert(y < NUM_ROWS);
	assert(color <= NUMPLANE);

	offScreen[color][y][x / 8u] |= shl_table[x % 8u];
}

static void flushOffScreenBuffer(offScreen_t offScreen)
{
	assert(offScreen != 0);

	/* off-screen blitting borrowed from fixed point math patterns animation,
	   see fpmath_patterns.c (fixDrawPattern(...) for details */
	unsigned char *pPixmap =
			(&pixmap[NUMPLANE - 1][NUM_ROWS - 1][LINEBYTES - 1]) + 1;
	unsigned char *pOffScreenDistHigh =
			(&offScreen[NUMPLANE][NUM_ROWS - 1][LINEBYTES - 1]) + 1;
	unsigned char *pOffScreenDistLow =
			(&offScreen[NUMPLANE - 1][NUM_ROWS - 1][LINEBYTES - 1]) + 1;
	while (pPixmap != &pixmap[0][0][0]) // stop at the beginning
	{
		*(--pPixmap) = *(--pOffScreenDistHigh);
		*(--pOffScreenDistLow) |= *pOffScreenDistHigh;
		*pOffScreenDistHigh = 0;
	}
}

/*----------------------getter/setter----------------------------*/

unsigned char getInvaderPixel(Invaders * iv, unsigned char x, unsigned char y)
{
	if (((x - iv->pos.x) < MAX_INVADER_WIDTH) && ((x - iv->pos.x) >= 0) &&
			((y - iv->pos.y) < MAX_INVADER_HEIGHT) && ((y - iv->pos.y) >= 0))
	{
		return iv->map[x - iv->pos.x][y - iv->pos.y];
	}
	return 0;
}

void setInvaderPixel(Invaders * iv, unsigned char x, unsigned char y,
		unsigned char val)
{
	if (((x - iv->pos.x) < MAX_INVADER_WIDTH) && ((x - iv->pos.x) >= 0) &&
			((y - iv->pos.y) < MAX_INVADER_HEIGHT) && ((y - iv->pos.y) >= 0))
	{
		iv->map[x - iv->pos.x][y - iv->pos.y] = val;
	}
}

void setGuardPixel(unsigned char *guards, unsigned char x, unsigned char y,
		unsigned char val)
{
	assert(val <= NUMPLANE);

	if (x < NUM_COLS && y == GUARD_LINE)
	{
		guards[x] = val;
	}
}

/*----------------------drawing Method---------------------------*/

void draw(offScreen_t offScreen, Invaders * iv, Spaceship * sc, Player * pl,
		Cannon * cn, unsigned char *guards, pixel *st, pixel * shot)
{
	unsigned char x, y;

	/*---SPACESHIP---*/
	if (sc->pos < NUM_COLS)
	{
		setOffScreenPixel(offScreen, sc->pos, SPACESHIP_LINE, sc->lives);
	}
	if ((sc->pos > 0) && ((sc->pos - 1) < NUM_COLS))
	{
		setOffScreenPixel(offScreen, sc->pos - 1, SPACESHIP_LINE, sc->lives);
	}

	/*---INVADERS--*/
	for (y = MAX_INVADER_HEIGHT; y--;)
	{
		for (x = MAX_INVADER_WIDTH; x--;)
		{
			if ((x + iv->pos.x >= 0) && (x + iv->pos.x < NUM_COLS) &&
					(y + iv->pos.y < NUM_ROWS))
			{
				setOffScreenPixel(offScreen, x + iv->pos.x, y + iv->pos.y,
						iv->map[x][y]);
			}
		}
	}

	/*---GUARDS---*/
	for (x = NUM_COLS; x--;)
	{
		setOffScreenPixel(offScreen, x, GUARD_LINE, guards[x]);
	}

	/*---SHOTS--*/
	unsigned char i;
	for (i = MAX_SHOTS; i--;)
	{
		if (st[i].x < NUM_COLS && st[i].y < NUM_ROWS)
		{
			setOffScreenPixel(offScreen, st[i].x, st[i].y, 3);
		}
	}

	/*draw player shot*/
	if (!(cn->ready))
	{
		setOffScreenPixel(offScreen, shot->x, shot->y, 3);
	}

	/*-- CANNON --*/
	if (cn->pos > 0) /* right border */
	{
		setOffScreenPixel(offScreen, cn->pos - 1, NUM_ROWS - 1, pl->lives);
	}
	if (cn->pos < NUM_COLS)
	{
		setOffScreenPixel(offScreen, cn->pos, NUM_ROWS - 2, pl->lives);
		setOffScreenPixel(offScreen, cn->pos, NUM_ROWS - 1, pl->lives);
	}
	if (cn->pos < (NUM_COLS - 1)) /* left border */
	{
		setOffScreenPixel(offScreen, cn->pos + 1, NUM_ROWS - 1, pl->lives);
	}

	flushOffScreenBuffer(offScreen);
}
