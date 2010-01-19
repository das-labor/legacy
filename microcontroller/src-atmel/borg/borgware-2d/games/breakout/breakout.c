#include <stdint.h>
#include "common.h"
void borg_breakout();

#ifdef MENU_SUPPORT
//static uint8_t breakout_icon[8] PROGMEM = {0x03, 0x03, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00}; /* our Icon */
static uint8_t breakout_icon[8] PROGMEM = {0x18, 0x18, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00}; /* our Icon */

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

	/* spawn a ball in the middle bottom of the field, let it move upwards with random speed & direction */
	ball_spawn (&balls[0], (uint16_t) (NUM_COLS / 2) * 256, (uint16_t) (NUM_ROWS-2) * 256, -120, 150, START_LIFES);
	level_init(0);
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
	}
}
