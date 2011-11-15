//
// This file is part of the gui-library for microcontrollers 
// by Peter Fuhrmann.
//

#include <stdint.h>
#include <stdlib.h>
#include "gui.h"


//a number widget.//////////////////////////////////////////////////////////////
//it takes getter and setter funktions for setting the number so instant
//response to changes is possible.
typedef struct {
	void(*draw)(gui_element_t * self, uint8_t redraw);
	void(*set_on_screen)(gui_element_t *self, uint8_t state);
	void(*touch_handler)(gui_element_t *self, touch_event_t t);
	void(*update_position)(gui_element_t *self, int16_t x_diff, int16_t y_diff); //moves an element and all sub elements.
	void(*delete)(gui_element_t *self);
	rectangle_t box;//gui element sets height and width, container sets x and y.

	char * text;
	icon_t * icon;

	//ref is a user definable reference for use with the set/get functions
	int (*get)(void * ref);
	void(*set)(void * ref, int value);
	void * ref;

} gui_number_widget_t;



void gui_number_widget_draw (gui_element_t * self, uint8_t redraw) {
	gui_number_widget_t * s = (gui_number_widget_t*)self;

	g_set_draw_color(0);

	g_fill_rectangle(&s->box);

	if (s->state) {
		g_set_draw_color(0);
	} else {
		g_set_draw_color(1);
	}

	g_draw_rectangle(&s->box);

	rectangle_t r = s->box;

	/*
	if(s->icon){
		uint8_t icon_x = (s->box.w - s->icon->width)/2; 
		g_draw_icon(s->box.x + icon_x, s->box.y, s->icon);
		r.y += s->icon->height;
		r.h -= s->icon->height;
	}
	*/


	g_draw_string_in_rect(&r, s->text);

	r.y += 100;
	r.h -= 100;

	char strbuf[12];
	sprintf(strbuf, s->get(s->ref));
	g_draw_string_in_rect(&r, strbuf);
}

void gui_number_widget_set_on_screen (gui_element_t *self, uint8_t state) {

}

void gui_number_widget_touch_handler (gui_element_t *self, touch_event_t t) {
	gui_number_widget_t * s = (gui_number_widget_t*)self;

	if (t.click) {
		s->state ^= 1;
	}
	self->draw(self, 0);
}


void gui_number_widget_delete (gui_element_t *self) {
	free (self);
}


void gui_number_widget_update_position(gui_element_t * self, int16_t x_diff, int16_t y_diff) {
	self->box.x += x_diff;
	self->box.y += y_diff;
}

//constructor
gui_number_widget_t * new_gui_number_widget() {
	gui_number_widget_t * s = malloc(sizeof(gui_number_widget_t));
	s->draw = gui_number_widget_draw;
	s->set_on_screen = gui_number_widget_set_on_screen;
	s->touch_handler = gui_number_widget_touch_handler;
	s->update_position = gui_number_widget_update_position;
	s->delete = gui_number_widget_delete;
	s->box = (rectangle_t){0,0,0,0};
	s->icon = 0;
	s->state = 0;
	return b;
}

