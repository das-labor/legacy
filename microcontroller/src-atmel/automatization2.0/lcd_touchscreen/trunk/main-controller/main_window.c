
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


#include "gui_lib/graphics.h"
#include "gui_lib/gui.h"
#include "touchscreen.h"
#include "menu_browser.h"

uint8_t test_dir[] = {
/*
	V_CONTAINER,
		CAN_BUTTON, 'H','a','l','l','o',0, 0x11, 0x11,
		CAN_BUTTON, 'H','a','l','l','o','1',0, 0x11, 0x11,
	END_CONTAINER,

	V_CONTAINER,
		CAN_BUTTON, 'H','a','l','l','o',0, 0x11, 0x11,

		CAN_BUTTON, 'H','a','l','l','o','1',0, 0x11, 0x11,
		CAN_BUTTON, 'H','a','l','l','o',0, 0x11, 0x11,
		CAN_BUTTON, 'H','a','l','l','o','1',0, 0x11, 0x11,

	END_CONTAINER,
*/

	V_CONTAINER,
		CAN_BUTTON, 'H','a','l','l','o',0,     0x10, 0x11,
		CAN_BUTTON, 'H','a','l','l','o','1',0, 0x11, 0x11,
		CAN_BUTTON, 'H','a','l','l','o',0,     0x12, 0x11,
		CAN_BUTTON, 'H','a','l','l','o','1',0, 0x13, 0x11,
		CAN_BUTTON, 'H','a','l','l','o',0,     0x14, 0x11,
		CAN_BUTTON, 'H','a','l','l','o','1',0, 0x15, 0x11,
		CAN_BUTTON, 'H','a','l','l','o',0,     0x16, 0x11,
	END_CONTAINER,

	V_CONTAINER,
		CAN_BUTTON, 'H','a','l','l','o',0,     0x10, 0x11,
		CAN_BUTTON, 'H','a','l','l','o','1',0, 0x11, 0x11,
		CAN_BUTTON, 'H','a','l','l','o',0,     0x12, 0x11,
		CAN_BUTTON, 'H','a','l','l','o','1',0, 0x13, 0x11,
		CAN_BUTTON, 'H','a','l','l','o',0,     0x14, 0x11,
		CAN_BUTTON, 'H','a','l','l','o','1',0, 0x15, 0x11,
		CAN_BUTTON, 'H','a','l','l','o',0,     0x16, 0x11,
	END_CONTAINER,

	V_CONTAINER,
		CAN_BUTTON, 'H','a','l','l','o',0,     0x10, 0x11,
		CAN_BUTTON, 'H','a','l','l','o','1',0, 0x11, 0x11,
		CAN_BUTTON, 'H','a','l','l','o',0,     0x12, 0x11,
		CAN_BUTTON, 'H','a','l','l','o','1',0, 0x13, 0x11,
		CAN_BUTTON, 'H','a','l','l','o',0,     0x14, 0x11,
		CAN_BUTTON, 'H','a','l','l','o','1',0, 0x15, 0x11,
		CAN_BUTTON, 'H','a','l','l','o',0,     0x16, 0x11,
	END_CONTAINER,

	V_CONTAINER,
		CAN_BUTTON, 'H','a','l','l','o',0,     0x10, 0x11,
		CAN_BUTTON, 'H','a','l','l','o','1',0, 0x11, 0x11,
		CAN_BUTTON, 'H','a','l','l','o',0,     0x12, 0x11,
		CAN_BUTTON, 'H','a','l','l','o','1',0, 0x13, 0x11,
		CAN_BUTTON, 'H','a','l','l','o',0,     0x14, 0x11,
		CAN_BUTTON, 'H','a','l','l','o','1',0, 0x15, 0x11,
		CAN_BUTTON, 'H','a','l','l','o',0,     0x16, 0x11,
	END_CONTAINER,



/*
	V_CONTAINER,
		CAN_BUTTON, 'H','a','l','l','o',0, 0x10, 0x11,
		CAN_BUTTON, 'H','a','l','l','o','1',0, 0x11, 0x11,
		CAN_BUTTON, 'H','a','l','l','o',0, 0x11, 0x11,
		CAN_BUTTON, 'H','a','l','l','o','1',0, 0x11, 0x10,
	END_CONTAINER,
*/

	V_CONTAINER,
		H_SLIDER, 'H','a','l','l','o',0,
		H_SLIDER, 'H','a','l','l','o',0,
	END_CONTAINER,


	0
};


