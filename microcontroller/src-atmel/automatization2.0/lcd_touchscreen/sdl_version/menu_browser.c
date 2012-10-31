#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "gui_lib/gui.h"
#include "touchscreen.h"
#include "menu_browser.h"
#include "netvar/netvar.h"
#include "lap_button.h"
#include "lap_slider.h"


typedef struct {
	gui_container_t *main_container;
	//gui_container_t *element_container;
	uint8_t *dir_data;
	uint8_t *dir_position;
} menu_browser_state_t;

menu_browser_state_t state;

void parse_dir_to_main_container(uint8_t *dir_data) {

	gui_container_delete_all_childs(state.main_container);

	gui_container_t *akt_container = state.main_container;

	gui_container_t *container_stack[10];
	uint8_t stack_pointer = 0;

	uint8_t b;

	while ((b = *dir_data++) != 0) {
		switch (b) {
			case V_CONTAINER: {
				gui_container_t *c = new_gui_container();
				c->orientation = ORIENTATION_VERTICAL;
				container_stack[stack_pointer++] = akt_container;
				akt_container = c;
			} break;
			case H_CONTAINER: {
				gui_container_t *c = new_gui_container();
				c->orientation = ORIENTATION_HORIZONTAL;
				container_stack[stack_pointer++] = akt_container;
				akt_container = c;
			} break;
			case END_CONTAINER: {
				gui_container_t *new_container = akt_container;
				akt_container = container_stack[--stack_pointer];
				gui_container_add(akt_container, (gui_element_t *) new_container);
			} break;
			case GUI_BUTTON: {
				gui_button_t *b = new_gui_button();
				b->box.w = 32;
				b->box.h = 30;
				b->text = (char *) dir_data;
				dir_data += strlen((char *) dir_data) + 1;
				gui_container_add(akt_container, (gui_element_t *) b);
			} break;
			case CAN_BUTTON: {
				char *txt = (char *) dir_data;
				dir_data += strlen(txt) + 1;
				uint16_t idx = *(uint16_t*)dir_data;
				dir_data += 2;
				lap_button_t *b = new_lap_button(idx);
				b->box.w = 42;
				b->box.h = 40;
				b->text = txt;
				b->toggle_mode = 1;
				gui_container_add(akt_container, (gui_element_t *) b);
			} break;
			case V_SLIDER: {
				char *txt = (char *) dir_data;
				dir_data += strlen(txt) + 1;
				gui_slider_t *b = new_gui_slider();
				b->box.w = 32;
				b->box.h = 100;
				b->text = txt;
				b->orientation = ORIENTATION_VERTICAL;
				b->min_value = 0;
				b->max_value = 255;
				b->value = 30;
				gui_container_add(akt_container, (gui_element_t *) b);
			} break;
			case H_SLIDER: {
				char *txt = (char *) dir_data;
				dir_data += strlen(txt) + 1;
				gui_slider_t *b = new_gui_slider();
				b->box.w = 100;
				b->box.h = 32;
				b->text = txt;
				b->orientation = ORIENTATION_HORIZONTAL;
				b->min_value = 0;
				b->max_value = 255;
				b->value = 30;
				gui_container_add(akt_container, (gui_element_t *) b);
			} break;
			case CAN_V_SLIDER: {
				char *txt = (char *) dir_data;
				dir_data += strlen(txt) + 1;
				uint16_t idx = *(uint16_t*)dir_data;
				dir_data += 2;
				lap_slider_t *b = new_lap_slider(idx);
				b->box.w = 32;
				b->box.h = 100;
				b->text = txt;
				b->orientation = ORIENTATION_VERTICAL;
				b->min_value = 0;
				b->max_value = 255;
				b->value = 30;
				gui_container_add(akt_container, (gui_element_t *) b);
			} break;
			case CAN_H_SLIDER: {
				char *txt = (char *) dir_data;
				dir_data += strlen(txt) + 1;
				uint16_t idx = *(uint16_t*)dir_data;
				dir_data += 2;
				lap_slider_t *b = new_lap_slider(idx);
				b->box.w = 100;
				b->box.h = 32;
				b->text = txt;
				b->orientation = ORIENTATION_HORIZONTAL;
				b->min_value = 0;
				b->max_value = 255;
				b->value = 30;
				gui_container_add(akt_container, (gui_element_t *) b);
			} break;
			case GRAPH_VIEW: {
				char * txt = (char *) dir_data;
				dir_data += strlen(txt) + 1;
				uint8_t idx = *dir_data;
				dir_data += 1;
				gui_graph_view_t * b = new_gui_graph_view(txt, idx);
				b->box.w = 100;
				b->box.h = 100;
				b->data_size = 64;
				b->data = malloc(128);
				uint8_t x;
				for(x=0;x<64;x++)b->data[x] = x;
				gui_container_add(akt_container, (gui_element_t *) b);
			} break;
		}
	}
}


void menu_browser_set_dir(uint8_t *dir_data) {
	state.dir_data = dir_data;
	parse_dir_to_main_container(dir_data);
}

void menu_browser_init() {
	state.main_container = new_gui_container();
	state.main_container->box.y = 32;
	state.main_container->box.w = 320;
	state.main_container->box.h = 210;
}


void menu_browser_draw() {
	state.main_container->draw( (gui_element_t *) state.main_container, 0);
}


void menu_browser_touch_handler(touch_event_t t) {
	if (state.main_container) {
		state.main_container->touch_handler((gui_element_t *) state.main_container, t);
	}
}
