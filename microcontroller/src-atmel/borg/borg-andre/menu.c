/* A game chooser for borgs
 * by: Christian Kroll
 * date: Tuesday, 2007/03/27
 */

#include <assert.h>
#include <inttypes.h>
#include "menu.h"
#include "config.h"
#include "util.h"
#include "pixel.h"
#include "joystick.h"
// #include "snake.h"
#include "tetris/logic.h"
#include "invaders2.h"

void menu()
{
	waitForFire = 0;
	clear_screen(0);
	
	while (JOYISFIRE)
	{
		wait(10);
	}
	
	static menu_item_t miSelection = MENU_ITEM_TETRIS;
	
	menu_animate(MENU_PREVITEM(miSelection), MENU_DIRECTION_LEFT);

	while (1)
	{
		if (JOYISFIRE)
		{
			while (JOYISFIRE)
			{
				wait(MENU_POLL_INTERVAL);
			}
			switch (miSelection)
			{
				/*
				case MENU_ITEM_SNAKE
					snakeJoystick();
					break;
				*/
				case MENU_ITEM_SPACEINVADERS:
					borg_invaders();
					break;
				case MENU_ITEM_TETRIS:
					tetris();
					break;
			}
			break;
		}
		else if (JOYISUP) // is really right
		{
			menu_animate(miSelection, MENU_DIRECTION_LEFT);
			miSelection = MENU_NEXTITEM(miSelection);
		}
		else if (JOYISDOWN) // is really left
		{
			menu_animate(miSelection, MENU_DIRECTION_RIGHT);
			miSelection = MENU_PREVITEM(miSelection);
		}
		else if (JOYISRIGHT) // is really up
		{
			break;
		}
		else
		{
			wait(MENU_POLL_INTERVAL);
		}
	}
	
	waitForFire = 1;
	return;
}


uint8_t menu_getIconPixel(menu_item_t item, int8_t x, int8_t y)
{
	// MSB is leftmost pixel
	static uint8_t nIcon[][8] =
		{/*{0xff, 0x81, 0xbd, 0xa5, 0xa5, 0xad, 0xa1, 0xbf},*/	// Snake icon
		 {0x66, 0x18, 0x3c, 0x5a, 0xff, 0xbd, 0xa5, 0x18},  // Invaders icon
		 {0x0f, 0x0f, 0xc3, 0xdb, 0xdb, 0xc3, 0xf0, 0xf0}}; // Tetris icon

	if (x < MENU_WIDTH_ICON)
	{
		return (0x80 >> x) & nIcon[item][y];
	}
	else
	{
		// delimiter
		return 0;
	}
}


void menu_animate(menu_item_t miInitial, menu_direction_t direction)
{
	int16_t nWait = MENU_WAIT_INITIAL;

	// space between left border and the icon in the middle
	int8_t nWidthSide = (NUM_COLS - MENU_WIDTH_ICON) / 2;
		
	// determine the icon at the leftmost position
	menu_item_t mi = miInitial + MENU_ITEM_MAX;
	int8_t nBack = nWidthSide / (MENU_WIDTH_ICON + MENU_WIDTH_DELIMITER);
	if ((nWidthSide % (MENU_WIDTH_ICON + MENU_WIDTH_DELIMITER)) != 0)
	{
		++nBack;
	}
	mi = (mi + MENU_ITEM_MAX - (nBack % MENU_ITEM_MAX)) % MENU_ITEM_MAX;
	
	int8_t nStart, nStop;
	if (direction == MENU_DIRECTION_STILL)
	{
		nStart = 0;
		nStop = 0;
	}
	else
	{
		nStart = 1;
		nStop = MENU_WIDTH_ICON + MENU_WIDTH_DELIMITER;	
	}
	
	int8_t i;
	for (i = nStart; i <= nStop; ++i)
	{
		int8_t nOffset;
		if (direction == MENU_DIRECTION_LEFT)
			nOffset = i;
		else
			nOffset = -i;

		// offset if the left border cuts the left most icon
		int8_t nInitialSideOffset = (((MENU_WIDTH_ICON + MENU_WIDTH_DELIMITER) - 
			(nWidthSide % (MENU_WIDTH_ICON + MENU_WIDTH_DELIMITER))) + nOffset +
			(MENU_WIDTH_ICON + MENU_WIDTH_DELIMITER)) %
			(MENU_WIDTH_ICON + MENU_WIDTH_DELIMITER);
								
		if (direction == MENU_DIRECTION_LEFT)
		{
			if (nInitialSideOffset == 0)
			{
				mi = MENU_NEXTITEM(mi);
			}
		}
		
		int8_t y;
		for (y = 0; y < MENU_HEIGHT_ICON; ++y)
		{
			menu_item_t miCurrent = mi;
			int8_t nIconOffset = nInitialSideOffset; 
			int8_t x;
			for (x = 0; x < NUM_COLS; ++x)
			{
				int8_t nPixel = menu_getIconPixel(miCurrent, nIconOffset, y);
				
				menu_setpixel(x, ((NUM_ROWS - MENU_HEIGHT_ICON) / 2) + y, nPixel);
				if (++nIconOffset >= (MENU_WIDTH_ICON + MENU_WIDTH_DELIMITER))
				{
					nIconOffset = 0;
					miCurrent = MENU_NEXTITEM(miCurrent);
				}
			}
		}

		if (direction == MENU_DIRECTION_RIGHT)
		{
			if (nInitialSideOffset == 0)
			{
				mi = MENU_PREVITEM(mi);
			}
		}

		wait(nWait);
		nWait += MENU_WAIT_INCREMENT;
	}
}


void menu_setpixel(int8_t x, int8_t y, int8_t isSet)
{
	uint8_t nColor;
	
	// mirror mirror on the wall, what's the quirkiest API of them all...
	x = NUM_COLS - 1 - x;
	uint8_t nMiddle = (NUM_COLS - MENU_WIDTH_ICON) / 2; 
	
	if (isSet != 0)
		{
		if ((x >= nMiddle - MENU_WIDTH_DELIMITER) &&
			(x < (nMiddle + MENU_WIDTH_ICON + MENU_WIDTH_DELIMITER)))
		{
			nColor = 3;
		}
		else if ((x == (nMiddle - MENU_WIDTH_DELIMITER - 1)) ||
		 (x == (nMiddle + MENU_WIDTH_ICON + MENU_WIDTH_DELIMITER)))
		{
			nColor = 2;
		}
		else
		{
			nColor = 1;
		}
	}
	else
	{
		nColor = 0;
	}
	
	setpixel((pixel) {x, y}, nColor);
}

