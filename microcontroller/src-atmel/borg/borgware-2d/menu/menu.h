/* A game chooser for borgs
 * by: Christian Kroll
 * date: Tuesday, 2008/03/16
 */

#ifndef MENU_H_
#define MENU_H_

#include <inttypes.h>

/*
typedef enum menu_item_t
{
	MENU_ITEM_SNAKE,
	MENU_ITEM_SPACEINVADERS,
	MENU_ITEM_TETRIS,
	MENU_ITEM_MAX // fake entry to mark the end
}
menu_item_t;
*/

typedef enum menu_direction_t
{
	MENU_DIRECTION_LEFT,
	MENU_DIRECTION_RIGHT,
	MENU_DIRECTION_STILL
}
menu_direction_t;

typedef struct{
	void(*run)(void);
	uint8_t * icon;
}game_descriptor_t;


void menu();
void menu_animate(uint8_t currentItem, menu_direction_t direction);
uint8_t menu_getIconPixel(uint8_t item, int8_t x, int8_t y);
void menu_setpixel(int8_t x, int8_t y, int8_t isSet);

#endif /*MENU_H_*/

