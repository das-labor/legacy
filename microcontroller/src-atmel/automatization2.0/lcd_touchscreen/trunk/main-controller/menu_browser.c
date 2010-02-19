#include <stdint.h>
#include <util/delay.h>
#include <string.h>
#include "gui.h"
#include "touchscreen.h"
#include "dc_com.h"


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

typedef struct {
	gui_container_t * main_container;
	//gui_container_t * element_container;
	uint8_t * dir_data;
	uint8_t * dir_position;
} menu_browser_state_t;

menu_browser_state_t state;

#define V_CONTAINER   0x10
#define H_CONTAINER   0x11
#define END_CONTAINER 0x1f
#define CAN_BUTTON    0x20

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






void parse_dir_to_main_container (uint8_t * dir_data) {

	gui_container_delete_all_childs(state.main_container);
	
	gui_container_t * akt_container = state.main_container;
	
	gui_container_t * container_stack[10];
	uint8_t stack_pointer = 0;

	uint8_t b;
		
	while((b = *dir_data++) != 0){
		switch(b){
			case CAN_BUTTON: {
				gui_button_t * b = new_gui_button();
				b->box.w = 32;
				b->box.h = 30;
				b->text = (char *) dir_data;
				dir_data += strlen((char *) dir_data) + 1;
				gui_container_add(akt_container,(gui_element_t *) b);
			} break;
			case V_CONTAINER: {
				gui_container_t * c = new_gui_container();
				c->orientation = ORIENTATION_VERTICAL;
				container_stack[stack_pointer++] = akt_container;
				akt_container = c;
			} break;
			case H_CONTAINER: {
				gui_container_t * c = new_gui_container();
				c->orientation = ORIENTATION_HORIZONTAL;
				container_stack[stack_pointer++] = akt_container;
				akt_container = c;
			} break;
			case END_CONTAINER: {
				gui_container_t * new_container = akt_container;
				akt_container = container_stack[--stack_pointer];
				gui_container_add(akt_container, (gui_element_t *) new_container);

			} break;
			
		}
	}
}


void menu_browser_set_dir (uint8_t * dir_data) {
	state.dir_data = dir_data;
	parse_dir_to_main_container(dir_data);
	
}

void menu_browser_init () {
	state.main_container = new_gui_container();
	state.main_container->box.y = 32;
	state.main_container->box.w = 320;
	state.main_container->box.h = 210;
	
	
	menu_browser_set_dir(test_dir);
	
	
	/*
	
	gui_container_t * akt_container;
	
	
	
	
	{
				gui_container_t * c = new_gui_container();
				c->orientation = ORIENTATION_VERTICAL;
				akt_container = c;
	}
	
	
	
	{
				gui_button_t * b = new_gui_button();
				b->box.w = 32;
				b->box.h = 30;

				b->text = "Hallo";
				gui_container_add(akt_container,(gui_element_t *) b);
	}{
				gui_button_t * b = new_gui_button();
				b->box.w = 32;
				b->box.h = 30;

				b->text = "Hallo";
				gui_container_add(akt_container,(gui_element_t *) b);
	}
	gui_container_add(state.main_container, akt_container);



	{
				gui_container_t * c = new_gui_container();
				c->orientation = ORIENTATION_VERTICAL;
				akt_container = c;
	}
	
	//menu_browser_set_dir(test_dir);
	
	{
				gui_button_t * b = new_gui_button();
				b->box.w = 32;
				b->box.h = 30;

				b->text = "Hallo1";
				gui_container_add(akt_container,(gui_element_t *) b);
	}
	gui_container_add(state.main_container, akt_container);

	{
				gui_container_t * c = new_gui_container();
				c->orientation = ORIENTATION_VERTICAL;
				akt_container = c;
	}
	
	//menu_browser_set_dir(test_dir);
	
	{
				gui_button_t * b = new_gui_button();
				b->box.w = 32;
				b->box.h = 30;

				b->text = "Hall2";
				gui_container_add(akt_container,(gui_element_t *) b);
	}{
				gui_button_t * b = new_gui_button();
				b->box.w = 32;
				b->box.h = 30;

				b->text = "Hallo 345678";
				gui_container_add(akt_container,(gui_element_t *) b);
	}
	
		{
				gui_button_t * b = new_gui_button();
				b->box.w = 32;
				b->box.h = 30;

				b->text = "Test";
				gui_container_add(state.main_container,(gui_element_t *) b);
	}
	

	
	gui_container_add(state.main_container, akt_container);
	*/

}


void menu_browser_draw () {
	state.main_container->draw( (gui_element_t *) state.main_container, 0);
}


void menu_browser_touch_handler(touch_event_t t){
	if(state.main_container){
		state.main_container->touch_handler ( (gui_element_t *) state.main_container, t);
	}
}



void init_menu() {
	navigation_bar_init();
	navigation_bar_draw();
	
	menu_browser_init();
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
