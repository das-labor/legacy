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
#include "text.h"
#include "lap.h"
#include "grafiklcd.h"
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
/*
menu_item_t mi_i0 = {&show_menu, &menu_handler, &root}; // nur zum testen
menu_item_t *bla[] = {&mi_i0};				// "

//menu_item_t *its_blight[] = {&};
menu_t mblight = {"blight", 1, 0};//its_blight};
menu_item_t iblight = {&show_menu, &switch_blight, &mblight};

menu_t mscan = {"scan", 1, bla};
menu_item_t iscan = {&show_menu, &menu_handler, &mscan};

menu_item_t *its_setup[] = {&iblight, &iscan};
menu_t msetup = {"setup", 2, its_setup};
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

menu_t mdimm = {"dimm", 1, bla};
menu_item_t idimm = {&show_mood, &menu_handler, &mdimm};

menu_item_t *its_alle[] = {&idimm};
menu_t mmoodadr = {"alle", 1, its_alle};
menu_item_t imoodadr = {&show_menu, &menu_handler, &mmoodadr};

menu_item_t *its_moodadr[] = {&imoodadr};
menu_t mmood = {"mood", 1, its_moodadr};
menu_item_t imood = {&show_menu, &menu_handler, &mmood};

menu_item_t *its_ctrl[] = {&imood};
menu_t mctrl = {"macht", 1, its_ctrl};
menu_item_t ictrl = {&show_menu, &menu_handler, &mctrl};

menu_item_t *items_root[] = {&ictrl, &iinfo, &isetup};
//menu_t root = {"/", 3, items_root};

*/


menu_t root = {"/", 0, 0};

TimerControlBlock switchtimer;

AVRX_GCC_TASKDEF(menu, 100, 5)  {

	DDRD |= (1 << PD7);
  	TCCR2 = (1<<WGM21) | (1<<WGM20) | (1 << COM21) | (1 << COM20) | (1 << CS22) | (1 << CS21) | (0 << CS20);
	OCR2 = 0;

	make_basemenu();
	menu_handler(&root);
	while (1);
}

void make_basemenu() {	
	menu_add_item(&root, make_item(&show_menu, &menu_handler, make_menu("setup"), 0), 0);
	menu_add_item(root.items[0]->data, make_item(&show_menu, &switch_blight, make_menu("bglight"), 1), 0);

	menu_add_item(&root, make_item(&show_menu, &menu_handler, make_menu("ctrl"), 0), 1);
	menu_add_item(root.items[1]->data, make_item(&show_menu, &menu_handler, make_menu("mood"), 0), 0);
	menu_t * ctrl = root.items[1]->data;
	menu_add_item(ctrl->items[0]->data, make_item(&show_menu, &switch_script, make_menu("script"), 1), 0);
	menu_add_item(ctrl->items[0]->data, make_item(&show_menu, &ctrl_mood, make_menu("dimm"), 1), 1);

	menu_add_item(&root, make_item(&show_menu, &menu_handler, make_menu("info"), 0), 2);
	menu_add_item(root.items[2]->data, make_item(&show_menu, &temp, make_menu("temp"), 0), 0);
}

