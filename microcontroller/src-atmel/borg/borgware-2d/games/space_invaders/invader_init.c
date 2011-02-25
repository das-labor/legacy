#include <stdint.h>
#include <string.h>
#include "../../compat/pgmspace.h"
#include "invaders2.h"

uint16_t const peter[8] PROGMEM =
		{0x0104, 0x0088, 0x01FC, 0x0376, 0x07FF, 0x05FD, 0x0505, 0x00D8};

uint16_t const hans[7] PROGMEM =
		{0x0000, 0x0372, 0x0552, 0x0372, 0x0552, 0x0356, 0x0000};

void initGuards(unsigned char *guards)
{
	memset(guards, 0, BORG_WIDTH);

	guards[3] = 3;
	guards[6] = 3;
	guards[10] = 3;
	guards[13] = 3;
}

void initInvaders(Invaders * iv, unsigned char lv)
{
	unsigned char x, y;

	// first zero out map!
	memset(iv->map, 0, sizeof(iv->map));

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
		for (y = 7; y--;)
		{
			uint16_t hansrow = pgm_read_word(&hans[y]);
			uint16_t mask = 0x0001;
			for (x = 11; x--;)
			{
				iv->map[x][y] = (hansrow & mask) ? 3 : 1;
				mask <<= 1;
			}
		}

		iv->pos.x = 3;
		iv->pos.y = SPACESHIP_LINE + 1;

		iv->speed = MIN_SPEED + 2;
		iv->direction = 1;

		break;

	case 4:
		for (y = 8; y--;)
		{
			uint16_t peterrow = pgm_read_word(&peter[y]);
			uint16_t mask = 0x0001;
			for (x = 11; x--;)
			{
				if (peterrow & mask)
				{
					iv->map[x][y] = 2;
				}
				mask <<= 1;
			}
		}

		iv->pos.x = 3;
		iv->pos.y = SPACESHIP_LINE + 1;

		iv->speed = MIN_SPEED + 3;
		iv->direction = 1;

		break;

	}
}
