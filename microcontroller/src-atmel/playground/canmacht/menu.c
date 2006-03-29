#include <avr/io.h>
#include <avrx-io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "avrx.h"               // AvrX System calls/data structures
#include "config.h"
#include "xcan.h"
#include "menu.h"
#include "text.h"
#include "grafiklcd.h"

struct {
	int8_t x;
	int8_t y;
	} pos;		// doto: position handeling

extern menu_t root;

menu_item_t mi_i0 = {&show_menu, &menu_handler, &root}; // nur zum testen
menu_item_t *bla[] = {&mi_i0};				// "

//menu_item_t *its_all[] = {&iadr};
menu_t madr = {"all", 1, bla};
menu_item_t iadr = {&show_menu, &menu_handler, &madr};

menu_item_t *its_adr[] = {&iadr};
menu_t mtemp = {"temp", 1, its_adr};
menu_item_t itemp = {&show_menu, &menu_handler, &mtemp};

menu_item_t *its_info[] = {&itemp};
menu_t minfo = {"wissen", 1, its_info};
menu_item_t iinfo = {&show_menu, &menu_handler, &minfo};

menu_t mmood = {"mood", 1, bla};
menu_item_t imood = {&show_menu, &menu_handler, &mmood};

menu_item_t *its_ctrl[] = {&imood};
menu_t mctrl = {"macht", 1, its_ctrl};
menu_item_t ictrl = {&show_menu, &menu_handler, &mctrl};

menu_item_t *items_root[] = {&ictrl, &iinfo};
menu_t root = {"/", 2, items_root};

TimerControlBlock switchtimer;

AVRX_GCC_TASKDEF(menu, 100, 5) {
//	uint8_t state, oldstate = 0;
	menu_handler(&root);
	while (1) {
	}
}
uint8_t getKey() {
	AvrXDelay(&switchtimer, 600);
	while(1) {

		if(!(PINA & (1 << PA0))) {
			return 0;
		}
		if(!(PINA & (1 << PA1))) {
			return 1;
		}
		if(!(PINA & (1 << PA2))) {
			return 2;
		}
		if(!(PINA & (1 << PA3))) {
			return 3;
		}
	}
}

void menu_handler(void *data) {
	menu_t *menu = data;
	uint8_t i, key;
	int8_t sel = 0;
	DRAW_MEN:  // sprung nach 
	pos.y = 0;

	fillRect(pos.y, pos.x, 41, 10*menu->size, 0);

	for(i = 0; i < menu->size; i++) {
		menu->items[i]->show(menu->items[i]->data, (sel==i));
		pos.y += 9;		
		if(i >= 6)
			break;
	}
	while(1) {  // keyhandeling
		key = getKey();
		// enter
		if(key == 2) {
			fillRect(0, pos.x, 41, 10*menu->size, 0);
			//pos.x += 20;
			menu->items[sel]->enter(menu->items[sel]->data); // enter	
			goto DRAW_MEN;
		}

	// zurück
		if(key == 3) {
			//fillRect(0, pos.x, 41, 10*menu->size, 1);
			//pos.x -= 20;
			if(menu->name != "/") // nicht aus Hauptmenu springen
				return;
		}
	//	up down
		if(key < 2) {
			pos.y = sel*9;
			menu->items[sel]->show(menu->items[sel]->data, 0);
			if(key == 1)
				sel++;
			if(key == 0)
				sel--;
			if(sel == menu->size)
				sel = 0;
			if(sel < 0)
				sel = menu->size - 1;
			pos.y = sel*9;
			menu->items[sel]->show(menu->items[sel]->data, 1);
		}
	}
}

void show_menu(void *data, uint8_t selected) {
	menu_t *menu = data;
	dispDrawRect(pos.x, pos.y, 40, 9, 1);
	fillRect(pos.x+1, pos.y+1, 39, 8, selected);
	draw_Text(menu->name, pos.x+2, pos.y+2, 0, 1, !selected);
}

void show_text(void *data, uint8_t selected) {
	menu_t *menu = data;
	
}

void show_mood(void *data, uint8_t selected) {
	menu_t *menu = data;
	dispDrawRect(50, pos.y, 65, 9, 1);
	fillRect(pos.x+1, pos.y+1, 39, 8, selected);
}

void ctrl_mood(void *data) {
	menu_t *menu = data;
	uint8_t i, key;
	while(1) {  // keyhandeling
		key = getKey();
		// enter
		if(key == 0) {
			// dunkeler balken --
		}
		if(key == 1) {
			// heller balken++
		}
		if(key == 2) {
			// sel balken?
		}
		if(key == 3) {
			// back
		}
	}
}
