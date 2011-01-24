
#ifdef AVR
	#include <avr/io.h>
	#include <util/delay.h>
	#include <stdlib.h>
#else
	#include <stdint.h>
#endif

#include "../touchscreen.h"
#include "../gui_lib/graphics.h"

#include "../config.h"


calibration_values_t calibration_values;

pixel read_touch_raw() {
	return (pixel){-1,-1};
}

extern pixel touch_state;

pixel read_touch_screen_coordinates() {
	return touch_state;
	
	
	
	pixel p;
	p = read_touch_raw();


	if (p.x == -1) {
		return p;
	}

	if (p.x < calibration_values.xz) {
		p.x = 0;
	} else {
		p.x = ((uint32_t)(p.x-calibration_values.xz) * calibration_values.xg) / 1024;
	}

	if (p.y < calibration_values.yz) {
		p.y = 0;
	} else {
		p.y = ((uint32_t)(p.y-calibration_values.yz) * calibration_values.yg) / 1024;
	}
	return p;
}

