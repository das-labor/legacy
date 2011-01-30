#include <stdint.h>
#include "../../compat/pgmspace.h"
#include "invaders2.h"

uint8_t const peter[8][11] PROGMEM =
{
{ 0, 0, P, 0, 0, 0, 0, 0, P, 0, 0 },
{ 0, 0, 0, P, 0, 0, 0, P, 0, 0, 0 },
{ 0, 0, P, P, P, P, P, P, P, 0, 0 },
{ 0, P, P, 0, P, P, P, 0, P, P, 0 },
{ P, P, P, P, P, P, P, P, P, P, P },
{ P, 0, P, P, P, P, P, P, P, 0, P },
{ P, 0, P, 0, 0, 0, 0, 0, P, 0, P },
{ 0, 0, 0, P, P, 0, P, P, 0, 0, 0 } };

uint8_t const hans[8][11] PROGMEM =
{
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
{ 1, 2, 1, 1, 2, 2, 2, 1, 2, 2, 1 },
{ 1, 2, 1, 1, 2, 1, 2, 1, 2, 1, 2 },
{ 1, 2, 1, 1, 2, 2, 2, 1, 2, 2, 1 },
{ 1, 2, 1, 1, 2, 1, 2, 1, 2, 1, 2 },
{ 1, 2, 2, 1, 2, 1, 2, 1, 2, 2, 2 },
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

void initGuards(unsigned char guards[BORG_WIDTH])
{
	unsigned char x;
	for (x = BORG_WIDTH; x--;)
	{
		guards[x] = 0;
	}

	guards[3] = 3;
	guards[6] = 3;
	guards[10] = 3;
	guards[13] = 3;

}

void initInvaders(Invaders * iv, unsigned char lv)
{
	unsigned char x, y;

	// first zero out map!
	for (x = MAX_INVADER_WIDTH; x--;)
	{
		for (y = MAX_INVADER_HEIGHT; y--;)
		{
			iv->map[x][y] = 0;
		}
	}

	iv->speedinc = 0;
	iv->isEdged = 0;

	switch (lv)
	{
	case 0:
		for (x = 0; x < 8; ++x)
		{
			iv->map[x][0] = 2;
			iv->map[x][1] = 2;
			iv->map[x][2] = 2;
			iv->map[x][3] = 1;
			// iv->map[x][4] = 1;
		}

		iv->pos.x = 4;
		iv->pos.y = SPACESHIP_LINE + 1;
		iv->speed = MIN_SPEED - 3;
		iv->direction = 1;
		break;

	case 1:
		for (x = 0; x < 8; ++x)
		{
			//for(y = 0; y < MAX_INVADER_HEIGHT; ++y) {
			iv->map[x][0] = 3;
			iv->map[x][1] = 3;
			iv->map[x][2] = 2;
			iv->map[x][3] = 2;
			// iv->map[x][4] = 1;
			//}     
		}

		iv->pos.x = 4;
		iv->pos.y = SPACESHIP_LINE + 1;
		iv->speed = MIN_SPEED - 2;

		iv->direction = 1;
		break;

	case 2:
		for (x = 0; x < 8; ++x)
		{
			//for(y = 0; y < MAX_INVADER_HEIGHT; ++y) {
			iv->map[x][0] = 3;
			iv->map[x][1] = 3;
			iv->map[x][2] = 2;
			iv->map[x][3] = 2;
			iv->map[x][4] = 1;
			//}     
		}

		iv->pos.x = 4;
		iv->pos.y = SPACESHIP_LINE + 1;
		iv->speed = MIN_SPEED - 1;

		iv->direction = 1;
		break;

	case 3:
		for (x = 11; x--;)
		{
			for (y = 8; y--;)
			{
				if (pgm_read_byte(&hans[y][x]) != 0)
				{
					iv->map[x][y] = 2;
				}
			}
		}

		iv->pos.x = 3;
		iv->pos.y = SPACESHIP_LINE + 1;

		iv->speed = MIN_SPEED + 2;
		iv->direction = 1;

		break;

	case 4:
		for (x = 11; x--;)
		{
			for (y = 8; y--;)
			{
				if (pgm_read_byte(&peter[y][x]) != 0)
				{
					iv->map[x][y] = 2;
				}
			}
		}

		iv->pos.x = 3;
		iv->pos.y = SPACESHIP_LINE + 1;

		iv->speed = MIN_SPEED + 3;
		iv->direction = 1;

		break;

	}
}
