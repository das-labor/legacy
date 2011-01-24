
#include <stdlib.h>
#include "gui_lib/gui.h"
#include "netvar/netvar.h"

typedef struct {
	void(*draw)(gui_element_t * self, uint8_t redraw);
	void(*set_on_screen)(gui_element_t *self, uint8_t state);
	void(*touch_handler)(gui_element_t *self, touch_event_t t);
	void(*update_position)(gui_element_t *self, int16_t x_diff, int16_t y_diff); //moves an element and all sub elements.
	void(*delete)(gui_element_t *self);
	rectangle_t box;//gui element sets height and width, container sets x and y.

	char * text;
	icon_t * icon;
	uint8_t state;

	void(*click_handler)(gui_element_t *);
	// -------------------- inherited from gui_button_t until here -------------

	netvar_desc * nv;	
}lap_button_t;

void lap_button_click_handler(gui_element_t * self){
	lap_button_t * s = (lap_button_t*)self;
	uint8_t new_state = s->state ^ 1;
	netvar_write(s->nv, &new_state);
}

void lap_button_nv_handler(netvar_desc * nd, void * ref){
	lap_button_t * s = (lap_button_t*) ref;
	s->state = nd->data[0];
	//printf("nv_handler nd=%X, ref=%X\r\n", nd, ref);
	s->draw(s, 0);
}

void lap_button_delete (gui_element_t *self) {
	free (self);
}



//constructor
lap_button_t * new_lap_button(char * text, uint16_t idx ){
	lap_button_t * b = malloc(sizeof(lap_button_t));
	b->draw = gui_button_draw;
	b->set_on_screen = gui_button_set_on_screen;
	b->touch_handler = gui_button_touch_handler;
	b->update_position = gui_button_update_position;
	b->delete = lap_button_delete;
	b->box = (rectangle_t){0,0,32,30};
	b->icon = 0;
	b->state = 0;
	
	b->text = text;
	b->click_handler = lap_button_click_handler;
	
	b->nv = netvar_register(idx,0,1);
	netvar_add_handler(b->nv, lap_button_nv_handler, b);
	
	return b;
	
}
