#ifndef _H_TOUCHSCREEN
#define _H_TOUCHSCREEN

typedef struct {
	int16_t x;
	int16_t y;
} pixel;

typedef struct {
	int16_t xz;
	int16_t xg;
	int16_t yz;
	int16_t yg;
} calibration_values_t;

extern calibration_values_t calibration_values;



pixel read_touch_raw();
pixel read_touch_screen_coordinates();
#endif // ifndef _H_TOUCHSCREEN