extern icon_t room_icon;
extern icon_t escape_icon;
extern icon_t main_icon;

gui_container_t * bar;
gui_button_t * escape_button;
gui_button_t * room_button;
gui_button_t * main_button;
gui_button_t * status_label;


uint16_t mem_available(){
	uint16_t min = 1;
	uint16_t max = 4096;
	uint16_t val = 0;
	
	while((max - min) > 2){
		val = (min+max)/2;
		void * x = malloc(val);
		if(x){
			free(x);
			min = val;
		}else{
			max = val;
		}
	
	}
	return val;
	
}

char status_string[32] = "Hallo";

void onclick(){
	malloc(23);
}

void navigation_bar_init() {
	bar = new_gui_container();
	bar->box.w = 320;
	bar->box.h = 30;
	bar->frame_size = 0x80;
	
	escape_button = new_gui_button();
	escape_button->box.w = 32;
	escape_button->box.h = 30;
	escape_button->text = "esc";
	escape_button->icon = &escape_icon;
	
	room_button = new_gui_button();
	room_button->box.w = 32;
	room_button->box.h = 30;
	room_button->text = "room";
	room_button->icon = &room_icon;
	
	main_button = new_gui_button();
	main_button->box.w = 32;
	main_button->box.h = 30;
	main_button->text = "main";
	main_button->icon = &main_icon;
	main_button->click_handler =onclick;

	status_label = new_gui_button();
	status_label->touch_handler = 0;
	status_label->box.w = 80;
	status_label->box.h = 30;
	status_label->text = status_string;

	
	gui_container_add(bar, (gui_element_t *) escape_button);
	gui_container_add(bar, (gui_element_t *) room_button);
	gui_container_add(bar, (gui_element_t *) main_button);
	gui_container_add(bar, (gui_element_t *) status_label);
}

void handle_status(){
	static uint8_t ticks;
	ticks++;
	if(ticks == 100){
		ticks = 0;
		sprintf(status_string, "mem free: %d", mem_available());
		status_label->draw(status_label, 0);
	}

}

void navigation_bar_draw () {
	bar->draw((gui_element_t *)bar, 0);
}


void init_main_window() {
	navigation_bar_init();
	navigation_bar_draw();

	menu_browser_init();

	menu_browser_set_dir(test_dir);

	menu_browser_draw();
}


void handle_touch (touch_event_t t) {
	bar->touch_handler( (gui_element_t *) bar, t);
	menu_browser_touch_handler(t);
}

void handle_touchscreen() {
	static pixel p1 = {-1, -1};

	pixel p;
	p = read_touch_screen_coordinates();

	uint8_t down = 0, up = 0;
	if (p.x != -1 && p1.x == -1) {
		down = 1;
	}

	if (p1.x != -1 && p.x == -1) {
		up = 1;
	}


	uint8_t flags = 0;

	if (down) {
		flags |= TOUCH_FLAG_DOWN;
	}

	if (up) {
		flags |= TOUCH_FLAG_UP;
	}

	touch_event_t t;
	t.x = p.x;
	t.y = p.y;
	t.flags = flags;

	if (p.x != -1) {
		handle_touch(t);
	} else if (last_touched_gui_element != 0) {
		last_touched_gui_element->touch_handler(last_touched_gui_element, t);
		last_touched_gui_element = 0;
	}

	p1 = p;
}

