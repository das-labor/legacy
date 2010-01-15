#include <stdint.h>
#include "common.h"
void borg_breakout();

#ifdef MENU_SUPPORT
//static uint8_t breakout_icon[8] PROGMEM = {0x03, 0x03, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00}; /* our Icon */
static uint8_t breakout_icon[8] PROGMEM = {0x03, 0x03, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00}; /* our Icon */

game_descriptor_t breakout_game_descriptor __attribute__((section(".game_descriptors"))) =
{
	&borg_breakout,
	breakout_icon
};
#endif

void borg_breakout()
{
	uint8_t rungame = 1, num_balls = 1;
	ball_t balls[1];
	
	rebound_init();

	/* spawn a ball in the middle bottom of the field, let it move upwards with random speed & x-direction */
	ball_spawn (&balls[0], (NUM_COLS / 2) << 8, (NUM_ROWS-2) << 8, - (random8() % 8), (random8() % 8), START_LIFES);
	level_init(1);

	while (rungame)
	{
		wait(50);
		rebound_tick();
		ball_think(&balls[0]);
		playfield_draw();
		ball_draw(&balls[0]);
	}
}
