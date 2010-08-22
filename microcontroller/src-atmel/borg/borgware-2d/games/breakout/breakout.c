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

#include "common.h"
#include "breakout.h"

#ifdef MENU_SUPPORT
//static uint8_t breakout_icon[8] PROGMEM = {0x03, 0x03, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00}; /* our Icon */
static uint8_t breakout_icon[8] PROGMEM = {0x00, 0x18, 0x18, 0x00, 0x00, 0xff, 0xff, 0x00}; /* our Icon */

game_descriptor_t breakout_game_descriptor __attribute__((section(".game_descriptors"))) =
{
	&borg_breakout_game,
	breakout_icon
};
#endif


void borg_breakout_game()
{
	borg_breakout(0);
}


void borg_breakout(uint8_t demomode)
{
	uint8_t ignorescore_buffer = ignorescore;
	uint16_t cycles = DEMO_CYCLES;
	uint8_t level;
	if (demomode)
	{
		level = random8() % 5;
		ignorescore = 1;
	}
	else
	{
		level = 0;
		ignorescore = 0;
	}

	ball_t balls[1];

	/* spawn a ball in the middle bottom of the field, let it move upwards with random speed & direction */
	ball_spawn_default(&(balls[0]));
	balls[0].strength = START_LIFES;
	level_init(level);
	uint8_t tick_divider = 0;
	rebound_init();

	while (cycles != 0)
	{
		wait(25);

		if ((tick_divider % 2) || JOYISFIRE)
			rebound_tick(demomode ? &balls[0] : NULL);

		if (tick_divider % 2)
		{
			ball_think(&(balls[0]));
			playfield_draw();
			ball_draw(&(balls[0]));
			if (!balls[0].strength)
			{
				print_score();
				break;
			}

			if (!level_getscorediff())
			{
				level++;
				/* respawn ball at rebound position */
				ball_spawn_default (&(balls[0]));
				balls[0].strength++;
				level_init(level);
				rebound_init();
			}
		}

		if (demomode)
			--cycles;

		// alternate the value of the tick divider
		tick_divider = tick_divider ? 0 : 1;
	}

	ignorescore = ignorescore_buffer;
}
