#include <stdlib.h>

#include "gui_lib/gui.h"
#include "netvar/netvar.h"
#include "lap_button.h"


void lap_button_click_handler(gui_element_t *self) {
	lap_button_t *s = (lap_button_t*)self;
	netvar_write(s->nv, &s->state);
}

void lap_button_nv_handler(netvar_desc *nd, void *ref) {
	lap_button_t *s = (lap_button_t*) ref;
	s->state = nd->data[0];
	//printf("nv_handler nd=%X, ref=%X\r\n", nd, ref);
	s->draw((gui_element_t *)s, 0);
}

void lap_button_delete (gui_element_t *self) {
	free(self);
}


//constructor
lap_button_t *new_lap_button(uint16_t idx, uint8_t sidx) {
	lap_button_t *b = malloc(sizeof(lap_button_t));
	b->draw = gui_button_draw;
	b->set_on_screen = gui_button_set_on_screen;
	b->touch_handler = gui_button_touch_handler;
	b->update_position = gui_button_update_position;
	b->delete = lap_button_delete;
	b->box = (rectangle_t){0, 0, 32, 30};
	b->icon = 0;
	b->state = 0;
	b->frame_size = 0x80;
	b->text = 0;
	b->toggle_mode = 1;
	b->click_handler = lap_button_click_handler;

	b->nv = netvar_register(idx, sidx, 1);
	netvar_add_handler(b->nv, lap_button_nv_handler, b);
	

	return b;
}
