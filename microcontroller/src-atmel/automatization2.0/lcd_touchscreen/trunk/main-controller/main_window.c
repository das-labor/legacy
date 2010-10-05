
#include <stdint.h>
#include "dc_com.h"
#include "gui_lib/gui.h"
#include "touchscreen.h"
#include "menu_browser.h"

uint8_t test_dir[] = {
	V_CONTAINER,
		CAN_BUTTON, 'H','a','l','l','o',0,
		CAN_BUTTON, 'H','a','l','l','o','1',0,
	END_CONTAINER,
	V_CONTAINER,
	V_CONTAINER,
		CAN_BUTTON, 'H','a','l','l','o',0,
		CAN_BUTTON, 'H','a','l','l','o','1',0,
		CAN_BUTTON, 'H','a','l','l','o',0,
		CAN_BUTTON, 'H','a','l','l','o','1',0,
	END_CONTAINER,
	END_CONTAINER,
	V_CONTAINER,
		CAN_BUTTON, 'H','a','l','l','o',0,
		CAN_BUTTON, 'H','a','l','l','o','1',0,
		CAN_BUTTON, 'H','a','l','l','o',0,
		CAN_BUTTON, 'H','a','l','l','o','1',0,
		CAN_BUTTON, 'H','a','l','l','o',0,
		CAN_BUTTON, 'H','a','l','l','o','1',0,
	END_CONTAINER,
	V_CONTAINER,
		CAN_BUTTON, 'H','a','l','l','o',0,
		CAN_BUTTON, 'H','a','l','l','o','1',0,
		CAN_BUTTON, 'H','a','l','l','o',0,
		CAN_BUTTON, 'H','a','l','l','o','1',0,
	END_CONTAINER,
	V_CONTAINER,
		CAN_BUTTON, 'H','a','l','l','o',0,
		CAN_BUTTON, 'H','a','l','l','o','1',0,
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


void navigation_bar_init () {
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


void navigation_bar_draw () {	
	bar->draw((gui_element_t *)bar,0);
}


void init_main_window() {
	navigation_bar_init();
	navigation_bar_draw();
	
	menu_browser_init();
	menu_browser_set_dir(test_dir);
	menu_browser_draw();	
}


void handle_touch (uint16_t x, uint16_t y, uint8_t click) {
	touch_event_t t;
	t.x = x;
	t.y = y;
	t.click = click;
	bar->touch_handler( (gui_element_t *) bar, t);
	menu_browser_touch_handler(t);
}


void handle_touchscreen() {
	static uint16_t click_timer = 0;
  	static pixel p1 = {-1, -1};
  	
	pixel p;
	p = read_touch_screen_coordinates();

	uint8_t click = 0;

	if (p.x != -1 && p1.x == -1 && click_timer == 0) {
		click = 1;
		click_timer = 20;
	}

	if (p.x != -1) {
		handle_touch(p.x, p.y, click);
	}

	if (click_timer > 0) {
		click_timer--;
	}

//	draw_menu(0);

	p1 = p;
}
