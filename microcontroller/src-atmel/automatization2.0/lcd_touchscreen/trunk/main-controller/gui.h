//
// This file is part of the gui-library for microcontrollers 
// by Peter Fuhrmann.
//

// Main gui header file - it contains the api.

#ifndef _H_GUI
#define _H_GUI

#include "rectangle.h"
#include "list.h"
#include "dc_com.h"

//needed for touchscreen ///////////////////////////////////////////////////////
typedef struct{
	uint16_t x;
	uint16_t y;
	uint8_t click;
}touch_event_t;
////////////////////////////////////////////////////////////////////////////////

//gui element is the abstract base class of all elements////////////////////////

typedef struct gui_element_t_s gui_element_t;

struct gui_element_t_s {
	void(*draw)(gui_element_t * self, uint8_t redraw);
	void(*set_on_screen)(gui_element_t *self, uint8_t state);
	void(*touch_handler)(gui_element_t *self, touch_event_t t);
	void(*update_position)(gui_element_t *self, int16_t x_diff, int16_t y_diff); //moves an element and all sub elements.
	void(*delete)(gui_element_t *self); //not necessarily a destructor - it might only remove the object from the screen
	rectangle_t box;//gui element sets height and width, container sets x and y.
};

////////////////////////////////////////////////////////////////////////////////


//gui container ////////////////////////////////////////////////////////////////
//it can put elements beside each other in horizontal or vertical direction.
//the border size can be defined, and a frame can be drawn or not.
//touch events are dispatched to childs.

typedef struct {
	void(*draw)(gui_element_t * self, uint8_t redraw);
	void(*set_on_screen)(gui_element_t *self, uint8_t state);
	void(*touch_handler)(gui_element_t *self, touch_event_t t);
	void(*update_position)(gui_element_t *self, int16_t x_diff, int16_t y_diff); //moves an element and all sub elements.
	void(*delete)(gui_element_t *self);
	rectangle_t box;//gui element sets height and width, container sets x and y.

	uint8_t on_screen;
	uint8_t orientation;//horizontal or vertical
	uint8_t frame_size;//how many pixels frame? 0x80+num draws bounding box
		
	list_t childs;
	uint16_t pos;//x or y position that we will place the next element in
} gui_container_t;

#define ORIENTATION_HORIZONTAL 0
#define ORIENTATION_VERTICAL 1

gui_container_t * new_gui_container();  //constructor
void gui_container_add (gui_container_t * c, gui_element_t * e);
void gui_container_delete_all_childs (gui_container_t * self);
void gui_container_draw (gui_element_t * self, uint8_t redraw);
void gui_container_set_on_screen (gui_element_t *self, uint8_t state);
void gui_container_touch_handler (gui_element_t *self, touch_event_t t);

////////////////////////////////////////////////////////////////////////////////


//a button//////////////////////////////////////////////////////////////////////
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
} gui_button_t;

void gui_button_draw (gui_element_t * self, uint8_t redraw);
void gui_button_set_on_screen (gui_element_t *self, uint8_t state);
void gui_button_touch_handler (gui_element_t *self, touch_event_t t);
gui_button_t * new_gui_button(); //constructor

////////////////////////////////////////////////////////////////////////////////


#endif // _H_GUI
