/* A game chooser for borgs
 * by: Christian Kroll
 * date: Tuesday, 2008/03/16
 */

#ifndef MENU_H_
#define MENU_H_

#include <inttypes.h>


typedef struct{
	void(*run)(void);
	uint8_t const *icon;
}game_descriptor_t;


void menu();

#endif /*MENU_H_*/