uint8_t getKey(uint16_t delay) {  // keyport in header
	AvrXDelay(&switchtimer, delay);
	while (1) {
		
		if (!(PINB & (1 << PB0))) { // up
			return 0;
		}
		if (!(PINB & (1 << PB1))) { // down
			return 1;
		}
		if (!(PINB & (1 << PB2))) { // ok
			return 2;
		}
		if (!(PINB & (1 << PB3))) { // back
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
		key = getKey(550);
		// enter
		if (key == 2) {
			if (!(menu->items[sel]->type & 0x01))
				dispFillRect(0, 0, 40, 11 * menu->size + 1, 0);
			//pos.x += 20;
			menu->items[sel]->enter(menu->items[sel]->data); // enter
			goto DRAW_MEN;
		}

	// zurück
		if (key == 3) {
			//pos.x -= 20;
			if (menu->name != "/") { // nicht aus Hauptmenu springen
				dispFillRect(0, 0, 40, 11 * menu->size + 1, 0);
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
	dispDrawRect(pos.x, pos.y, 40, 12, 1);
	dispFillRect(pos.x + 1, pos.y + 1, 38, 10, selected);
	draw_Text(menu->name, pos.x + 2, pos.y + 2, 0, 1, !selected);
}
/*
void show_text(void *data, uint8_t selected) {
	menu_t *menu = data;
	
} */
/*
void show_mood(void *data, uint8_t selected) {
	menu_t *menu = data;
	dispDrawRect(50, pos.y, 65, 9, 1);
	dispFillRect(pos.x + 1, pos.y + 1, 39, 8, selected);
}
*/
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

void switch_blight(void *data, uint8_t selected) {
	uint8_t key;
	dispDrawRect(50, 15, 32, 5, 1);
	dispFillRect(51, 16, blightstat * 3, 3, 1);
	while (1) {  // keyhandeling
		key = getKey(550);
		if (key == 3) {
			dispFillRect(50, 15, 32, 5, 0);
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
			OCR2 = blightstat * 15;
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

void temp(void *data) {										// todo: start temp sending
	//static can_message_t msg = {0xa0, 0xa1, PORT_REMOTE, PORT_TEMP};
	//msg.addr_src = 0xa0;
	
	//msg.dlc = 1;
	//msg.data[0] = 0x90;
	while (1) {  // keyhandeling
		AvrXDelay(&switchtimer, 550);
		if (!(PINB & (1 << PB3))) { // ok
			dispFillRect(50, 30, 30, 8, 0);
			return;
		}
		dispFillRect(50, 30, 30, 8, 0);
		//can_put(&msg);
		AvrXDelay(&switchtimer, 5); // timing testen
		//draw_Text(bla, 50, 30, 0, 1, 1);
	}
}
/*
char* canmessage(uint8_t dst_adr, uint8_t dst_port, char* msg) {  // adresse, zielport, message
	can_message_t msg = {0xa0, 0, PORT_REMOTE, 0, 0};
	msg.dlc = strlen(msg);
	msg.port_dst = dst_port;
	msg.addr_dst = dst_adr;
	strcpy(msg.data, msg);
	can_put(&msg);
	AvrXDelay(&switchtimer, 5); // timing testen
	return *bla;
} */


void ctrl_mood(void *data) {
/*	uint8_t i, key;
	can_message_t msg = {0xa0, 0xa1, PORT_REMOTE, PORT_MOOD, 1};
	msg.data[0] = FKT_MOOD_GET_B;
	can_put(&msg);
	AvrXDelay(&switchtimer, 5);
	uint8_t val[8];
	memcpy(val, bla, 4);
	for (i = 0; i < 3; i++) {
		dispDrawRect(50, (10 * i), 30, 5, 1);
		dispFillRect(51, (10 * i) + 1, mc_POS_DIV(val[i], 9), 3, 1);
	}
	
	
	msg.dlc = 3;
	msg.data[0] = FKT_MOOD_SET_B;
	i = 0;
	dispFillRect(45, 1, 3, 3, 1);
	AvrXDelay(&switchtimer, 500);
	while (1) {  // keyhandeling
		key = getKey(60);
		if (key < 2) {
			dispFillRect(51, (10 * i) + 1, mc_POS_DIV(val[i], 9), 3, 0);
			if(key == 1 && val[i] != 0)
				val[i]--;
			if(key == 0 && val[i] != 255)
				val[i]++;
			dispFillRect(51, (10 * i) + 1, mc_POS_DIV(val[i], 9), 3, 1);
			msg.data[1] = i;
			msg.data[2] = val[i];
			can_put(&msg);
		}
		if (key == 2) {
			AvrXDelay(&switchtimer, 500);
			dispFillRect(45, (10 * i) + 1, 3, 3, 0);
			i++;
			if(i == 3)
				i = 0;
			dispFillRect(45, (10 * i) + 1, 3, 3, 1);
		}
		if (key == 3) {
			dispFillRect(45, (10 * i) + 1, 3, 3, 0);
			for (i = 0; i < 3; i++) {
				dispFillRect(50, 10 * i, 30, 5, 0);
	 		}
			return;
		}
	}*/
}
/*
void scan() {
	static can_message_t msg = {0xa0, 0xa1, PORT_REMOTE, PORT_TEMP};
	//msg.addr_src = 0xa0;
	msg.addr_dst++;  //temp.addr_dst = rx_msg.addr_src
	msg.dlc = 1;
	msg.data[0] = 0x90;
	can_put(&msg);
	AvrXDelay(&switchtimer, 5);
	uint8_t val[8];

}
*/
/*
scan()
	ping?
	antwort / timeout
	liste
	
	


*/


/*
setup
scan
	remove entrys
	scan + add entrys
	// kontrast
*/
/*
// adds an entry to a menu structure at pos


void menu_remove_entry(menu **menu_pt, uint16_t pos) {
	uint16_t x;
	for (x = pos; x < (*menu_pt)->entry_num-1; x++) {
		(*menu_pt)->entries[x] = (*menu_pt)->entries[x + 1];
	}
	(*menu_pt)->entry_num--;	                                
	(*menu_pt) = realloc((*menu_pt), 4 + ((*menu_pt)->entry_num) * sizeof(menu_entry));
}
*/
