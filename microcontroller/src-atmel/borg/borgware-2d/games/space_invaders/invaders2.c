#include <stdio.h>
#include "../../util.h"
#include "../../compat/eeprom.h"
#include "../../compat/pgmspace.h"
#include "../../menu/menu.h"
#include "../../scrolltext/scrolltext.h"
#include "invaders2.h"

void borg_invaders();

#ifdef MENU_SUPPORT
// MSB is leftmost pixel
static const uint8_t icon[8] PROGMEM =
	 {0x66, 0x18, 0x3c, 0x5a, 0xff, 0xbd, 0xa5, 0x18}; // Invaders icon

game_descriptor_t invaders_game_descriptor __attribute__((section(".game_descriptors"))) ={
	&borg_invaders,
	icon,
};
#endif

void borg_invaders()
{
	/****************************************************************/
	/*                          INITIALIZE                          */
	/****************************************************************/
	offScreen_t offScreen = {{{0}}};

	Invaders iv;
	Cannon cn;
	Player pl;
	Spaceship sc;

	unsigned char guards[NUM_COLS];
	unsigned char level = 0;
	unsigned char ivStatus = 0;

	pixel st[MAX_SHOTS] =
	{
		{255, 255},
		{255, 255},
		{255, 255},
		{255, 255},
		{255, 255},
		{255, 255},
		{255, 255}
	};

	pixel shot;

	pl.points = 0;
	pl.lives = 3;


	/****************************************************************/
	/*                          GAME LOOP                           */
	/****************************************************************/
	do
	{
		//----- INITIALIZE LEVEL-----//
		initGuards(guards);
		initInvaders(&iv, level);

		//Spaceship 
		sc.lives = 1;
		sc.pos = NO_SPACESHIP;

		//Cannon
		cn.pos = (NUM_COLS - 3) / 2;
		cn.ready = 1;

		while (pl.lives != 0)
		{
			procInvaders(&iv, st);
			procSpaceship(&sc);
			procShots(&iv, &pl, &cn, &sc, guards, st, &shot);
			procCannon(&cn, &shot);

			draw(offScreen, &iv, &sc, &pl, &cn, guards, st, &shot);

			ivStatus = getStatus(&iv);

			if (ivStatus == 2) //LOST
			{
				pl.lives = 0;
				break;
			}
			else if (ivStatus == 1) //WON
			{
				unsigned int bonus = POINTS_FOR_LEVEL * (level + 1u) *
						(unsigned int)(NUM_ROWS - 4 - iv.pos.y);
				pl.points += bonus;

				level = (level + 1) % MAX_LEVEL;
				break;
			}

			wait (WAIT_MS);
		} //IN LEVEL LOOP

	} while (pl.lives != 0); //GAME LOOP

	/****************************************************************/
	/*                         PLAYER STAT                          */
	/*                         HIGH SCORES                          */
	/****************************************************************/
	#ifdef SCROLLTEXT_SUPPORT
		char text[64];
		snprintf(text, 64, "</#points: %u", pl.points);
		scrolltext(text);
	#endif
}
