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

//menu_item_t *its_setup[] = {&bla};
menu_t msetup = {"setup", 1, bla};
menu_item_t isetup = {&show_menu, &menu_handler, &msetup};

//menu_item_t *its_all[] = {&iadr};
menu_t mtempadr = {"alle", 1, bla};
menu_item_t itempadr = {&show_menu, &menu_handler, &mtempadr};

menu_item_t *its_tempadr[] = {&itempadr};
menu_t mtemp = {"temp", 1, its_tempadr};
menu_item_t itemp = {&show_menu, &menu_handler, &mtemp};

menu_item_t *its_info[] = {&itemp};
menu_t minfo = {"wissen", 1, its_info};
menu_item_t iinfo = {&show_menu, &menu_handler, &minfo};

menu_t mmoodadr = {"alle", 1, bla};
menu_item_t imoodadr = {&show_menu, &menu_handler, &mmoodadr};

menu_item_t *its_moodadr[] = {&imoodadr};
menu_t mmood = {"mood", 1, its_moodadr};
menu_item_t imood = {&show_menu, &menu_handler, &mmood};

menu_item_t *its_ctrl[] = {&imood};
menu_t mctrl = {"macht", 1, its_ctrl};
menu_item_t ictrl = {&show_menu, &menu_handler, &mctrl};

menu_item_t *items_root[] = {&ictrl, &iinfo, &isetup};
menu_t root = {"/", 3, items_root};

TimerControlBlock switchtimer;

AVRX_GCC_TASKDEF(menu, 100, 5) {
	menu_handler(&root);
	while (1) {
	}
}
uint8_t getKey() {
	
	while(1) {
		//AvrXDelay(&switchtimer, 550);
		if(!(PINA & (1 << PB0))) {
			return 0;
		}
		if(!(PINA & (1 << PB1))) {
			return 1;
		}
		if(!(PINA & (1 << PB2))) {
			return 2;
		}
		if(!(PINA & (1 << PB3))) {
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

	dispFillRect(pos.y, pos.x, 41, 10*menu->size, 0);

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
			dispFillRect(0, pos.x, 41, 10*menu->size, 0);
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
	dispFillRect(pos.x+1, pos.y+1, 39, 8, selected);
	draw_Text(menu->name, pos.x+2, pos.y+2, 0, 1, !selected);
}

void show_text(void *data, uint8_t selected) {
	menu_t *menu = data;
	
}

void show_mood(void *data, uint8_t selected) {
	menu_t *menu = data;
	dispDrawRect(50, pos.y, 65, 9, 1);
	dispFillRect(pos.x+1, pos.y+1, 39, 8, selected);
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

/*
setup
scan
	remove entrys
	scan + add entrys
	backlight // kontrast
*/
/*
// adds an entry to a menu structure at pos
void menu_add_entry(menu **menu_pt, menu_entry * entry, uint16_t pos){
	(*menu_pt) = realloc((*menu_pt), 4+((*menu_pt)->entry_num+1)*sizeof(menu_entry) );
	uint16_t x;
	for(x=(*menu_pt)->entry_num; x>=(pos+1); x--){
		(*menu_pt)->entries[x] = (*menu_pt)->entries[x-1];
	}
	(*menu_pt)->entries[pos] = *entry;
	(*menu_pt)->entry_num++;
}

void menu_remove_entry(menu **menu_pt, uint16_t pos){
	uint16_t x;
	for(x=pos; x < (*menu_pt)->entry_num-1; x++){
		(*menu_pt)->entries[x] = (*menu_pt)->entries[x+1];
	}
	(*menu_pt)->entry_num--;	                                
	(*menu_pt) = realloc((*menu_pt), 4+((*menu_pt)->entry_num)*sizeof(menu_entry) );
}
*/
