#include <stdlib.h>

#include "gui_lib/gui.h"
#include "netvar/netvar.h"
#include "lap_slider.h"


static void lap_slider_value_changed (gui_element_t *self, int16_t value) {
	lap_slider_t *s = (lap_slider_t*)self;
	netvar_write(s->out_nv, &value);
}

void lap_slider_nv_handler(netvar_desc *nd, void *ref) {
	gui_slider_t *s = (gui_slider_t*) ref;
	gui_slider_set_value(s, nd->data[0], 0);
}

void lap_slider_delete(gui_element_t *self) {
	free(self);
}

//constructor
lap_slider_t *new_lap_slider(uint16_t idx, uint8_t sidx) {
	lap_slider_t *b = malloc(sizeof(lap_slider_t));
	b->draw = gui_slider_draw;
	b->set_on_screen = gui_slider_set_on_screen;
	b->touch_handler = gui_slider_touch_handler;
	b->update_position = gui_slider_update_position;
	b->delete = lap_slider_delete;
	b->box = (rectangle_t) {0, 0, 0, 0};
	b->icon = 0;
	b->value_changed = lap_slider_value_changed;
	b->orientation = ORIENTATION_VERTICAL; // vertical - default
	
//  Lamp controller:
//	0x40-0x4f   slider inputs
//	0x50-0x5f   slider outputs

	b->in_nv = netvar_register(idx, 0x50 + sidx, 1); //size = 1
	netvar_add_handler(b->in_nv, lap_slider_nv_handler, b);

	b->out_nv = netvar_register(idx, 0x40 + sidx, 1); //size = 1
	
	return b;
}
