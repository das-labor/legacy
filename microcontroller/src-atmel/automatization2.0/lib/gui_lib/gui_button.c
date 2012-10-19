//
// This file is part of the gui-library for microcontrollers
// by Peter Fuhrmann.
//

#include <stdint.h>
#include <stdlib.h>
#include "gui.h"

void gui_button_draw(gui_element_t *self, uint8_t redraw) {
	gui_button_t *s = (gui_button_t*)self;

	if (s->state) {
		g_set_draw_color(1);
	} else {
		g_set_draw_color(0);
	}

	g_fill_rectangle(&s->box);

	if (s->state) {
		g_set_draw_color(0);
	} else {
		g_set_draw_color(1);
	}

	if (s->frame_size & 0x80) {
		g_draw_rectangle(&s->box);
	}
	rectangle_t r = s->box;

	if (s->icon) {
		uint8_t icon_x = (s->box.w - s->icon->width) / 2;
		g_draw_icon(s->box.x + icon_x, s->box.y + 1, s->icon);
		r.y += s->icon->height + 2;
		r.h -= s->icon->height + 2;
	}

	g_draw_string_in_rect(&r, s->text);
}

void gui_button_set_on_screen(gui_element_t *self, uint8_t state) {

}

void gui_button_touch_handler(gui_element_t *self, touch_event_t t) {
	gui_button_t *s = (gui_button_t*)self;

	if (t.flags & TOUCH_FLAG_DOWN) {
		//only take focus on press
		last_touched_gui_element = self;
	}

	if (s->toggle_mode) {
		if (t.flags & TOUCH_FLAG_DOWN) {
			s->state ^= 1;
			s->draw(self, 0);
			if (s->click_handler) {
				s->click_handler(self);
			}
		}
	} else {
		if (t.flags & TOUCH_FLAG_DOWN) {
			s->state = 1;
			s->draw(self, 0);
		}

		if (t.flags & TOUCH_FLAG_UP) {
			s->state = 0;
			s->draw(self, 0);
			if (s->click_handler) {
				s->click_handler(self);
			}
		}
	}
}


void gui_button_delete(gui_element_t *self) {
	free(self);
}

void gui_button_update_position(gui_element_t *self, int16_t x_diff, int16_t y_diff) {
	gui_button_t *s = (gui_button_t *) self;

	s->box.x += x_diff;
	s->box.y += y_diff;
}


//constructor
gui_button_t *new_gui_button() {
	gui_button_t *b = malloc(sizeof(gui_button_t));
	b->draw = gui_button_draw;
	b->set_on_screen = gui_button_set_on_screen;
	b->touch_handler = gui_button_touch_handler;
	b->update_position = gui_button_update_position;
	b->delete = gui_button_delete;
	b->box = (rectangle_t){0, 0, 0, 0};
	b->icon = 0;
	b->state = 0;
	b->click_handler = 0;
	b->frame_size = 0x80;
	b->toggle_mode = 0;
	return b;
}

