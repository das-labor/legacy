#include <stdlib.h>
#include <stdint.h>

#ifndef MENU_H

typedef struct
{
	char name[14];
	uint8_t next;
	uint16_t func_param;
	void ((*func)());
} menuentry_t;

extern menuentry_t mymenu[9];

void menu_init();
void menu_select();
void menu_next();
void menu_display();
void menu_last();
void menu_setactive ();
void menu_setinactive ();

#define MENU_H
#endif
