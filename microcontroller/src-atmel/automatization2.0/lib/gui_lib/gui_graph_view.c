//
// This file is part of the gui-library for microcontrollers 
// by Peter Fuhrmann.
//

#include <stdint.h>
#include <stdlib.h>
#include "gui.h"

void gui_graph_view_draw (gui_element_t * self, uint8_t redraw) {
	gui_graph_view_t * s = (gui_graph_view_t*) self;

	g_set_draw_color(0);		
	g_fill_rectangle   (&s->box);
	
	g_set_draw_color(1);	
	g_draw_rectangle   (&s->box);

	rectangle_t r = s->box;
	
	if(s->icon){
		uint8_t icon_x = (s->box.w - s->icon->width)/2; 
		g_draw_icon(s->box.x + icon_x, s->box.y, s->icon);
		r.y += s->icon->height;
		r.h -= s->icon->height;
	}
	
	g_draw_string_in_rect(&r, s->text);
	
	
	g_set_draw_color(1);		
	
	uint16_t x;
	for(x=0 ; x < s->data_size ; x++){
		g_draw_pixel(s->box.x + x, s->box.y + s->box.h - s->data[x]);
	}
}

void gui_graph_view_set_on_screen (gui_element_t *self, uint8_t state) {

}

void gui_graph_view_touch_handler (gui_element_t *self, touch_event_t t) {
	//gui_graph_view_t * s = (gui_graph_view_t*)self;
	
	
}


void gui_graph_view_delete (gui_element_t *self) {
	free (self);
}

void gui_graph_view_update_position(gui_element_t * self, int16_t x_diff, int16_t y_diff) {
	gui_graph_view_t * s = (gui_graph_view_t *) self;
			
	s->box.x += x_diff;
	s->box.y += y_diff;
}


//constructor
gui_graph_view_t * new_gui_graph_view(char * txt, uint8_t idx) {
	gui_graph_view_t * b = malloc(sizeof(gui_graph_view_t));
	b->draw = gui_graph_view_draw;
	b->set_on_screen = gui_graph_view_set_on_screen;
	b->touch_handler = gui_graph_view_touch_handler;
	b->update_position = gui_graph_view_update_position;
	b->delete = gui_graph_view_delete;
	b->box = (rectangle_t){0,0,0,0};
	b->icon = 0;
	b->text = txt;
	b->idx = idx;
	b->data_size = 0;
	return b;
}
