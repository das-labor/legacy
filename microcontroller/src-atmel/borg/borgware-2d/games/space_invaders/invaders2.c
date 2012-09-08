#include <stdio.h>
#include "../../util.h"
#include "../../compat/eeprom.h"
#include "../../compat/pgmspace.h"
#include "../../menu/menu.h"
#include "../../scrolltext/scrolltext.h"
#include "invaders2.h"

//#include <stdio.h>

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
	//      waitForFire = 0;        
	/****************************************************************/
	/*                          INITIALIZE                          */
	/****************************************************************/

	Invaders iv;
	Cannon cn;
	Player pl;
	Spaceship sc;

	unsigned char guards[BORG_WIDTH];

	unsigned char level = 0;
	unsigned char ivStatus = 0;

	uPixel st[MAX_SHOTS] =
	{
		{	255, 255},
		{	255, 255},
		{	255, 255},
		{	255, 255},
		{	255, 255}
	};

	uPixel shot;
	// = {0,0};

	pl.points = 0;
	pl.lives = 3;

	//--------Init Cannon-------//
	//cn.pos = 4;
	//cn.ready = 1;

	/****************************************************************/
	/*                          INTRO                               */
	/****************************************************************/
	//drawIntroScreen(2000);


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
		sc.pos = 255;

		//Cannon
		cn.pos = 7;
		cn.ready = 1;

		draw(&iv, &sc, &pl, &cn, guards, st, &shot);

		while (1)
		{
			procInvaders(&iv, st);
			procSpaceship(&sc);

			procShots(&iv, &pl, &cn, &sc, guards, st, &shot);
			procCannon(&cn, &shot);

			draw(&iv, &sc, &pl, &cn, guards, st, &shot);

			ivStatus = getStatus(&iv);

			if (ivStatus == 2) //LOST  
			{
				//pl.lives--;
				pl.lives = 0;
				break;

			}
			else if (ivStatus == 1) //WON
			{
				unsigned int bonus = POINTS_FOR_LEVEL * (level + 1u) *
						(unsigned int)(12 - iv.pos.y);
				pl.points += bonus;
				//printf("cleared l: %d , y: %d bonus: %d \n", 
				//              level, iv.pos.y, bonus);
				if (level == MAX_LEVEL - 1)
				{
					level = 0;
				}
				else
				{
					level = (level + 1);
				}
				break;
			}

			if (pl.lives <= 0)
			{
				//scrolltext("GAME OVER",0,80);
				break;
			}

			wait (WAIT_MS);
		} //IN LEVEL LOOP

	} while (pl.lives > 0); //GAME LOOP

	clearScreen ();
	//wait(5000);
	#ifdef SCROLLTEXT_SUPPORT
		char text[64];
		snprintf(text, 64, "</#points: %u", pl.points);
		scrolltext(text);
	#endif
	//printf("scores: %d\n", pl.points);


	/****************************************************************/
	/*                         PLAYER STAT                          */
	/*                         HIGH SCORES                          */
	/****************************************************************/

	//      waitForFire = 1;                

}
