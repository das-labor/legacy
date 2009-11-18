#include <stdlib.h>
#include <stdint.h>
#ifndef MENU_H

typedef struct
{
	char name[14];
	uint16_t func_param;
	void ((*func)());
} menuentry_t;

void menu_init();
void menu_select();
void menu_next();
void menu_display();
void menu_last();
void mdebug_jump (uint16_t );
void dbg_reboot ();

#define MENU_H
#endif
