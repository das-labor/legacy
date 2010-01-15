
#include "../../menu/menu.h"

#ifdef MENU_SUPPORT
static uint8_t icon[8] PROGMEM =
	 {0x03, 0x03, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00}; /* our Icon */

game_descriptor_t invaders_game_descriptor __attribute__((section(".game_descriptors"))) ={
	&borg_breakout,
	icon,
};
#endif

void borg_breakout()
{
	uint8_t rungame = 1, num_balls = 1;
	ball_t balls[1];
	
	ball_init (balls[0]);

	/* spawn a ball in the middle bottom of the field, let it move upwards with random speed & x-direction */
	ball_spawn (balls[0], (NUM_COLS / 2) << 8, (NUM_ROWS-2) << 8, - random8(), random8(), START_LIFES);
	level_init(1);

	while (rungame)
	{
		ball_think(balls[0]);
		playfield_draw();
		ball_draw(balls[0]);
	}
}
