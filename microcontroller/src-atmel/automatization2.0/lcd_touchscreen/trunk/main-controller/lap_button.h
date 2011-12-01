#ifndef _LAP_BUTTON_H
#define _LAP_BUTTON_H

typedef struct {
	void(*draw)(gui_element_t * self, uint8_t redraw);
	void(*set_on_screen)(gui_element_t *self, uint8_t state);
	void(*touch_handler)(gui_element_t *self, touch_event_t t);
	void(*update_position)(gui_element_t *self, int16_t x_diff, int16_t y_diff); //moves an element and all sub elements.
	void(*delete)(gui_element_t *self);
	rectangle_t box;//gui element sets height and width, container sets x and y.

	uint8_t frame_size;//how many pixels frame? 0x80+num draws bounding box
	char * text;
	icon_t * icon;
	uint8_t state;

	void(*click_handler)(gui_element_t *);
	// -------------------- inherited from gui_button_t until here -------------

	netvar_desc * nv;
} lap_button_t;

void lap_button_click_handler(gui_element_t * self);
void lap_button_delete (gui_element_t *self);
//constructor
lap_button_t * new_lap_button(char * text, uint16_t idx);

#endif //_LAP_BUTTON_H
