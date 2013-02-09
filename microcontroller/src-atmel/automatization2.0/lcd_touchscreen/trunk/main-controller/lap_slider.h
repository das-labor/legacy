
typedef struct {
	void(*draw)(gui_element_t *self, uint8_t redraw);
	void(*set_on_screen)(gui_element_t *self, uint8_t state);
	void(*touch_handler)(gui_element_t *self, touch_event_t t);
	void(*update_position)(gui_element_t *self, int16_t x_diff, int16_t y_diff); //moves an element and all sub elements.
	void(*delete)(gui_element_t *self);
	rectangle_t box;//gui element sets height and width, container sets x and y.

	void(*value_changed)(gui_element_t *self, int16_t value);
	uint8_t orientation;//horizontal or vertical
	char *text;
	icon_t *icon;
	int16_t value;
	int16_t min_value;
	int16_t max_value;
	rectangle_t range_rectangle;
	// -------------------- inherited from gui_slider_t until here -------------
	
	netvar_desc *nv;
} lap_slider_t;

lap_slider_t *new_lap_slider(uint16_t idx, uint8_t sidx);
