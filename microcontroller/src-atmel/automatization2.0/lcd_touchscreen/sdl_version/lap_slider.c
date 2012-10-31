#include <stdlib.h>

#include "gui_lib/gui.h"
#include "netvar/netvar.h"
#include "lap_slider.h"


void lap_slider_value_changed(gui_element_t *self, int16_t value) {
	lap_slider_t *s = (lap_slider_t*)self;
	uint8_t val[] = {s->value & 0xff, s->value >> 8};
	netvar_write(s->nv, val);
}


void lap_slider_nv_handler(netvar_desc *nd, void *ref) {
	lap_slider_t *s = (lap_slider_t*) ref;
	s->value = nd->data[0];
	s->value += nd->data[1] << 8;
	s->draw((gui_element_t *)s, 1);
}

void lap_slider_delete(gui_element_t *self) {
	free(self);
}

//constructor
lap_slider_t *new_lap_slider(uint16_t idx) {
	lap_slider_t *b = malloc(sizeof(lap_slider_t));
	b->draw = gui_slider_draw;
	b->set_on_screen = gui_slider_set_on_screen;
	b->touch_handler = gui_slider_touch_handler;
	b->update_position = gui_slider_update_position;
	b->delete = lap_slider_delete;
	b->box = (rectangle_t){0, 0, 0, 0};
	b->icon = 0;
	b->value_changed = lap_slider_value_changed;
	b->text = 0;
	b->orientation = 0; // vertical - default
	b->nv = netvar_register(idx, 0, 2);
	netvar_add_handler(b->nv, lap_slider_nv_handler, b);
	return b;
}
