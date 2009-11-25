#include <avr/io.h>
#include <avrx-io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "avrx.h"               // AvrX System calls/data structures
#include "config.h"
#include "xcan.h"
#define MENU
#include "menu.h"
#include "ls020/ls020.h"
#include "ls020/glcd.h"
#include "lap.h"
#include "stdlib.h"
#include "string.h"

#define mc_POS_DIV(a, b)  ( (a)/(b) +  ( ( (a) % (b) >= (b)/2 ) ? 1 : 0 ) )

struct {
	uint8_t x;
	uint8_t y;
	} pos;		// todo: position handeling

extern menu_t root;
int8_t blightstat;

uint8_t scriptstat;


void make_basemenu();
menu_item_t *make_item(void *show, void *enter, menu_t *menu, uint8_t type) ;
menu_t *make_menu(char *name);
void menu_add_item(menu_t *menu, menu_item_t *entry, uint8_t pos) ;
void switch_blight(void *data, uint8_t selected);
void menu_handler(void *data);
void show_menu(void *data, uint8_t selected);
void menu_update(uint8_t act);
void show_text(void* data, uint8_t selected);
void show_mood(void *data, uint8_t selected);
void switch_script();
void temp(void *data);
void ctrl_mood(void *data);

menu_t root = {"/", 0, 0};

TimerControlBlock switchtimer;

AVRX_GCC_TASKDEF(menu, 100, 5)  {

	blightstat = OCR1B;
	make_basemenu();
	menu_handler(&root);
	while (1);
}

void make_basemenu() {	
	menu_add_item(&root, make_item(&show_menu, &menu_handler, make_menu("dim"), 0), 0);
	menu_add_item(root.items[0]->data, make_item(&show_menu, &switch_blight, make_menu("bglight"), 1), 0);

	menu_add_item(&root, make_item(&show_menu, &menu_handler, make_menu("ctrl"), 0), 1);
	menu_add_item(root.items[1]->data, make_item(&show_menu, &menu_handler, make_menu("mood"), 0), 0);
	menu_t * ctrl = root.items[1]->data;
	menu_add_item(ctrl->items[0]->data, make_item(&show_menu, &switch_script, make_menu("script"), 1), 0);
	menu_add_item(ctrl->items[0]->data, make_item(&show_menu, &ctrl_mood, make_menu("dimm"), 1), 1);

	menu_add_item(&root, make_item(&show_menu, &menu_handler, make_menu("licht"), 0), 2);
	menu_add_item(root.items[2]->data, make_item(&show_menu, &temp, make_menu("lampe"), 0), 0);
}

uint8_t getKey(uint16_t delay) {  // keyport in header
	AvrXDelay(&switchtimer, delay);
	while (1) {
		
		if (!(PIND & (1 << PD4))) { // up
			return 0;
		}
		if (!(PIND & (1 << PD5))) { // down
			return 1;
		}
		if (!(PIND & (1 << PD6))) { // ok
			return 2;
		}
		if (!(PIND & (1 << PD7))) { // back
			return 3;
		}
	}
}

void menu_handler(void *data) {
	menu_t *menu = data;
	uint8_t i, key;
	int8_t sel = 0;
	DRAW_MEN:  // sprung für neuzeichnung
	pos.y = 0;

	//dispFillRect(0, 0, 40, 11*menu->size, 0);

	for (i = 0; i < menu->size; i++) {
		menu->items[i]->show(menu->items[i]->data, (sel==i));
		pos.y += 11;
		if (i >= 6)
			break;
	}
	while (1) {  // keyhandeling
		key = getKey(500);
		// enter
		if (key == 2) {
			if (!(menu->items[sel]->type & 0x01))
				//dispFillRect(0, 0, 40, 11 * menu->size + 1, 0);
			//pos.x += 20;
			menu->items[sel]->enter(menu->items[sel]->data); // enter
			goto DRAW_MEN;
		}

	// zurück
		if (key == 3) {
			//pos.x -= 20;
			if (menu->name != "/") { // nicht aus Hauptmenu springen
				//dispFillRect(0, 0, 40, 11 * menu->size + 1, 0);
				return;
			}
		}
	//	up down
		if (key < 2) {
			pos.y = sel * 11;
			menu->items[sel]->show(menu->items[sel]->data, 0);
			if (key == 1)
				sel++;
			if (key == 0)
				sel--;
			if (sel == menu->size)
				sel = 0;
			if (sel < 0)
				sel = menu->size - 1;
			pos.y = sel * 11;
			menu->items[sel]->show(menu->items[sel]->data, 1);
		}
	}
}

void show_menu(void *data, uint8_t selected) {
	menu_t *menu = data;
	//dispDrawRect(pos.x, pos.y, 40, 12, 1);
	ls020_rectangle8(pos.x, pos.y, 40, 12, 0xff);
	//dispFillRect(pos.x + 1, pos.y + 1, 38, 10, selected);
	draw_Text(menu->name, pos.x + 2, pos.y + 2, 0, 1, !selected);
}


void switch_blight(void *data, uint8_t selected) {
	uint8_t key;
	ls020_rectangle8(50, 15, 32, 5, 0xff);
	//dispDrawRect(50, 15, 32, 5, 1);
	dispFillRect(51, 16, blightstat * 3, 3, 1);
	while (1) {  // keyhandeling
		key = getKey(550);
		if (key == 3) {
			//dispFillRect(50, 15, 32, 5, 0);
			return;
		}
	//	up down
		if (key < 2) {
			dispFillRect(51, 16, blightstat * 3, 3, 0);
			if (key == 1)
				blightstat--;
			if (key == 0)
				blightstat++;
			if (blightstat == 11)
				blightstat = 0;
			if (blightstat < 0)
				blightstat = 10;
			dispFillRect(51, 16, blightstat * 3, 3, 1);
			OCR1B = blightstat * 15;
		}
	}
}

void switch_script() {
	static can_message_t msg={0xa0, 0xa1, PORT_REMOTE, 0x06};

	msg.dlc = 1;
	msg.data[0] = scriptstat;
	can_put(&msg);
	if (scriptstat) {
		scriptstat = 0;
	} else {
		scriptstat = 1;
	}
}


// make item
menu_t *make_menu(char *name) { // name
	menu_t *newmenu = malloc(sizeof(menu_t)) ;

	newmenu->name = malloc(strlen(name) + 1);
	strcpy(newmenu->name, name);
	newmenu->items = 0;
	newmenu->size = 0;
	return newmenu;
}
/*
void menu_del_item (menu_t *menu, uint8_t pos) { // von wo  pos

}
*/
menu_item_t *make_item(void *show, void *enter, menu_t *menu, uint8_t type) {
	menu_item_t *menuitem = malloc(sizeof(menu_item_t));
	menuitem->show = show;
	menuitem->enter = enter;
	menuitem->data = menu;
	menuitem->type = type;
	return menuitem;
}

void menu_add_item(menu_t *menu, menu_item_t *entry, uint8_t pos) {
	uint8_t i;
	//pos > size? pos=size
	menu->size += 1;
	menu->items = realloc(menu->items, (menu->size) * sizeof(menu_item_t*));
	for (i = menu->size - 1; i > pos; i--) {
		menu->items[i] = menu->items[i - 1];
	}
	menu->items[pos] = entry;
	// elemente von hinten nach hinten aufrücken bis pos
	// element einfügen
}
