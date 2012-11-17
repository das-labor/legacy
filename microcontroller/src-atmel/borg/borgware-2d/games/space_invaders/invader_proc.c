#include <stdlib.h>
#include "../../config.h"
#include "../../joystick/joystick.h"
#include "../../random/prng.h"
#include "invaders2.h"

void procCannon(Cannon * cn, pixel * shot)
{
	static unsigned char mv = 0;
	if (mv++ >= CANNON_SPEED)
	{
		mv = 0;
		if (JOYISLEFT)
		{
			if (cn->pos < (NUM_COLS - 1))
			{
				cn->pos++;
			}
		}
		else if (JOYISRIGHT)
		{
			if (cn->pos > 0)
			{
				--cn->pos;
			}
		}
		else if (JOYISFIRE)
		{
			if (cn->ready)
			{
				shot->x = cn->pos;
				shot->y = NUM_ROWS - 3;
				cn->ready = 0;
			}
		}
	}
}

static unsigned char areAtBorder(Invaders * iv)
{
	unsigned char y;
	for (y = SPACESHIP_LINE + 1; y <= GUARD_LINE; ++y)
	{
		if (getInvaderPixel(iv, NUM_COLS - 1, y) || getInvaderPixel(iv, 0, y))
		{
			return 1;
		}
	}
	return 0;
}

void procInvaders(Invaders * iv, pixel *st)
{
	static unsigned char mv = 0;

	if (mv++ >= iv->speed)
	{
		mv = 0;
		if (areAtBorder(iv) && !(iv->isEdged))
		{
			iv->pos.y++;
			iv->direction = -iv->direction;
			iv->isEdged = 1;
		}
		else
		{
			iv->pos.x += iv->direction;
			iv->isEdged = 0;
		}
	}

	unsigned char i, y;
	unsigned char spos = random8() % UNUM_COLS;

	if (random8() < SHOOTING_RATE)
	{
		for (i = 0; i < MAX_SHOTS; ++i)
		{
			if (st[i].y >= NUM_ROWS)
			{
				for (y = GUARD_LINE; y > SPACESHIP_LINE; --y)
				{
					if (getInvaderPixel(iv, spos, y) != 0)
					{
						st[i].x = spos;
						st[i].y = y + 1;
						return;
					}
				}
			}
		} //for SHOTS
	}
}

void procShots(Invaders * iv, Player * pl, Cannon * cn, Spaceship * sc,
		unsigned char *guards, pixel *st, pixel * shot)
{
	unsigned char i;
	static unsigned char cmv = 0, imv = 0;

	if (cmv >= CANNON_SHOOTING_SPEED)
	{
		cmv = 0;
		if (!(cn->ready))
		{
			if (shot->y > 0)
			{
				shot->y--;
			}
			else
			{
				cn->ready = 1;
			}
		}
	}

	if (imv >= INVADER_SHOOTING_SPEED)
	{
		imv = 0;

		for (i = MAX_SHOTS; i--;)
		{
			if (st[i].y < NUM_ROWS)
			{
				st[i].y++;
			}
		}
	}

	cmv++;
	imv++;

	/****************************************************************/
	/*                    TESTE OB GETROFFEN                        */
	/****************************************************************/

	// USER CANNON
	unsigned char tmp;
	if (!(cn->ready))
	{
		for (i = MAX_SHOTS; i--;)
		{
			if (shot->x == st[i].x && shot->y == st[i].y)
			{
				st[i].x = 255;
				st[i].y = 255;
				cn->ready = 1;
				goto invader_shots;
			}
		}

		//GUARDS
		if ((tmp = getGuardPixel(guards, shot->x, shot->y)))
		{
			--tmp;
			setGuardPixel(guards, shot->x, shot->y, tmp);
			cn->ready = 1;
			goto invader_shots;
		}

		//INVADER
		if ((tmp = getInvaderPixel(iv, shot->x, shot->y)))
		{
			--tmp;
			setInvaderPixel(iv, shot->x, shot->y, tmp);

			if (tmp == 0)
			{
				iv->speedinc++;
				if (iv->speedinc == SPEED_INC_RATE)
				{
					iv->speedinc = 0;
					iv->speed -= SPEED_INC_VALUE;
				}
				pl->points += POINTS_FOR_KILL;
			}
			else
			{
				pl->points += POINTS_FOR_HIT;
			}
			cn->ready = 1;
			goto invader_shots;
		}

		//SPACESHIP  
		if (shot->y == SPACESHIP_LINE)
		{
			if ((shot->x <= sc->pos) && (shot->x >= sc->pos - 1))
			{
				sc->pos = NO_SPACESHIP;
				pl->points += POINTS_FOR_SPACESHIP;
				cn->ready = 1;
				goto invader_shots;
			}
		}
	}

invader_shots:
	for (i = 0; i < MAX_SHOTS; ++i)
	{
		if ((tmp = getGuardPixel(guards, st[i].x, st[i].y)))
		{
			--tmp;
			setGuardPixel(guards, st[i].x, st[i].y, tmp);
			st[i].x = 255;
			st[i].y = 255;
		}

		if (st[i].y == (NUM_ROWS - 2))
		{
			if (st[i].x == cn->pos)
			{

				pl->lives--;
				st[i].x = 255;
				st[i].y = 255;
			}
		}
	}

}

void procSpaceship(Spaceship * sc)
{
	unsigned char const rnd1 = random8();
	unsigned char const rnd2 = random8();

	static unsigned char sct = 0;

	if (sc->pos == NO_SPACESHIP)
	{
		if ((rnd1 == 73) && (rnd2 >= 200))
		{
			sc->pos = NUM_COLS;
			sct = 0;
		}
	}
	else
	{
		if (sct++ == SPACESHIP_SPEED)
		{
			sct = 0;
			if (sc->pos > 0)
			{
				sc->pos--;
			}
			else
			{
				sc->pos = NO_SPACESHIP;
			}
		}
	}
}

unsigned char getStatus(Invaders * iv)
{
	unsigned char x, y;

	// did invaders reached earth?
	for (x = NUM_COLS; x--;)
	{
		if (getInvaderPixel(iv, x, GUARD_LINE + 1))
		{
			return 2;
		}
	}

	// any invaders left?
	for (x = MAX_INVADER_WIDTH; x--;)
	{
		for (y = MAX_INVADER_HEIGHT; y--;)
		{
			if (iv->map[x][y])
			{
				return 0; // yes
			}
		}
	}

	// if we reach here, level was cleared \o/
	return 1;
}
