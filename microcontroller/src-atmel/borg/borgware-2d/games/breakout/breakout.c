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
static void borg_breakout();

#ifdef MENU_SUPPORT
//static uint8_t breakout_icon[8] PROGMEM = {0x03, 0x03, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00}; /* our Icon */
static uint8_t breakout_icon[8] PROGMEM = {0x00, 0x18, 0x18, 0x00, 0x00, 0xff, 0xff, 0x00}; /* our Icon */

game_descriptor_t breakout_game_descriptor __attribute__((section(".game_descriptors"))) =
{
	&borg_breakout,
	breakout_icon
};
#endif

void borg_breakout()
{
	uint8_t level = 0;
	ball_t balls[1];

	/* spawn a ball in the middle bottom of the field, let it move upwards with random speed & direction */
	ball_spawn_default(&(balls[0]));
	balls[0].strength = START_LIFES;
	level_init(level);
	rebound_init();

	while (23)
	{
		wait(50);
		rebound_tick();
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
}
