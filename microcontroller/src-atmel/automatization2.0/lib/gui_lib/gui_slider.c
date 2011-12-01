//
// This file is part of the gui-library for microcontrollers 
// by Peter Fuhrmann.
//

#include <stdint.h>
#include <stdlib.h>
#include "gui.h"

#define SLIDER_SIZE 5

static rectangle_t calculate_slider_rectangle(gui_slider_t * s) {
	rectangle_t r;

	int range_size = s->range_rectangle.h - SLIDER_SIZE;

	int pos = (s->value - s->min_value) * range_size / (s->max_value - s->min_value);

	r.y = s->range_rectangle.y + s->range_rectangle.h - SLIDER_SIZE - pos;
	r.x = s->range_rectangle.x;
	r.w = s->range_rectangle.w;
	r.h = SLIDER_SIZE;

	return r;
}

static void clear_inner_rectangle(rectangle_t r) {
	r.x += 1;
	r.y += 1;
	r.w -= 2;
	r.h -= 2;

	g_set_draw_color(0);
	g_fill_rectangle(&r);
	g_set_draw_color(1);
}

void gui_slider_draw(gui_element_t * self, uint8_t redraw) {
	gui_slider_t * s = (gui_slider_t*)self;

	if (!redraw) {
		g_set_draw_color(0);
		g_fill_rectangle(&s->box);
		g_set_draw_color(1);
		g_draw_rectangle(&s->box);
	}

	rectangle_t r = s->box;

	if (s->icon) {
		uint8_t icon_x = (s->box.w - s->icon->width) / 2;
		g_draw_icon(s->box.x + icon_x, s->box.y, s->icon);
		r.y += s->icon->height;
		r.h -= s->icon->height;
	}

	g_draw_string_in_rect(&r, s->text);
	uint8_t text_height = g_get_last_text_height();
	r.y += text_height;
	r.h -= text_height;

	char valstr[6];
	itoa(s->value, valstr, 10);
	if (redraw) {
		rectangle_t tr = r;
		tr.h = text_height;
		clear_inner_rectangle(tr);
	}
	g_draw_string_in_rect(&r, valstr);

	text_height = g_get_last_text_height();

	r.y += text_height + 2;
	r.h -= text_height + 4;
	r.x += 2;
	r.w -= 4;

	s->range_rectangle = r;
	g_draw_rectangle(&r);

	rectangle_t slider_rect = calculate_slider_rectangle(s);

	if (redraw) {
		clear_inner_rectangle(r);
	}

	g_fill_rectangle(&slider_rect);
}

void gui_slider_set_on_screen(gui_element_t *self, uint8_t state) {

}

void gui_slider_touch_handler(gui_element_t *self, touch_event_t t) {
	gui_slider_t * s = (gui_slider_t*)self;
	
	int range_size = s->range_rectangle.h - SLIDER_SIZE;
	int offset = s->range_rectangle.y + s->range_rectangle.h - 2 - t.y;
	int val = (s->max_value - s->min_value) * offset / range_size;
	
	if (val > s->max_value) val = s->max_value;
	if (val < s->min_value) val = s->min_value;
	
	if (s->value != val) {
		s->value = val;
		s->draw(self, 1);

		if (s->value_changed) {
			s->value_changed(self, s->value);
		}
	}
}


void gui_slider_delete(gui_element_t *self) {
	free(self);
}

void gui_slider_update_position(gui_element_t * self, int16_t x_diff, int16_t y_diff) {
	gui_slider_t * s = (gui_slider_t *) self;

	s->box.x += x_diff;
	s->box.y += y_diff;
}


//constructor
gui_slider_t * new_gui_slider() {
	gui_slider_t * b = malloc(sizeof(gui_slider_t));
	b->draw = gui_slider_draw;
	b->set_on_screen = gui_slider_set_on_screen;
	b->touch_handler = gui_slider_touch_handler;
	b->update_position = gui_slider_update_position;
	b->delete = gui_slider_delete;
	b->box = (rectangle_t){0, 0, 0, 0};
	b->icon = 0;
	b->value_changed = 0;
	return b;
}

