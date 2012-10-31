
#include <stdint.h>

#include "gui_lib/graphics.h"
#include "gui_lib/gui.h"
#include "touchscreen.h"
#include "menu_browser.h"

uint8_t test_dir[] = {
	H_CONTAINER,
		CAN_BUTTON, 'L', 'i', 'c', 'h', 't', 0, 0x11, 0x11,
		CAN_H_SLIDER, 'd', 'r', '4', 'g', 'o', 0, 0x11, 0x12,
	END_CONTAINER,

	V_CONTAINER,
		CAN_V_SLIDER, 'V', '-', 'R', 'a', 'u', 'm', 0, 0x11, 0x13,
	END_CONTAINER,
	0
};

/*
uint8_t test_dir[] = {
	GRAPH_VIEW, 'H','a','l','l','o',0, 0x23,0
};*/


extern icon_t room_icon;
extern icon_t escape_icon;
extern icon_t main_icon;

gui_container_t *bar;
gui_button_t *escape_button;
gui_button_t *room_button;
gui_button_t *main_button;


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

	gui_container_add(bar, (gui_element_t *) escape_button);
	gui_container_add(bar, (gui_element_t *) room_button);
	gui_container_add(bar, (gui_element_t *) main_button);
}


void navigation_bar_draw() {
	bar->draw((gui_element_t *)bar, 0);
}


void init_main_window() {
	navigation_bar_init();
	navigation_bar_draw();
	menu_browser_init();
	menu_browser_set_dir(test_dir);
	menu_browser_draw();
}


void handle_touch(touch_event_t t) {
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
