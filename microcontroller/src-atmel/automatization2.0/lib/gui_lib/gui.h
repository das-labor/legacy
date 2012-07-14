//
// This file is part of the gui-library for microcontrollers 
// by Peter Fuhrmann.
//

// Main gui header file - it contains the api.

#ifndef _H_GUI
#define _H_GUI

#include <stdint.h>
#include "rectangle.h"
#include "../util_lib/list.h"
#include "graphics.h"



//needed for touchscreen ///////////////////////////////////////////////////////
#define TOUCH_FLAG_DOWN    0x01
#define TOUCH_FLAG_UP      0x02
#define TOUCH_FLAG_LEAVE   0x04

typedef struct {
	uint16_t x;
	uint16_t y;
	uint8_t flags;
	//uint8_t click; //replaced by flags
} touch_event_t;
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

//handler types for conevnience ////////////////////////////////////////////////

typedef void(*touch_handler_t)(gui_element_t *self, touch_event_t t);
typedef void(*click_handler_t)(gui_element_t *);
////////////////////////////////////////////////////////////////////////////////

//the innermost touchhandler saves itself here so we can implement mouse up
//and leave events
gui_element_t * last_touched_gui_element;


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
void gui_container_init(gui_container_t * c);
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

	uint8_t frame_size;//how many pixels frame? 0x80+num draws bounding box
	char * text;
	icon_t * icon;
	uint8_t state;
	uint8_t toggle_mode;

	void(*click_handler)(gui_element_t *);
} gui_button_t;

void gui_button_draw (gui_element_t * self, uint8_t redraw);
void gui_button_set_on_screen (gui_element_t *self, uint8_t state);
void gui_button_touch_handler (gui_element_t *self, touch_event_t t);
void gui_button_update_position(gui_element_t * self, int16_t x_diff, int16_t y_diff);
gui_button_t * new_gui_button(); //constructor

////////////////////////////////////////////////////////////////////////////////


//a slider//////////////////////////////////////////////////////////////////////

typedef struct {
	void(*draw)(gui_element_t * self, uint8_t redraw);
	void(*set_on_screen)(gui_element_t *self, uint8_t state);
	void(*touch_handler)(gui_element_t *self, touch_event_t t);
	void(*update_position)(gui_element_t *self, int16_t x_diff, int16_t y_diff); //moves an element and all sub elements.
	void(*delete)(gui_element_t *self);
	rectangle_t box;//gui element sets height and width, container sets x and y.

	void(*value_changed)(gui_element_t * self, int16_t value);
	uint8_t orientation;//horizontal or vertical
	char * text;
	icon_t * icon;
	int16_t value;
	int16_t min_value;
	int16_t max_value;
	rectangle_t range_rectangle;
} gui_slider_t;

void gui_slider_draw (gui_element_t * self, uint8_t redraw);
void gui_slider_set_on_screen (gui_element_t *self, uint8_t state);
void gui_slider_touch_handler (gui_element_t *self, touch_event_t t);
void gui_slider_update_position(gui_element_t * self, int16_t x_diff, int16_t y_diff);
void gui_slider_set_value(gui_slider_t *self, int16_t val);
gui_slider_t * new_gui_slider(); //constructor

////////////////////////////////////////////////////////////////////////////////


//graph view////////////////////////////////////////////////////////////////////
typedef struct {
	void(*draw)(gui_element_t * self, uint8_t redraw);
	void(*set_on_screen)(gui_element_t *self, uint8_t state);
	void(*touch_handler)(gui_element_t *self, touch_event_t t);
	void(*update_position)(gui_element_t *self, int16_t x_diff, int16_t y_diff); //moves an element and all sub elements.
	void(*delete)(gui_element_t *self);
	rectangle_t box;//gui element sets height and width, container sets x and y.

	char * text;
	icon_t * icon;

	//graph data
	uint8_t idx;
	uint16_t data_size;
	uint16_t * data;
} gui_graph_view_t;

void gui_graph_view_draw (gui_element_t * self, uint8_t redraw);
void gui_graph_view_set_on_screen (gui_element_t *self, uint8_t state);
void gui_graph_view_touch_handler (gui_element_t *self, touch_event_t t);
void gui_graph_view_update_position(gui_element_t * self, int16_t x_diff, int16_t y_diff);
gui_graph_view_t * new_gui_graph_view(char * txt, uint8_t idx); //constructor

////////////////////////////////////////////////////////////////////////////////


#endif // _H_GUI
