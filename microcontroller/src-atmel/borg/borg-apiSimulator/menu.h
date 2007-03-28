/* A game chooser for borgs
 * by: Christian Kroll
 * date: Tuesday, 2007/03/27
 */

#ifndef MENU_H_
#define MENU_H_

#include <inttypes.h>

#define MENU_WIDTH_ICON 8
#define MENU_HEIGHT_ICON 8
#define MENU_WIDTH_DELIMITER 2

#define MENU_WAIT_INITIAL 20
#define MENU_WAIT_INCREMENT 0

#define MENU_NEXTITEM(item) ((item + 1) % MENU_ITEM_MAX)
#define MENU_PREVITEM(item) ((item + MENU_ITEM_MAX - 1) % MENU_ITEM_MAX)

typedef enum menu_item_t
{
	// MENU_ITEM_SNAKE,
	MENU_ITEM_SPACEINVADERS,
	MENU_ITEM_TETRIS,
	MENU_ITEM_MAX // fake entry to mark the end
}
menu_item_t;

typedef enum menu_direction_t
{
	MENU_DIRECTION_LEFT,
	MENU_DIRECTION_RIGHT,
	MENU_DIRECTION_STILL
}
menu_direction_t;

void menu();
void menu_animate(menu_item_t currentItem, menu_direction_t direction);
uint8_t menu_getIconPixel(menu_item_t item, int8_t x, int8_t y);
void menu_setpixel(int8_t x, int8_t y, int8_t isSet);


#endif /*MENU_H_*/

