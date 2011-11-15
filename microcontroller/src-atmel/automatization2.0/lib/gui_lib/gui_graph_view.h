//
// This file is part of the gui-library for microcontrollers 
// by Peter Fuhrmann.
//

// Main gui header file - it contains the api.

#ifndef _H_GUI_GRAPH_VIEW
#define _H_GUI_GRAPH_VIEW

void gui_graph_view_draw (gui_element_t * self, uint8_t redraw);
void gui_graph_view_set_on_screen (gui_element_t *self, uint8_t state);
void gui_graph_view_touch_handler (gui_element_t *self, touch_event_t t);
void gui_graph_view_delete (gui_element_t *self);
void gui_graph_view_update_position(gui_element_t * self, int16_t x_diff, int16_t y_diff);
gui_graph_view_t * new_gui_graph_view(char * txt, uint8_t idx);

#endif
