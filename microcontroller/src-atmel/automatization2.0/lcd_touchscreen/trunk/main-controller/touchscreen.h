

typedef struct {
	int16_t x;
	int16_t y;
} pixel;

typedef struct {
	uint16_t xz;
	uint16_t xg;
	uint16_t yz;
	uint16_t yg;
} calibration_values_t;

extern calibration_values_t calibration_values;



void handle_touchscreen();
pixel read_touch_raw();
pixel read_touch_screen_coordinates();
