#include <stdint.h>

#include "gui_lib/graphics.h"
#include "draw_pixmap.h"
#include "util/delay.h"
#include "config.h"



typedef struct {
	uint8_t *pattern;
	uint8_t anim_step;
	uint8_t x;
	uint8_t y;
} eye_t;


static uint16_t hypnotoad_x = 40;
static uint16_t hypnotoad_y = 0;


void clear_eye(eye_t *e) {
	draw_color = 0;
	g_fill_circle(e->x + hypnotoad_x, e->y + hypnotoad_y, 6);
}

void animate_eye(eye_t *e) {
	draw_color = 1;

	switch (e->pattern[e->anim_step]) {
		case 1:
			e->y++;
		case 0:
			e->x++;
			break;
		case 3:
			e->x--;
		case 2:
			e->y++;
			break;
		case 5:
			e-> y--;
		case 4:
			e->x--;
			break;
		case 7:
			e->x++;
		case 6:
			e->y--;
			break;
	}
	e->anim_step ++;
	if (e->pattern[e->anim_step] == 8)
		e->anim_step = 0;
	g_fill_circle(e->x + hypnotoad_x, e->y + hypnotoad_y, 6);
}


uint8_t pattern1[] = {2,2,2,6,6,6,1,1,3,3,6,6,6,6,8};
uint8_t pattern2[] = {2,2,2,2,6,6,6,6,6,3,3,3,1,1,6,7,6,6,8};

eye_t eye1 = {pattern1, 0, 37, 23};
eye_t eye2 = {pattern1, 4, 40, 24};
eye_t eye3 = {pattern2, 7, 46, 24};


eye_t eye4 = {pattern2, 0, 96, 22};
eye_t eye5 = {pattern1, 1, 99, 23};
eye_t eye6 = {pattern2, 3, 104, 23};


extern uint8_t hypnotoad_data[];
extern volatile uint8_t display_line;

void hypnotoad() {
	g_copy_logo(hypnotoad_data, 30, 195, hypnotoad_x / 8, hypnotoad_y);

	while (1) {
		while (display_line != 40);
		clear_eye(&eye1);
		clear_eye(&eye2);
		clear_eye(&eye3);

		animate_eye(&eye1);
		animate_eye(&eye2);
		animate_eye(&eye3);

		while (display_line != 40);
		clear_eye(&eye4);
		clear_eye(&eye5);
		clear_eye(&eye6);

		animate_eye(&eye4);
		animate_eye(&eye5);
		animate_eye(&eye6);

		_delay_ms(100);

		if (TEST_ATN() == 0)
			break;

	}

	//g_fill_circle(100, 100, 5);

}

