//
// This file is part of the gui-library for microcontrollers
// by Peter Fuhrmann.
//

#ifdef AVR
	#include <stdint.h>
	#include <util/delay.h>
#else
	#include <stdint.h>
#endif

#include "gui.h"
//#include "touchscreen.h"
//#include "dc_com.h"
//#include "can/can.h"
//#include "can/lap.h"

#define TYPE_BUTTON 1
#define TYPE_SLIDER 2

/*
 * Quick hack to enable light switching!
 * Better use message queue or something alike to schedule can packets...
 */
void self_destruct(void* self, uint16_t x, uint16_t y);
void self_slider(void* self, uint16_t x, uint16_t y);

/*
typedef struct {
	uint8_t type;
	uint8_t redraw;
	void(*touched)(void* self, uint16_t x, uint16_t y);

	//void * self;
	//void(*touched)(void* self, uint16_t x, uint16_t y);
	//void(*draw)(void* self, uint16_t x, uint16_t y);
} menu_item_t;


typedef struct {
	menu_item_t *item;
	uint16_t x;
	uint16_t y;
} menu_entry_t;

typedef struct {
	uint8_t type;
	uint8_t redraw;
	void(*touched)(void* self, uint16_t x, uint16_t y);
	char *text;
	uint8_t state;
} button_t;

typedef struct {
	uint8_t type;
	uint8_t redraw;
	void(*touched)(void* self, uint16_t x, uint16_t y);
	char *text;
	uint8_t value;
} slider_t;


slider_t test_slider1 = {
	TYPE_SLIDER,
	0,
	self_slider,
	"Licht Raum 5",
	30
};


button_t test_button1 = {
	TYPE_BUTTON,
	0,
	0,
	"Licht Raum 5",
	0
};

button_t test_button2={
	TYPE_BUTTON,
	0,
	0,
	"Licht Raum 2",
	0
};

button_t test_button3={
	TYPE_BUTTON,
	0,
	0,
	"Protonenbeschleuniger",
	0
};

button_t test_button4={
	TYPE_BUTTON,
	0,
	0,
	"Reaktor Dampf",
	0
};

button_t test_button5={
	TYPE_BUTTON,
	0,
	self_destruct,
	"Selbstzerstörung",
	0
};

button_t test_button6={
	TYPE_BUTTON,
	0,
	0,
	"Test Button",
	0
};

button_t test_button7={
	TYPE_BUTTON,
	0,
	0,
	"foo",
	0
};

button_t test_button8={
	TYPE_BUTTON,
	0,
	0,
	"Sinnlos",
	0
};


menu_item_t * main_menu[] = {
	(menu_item_t*)&test_button1,
	(menu_item_t*)&test_button2,
	(menu_item_t*)&test_button3,
	(menu_item_t*)&test_button4,
	(menu_item_t*)&test_button5,
	(menu_item_t*)&test_button6,
	(menu_item_t*)&test_slider1,
	(menu_item_t*)&test_button7,
	(menu_item_t*)&test_button8,
	(menu_item_t*)&test_button1,
	(menu_item_t*)&test_button1,
	(menu_item_t*)&test_button1,
	(menu_item_t*)&test_button1,
	(menu_item_t*)&test_button1,
	0
};


void self_destruct(void* self, uint16_t x, uint16_t y) {
	static can_message msg = {0, 0, 1, 1, 4, {0,0,0,0}};

	msg.addr_src = 0xd0;
	msg.addr_dst = 0x02;
	msg.data[0] = 0;
	msg.data[1] = 2;
	msg.data[2] = ((button_t *)self)->state;
	msg.data[3] = 0;
	can_transmit(&msg);
}


void self_slider(void* self, uint16_t x, uint16_t y) {
	static can_message msg = {0, 0, 1, 1, 4, {0,0,0,0}};
	uint16_t v = ((slider_t *)self)->value;
	v *= 255;
	v /= 112;

	msg.addr_src = 0xd0;
	msg.addr_dst = 0x02;
	msg.data[0] = 2;
	msg.data[1] = 2;
	msg.data[2] = 0;
	msg.data[3] = (uint8_t)(v&0xff);
	can_transmit(&msg);
}

void draw_slider(slider_t *self, uint16_t x, uint16_t y) {
	rectangle_t r = {x, y, 60, 116};
	g_set_draw_color(PIXEL_ON);
	g_draw_rectangle(&r);

	uint8_t wert = self->value;

	rectangle_t b = {x + 10, y + (115 - wert), 38, wert};
	g_fill_rectangle(&b);

	g_draw_string_in_rect_vert(&r, self->text);

	b.y = y + 1;
	b.h = (114 - wert);

	if (b.h > 114)
		b.h = 0;

	g_set_draw_color(PIXEL_OFF);
	g_fill_rectangle(&b);

	g_set_draw_color(PIXEL_OFF);

	r.w = 12;
}


void draw_button(button_t *self, uint16_t x, uint16_t y) {
	rectangle_t r = { x, y, 60, 56};

	if (self->state) {
		g_set_draw_color(PIXEL_ON);
		g_fill_rectangle(&r);
		g_set_draw_color(PIXEL_OFF);
		g_draw_string_in_rect(&r, self->text);
	} else {
		g_set_draw_color(PIXEL_OFF);
		g_fill_rectangle(&r);
		g_set_draw_color(PIXEL_ON);
		g_draw_rectangle(&r);
		g_draw_string_in_rect(&r, self->text);
	}
}


void button_touched(button_t * self, uint8_t click) {
	if (click) {
		self->state ^= 1;
		self->redraw = 1;
		if (self->touched != 0) {
			self->touched(self, click, 0);
		}
		_delay_ms(30);
	}
}


void slider_touched(slider_t * self, uint16_t x, uint16_t y, uint8_t click) {
	uint8_t oldvalue = self->value;

	if (y > 112) y = 112;
	self->value = 112 - y;

	if (self->value != oldvalue) {
		self->redraw = 1;

		if (self->touched != 0) {
			self->touched(self, x, y);
		}
	}
}


menu_entry_t akt_menu[10];


void setup_menu(menu_item_t * m[]) {
	uint8_t i, j = 0;
	for (i = 0; i < 8; i++) {
		menu_item_t * item = m[j];
		akt_menu[i].item = item;
		if (m[j] == 0)
			break;
		if (item->type == TYPE_SLIDER) {
			if ((i % 2) == 1) {
				 continue;
			} else {
				akt_menu[i].y = (i % 2) * 60 + 10;
				akt_menu[i].x = (i / 2) * 64 + 16;
				akt_menu[i + 1].item = item;
				akt_menu[i + 1].y = (i % 2) * 60 + 10;
				akt_menu[i + 1].x = (i / 2) * 64 + 16;
				i++;
				j++;
			}
		} else {
			akt_menu[i].y = (i % 2) * 60 + 10;
			akt_menu[i].x = (i / 2) * 64 + 16;
			j++;
		}
	}
}


void draw_menu(uint8_t force_draw) {
	uint8_t i;
	for (i = 0; i < 8; i++) {
		menu_item_t * item = akt_menu[i].item;
		if (item == 0)
			break;
		if (force_draw || item->redraw) {
			item->redraw = 0;
			switch (item->type) {
				case TYPE_BUTTON:
					draw_button(item, akt_menu[i].x, akt_menu[i].y);
					break;

				case TYPE_SLIDER:
					if ((i % 2) == 0) draw_slider(item, akt_menu[i].x, akt_menu[i].y);
					break;
			}
		}
	}
}


void menu_handle_touch(uint16_t x, uint16_t y, uint8_t click) {
	uint8_t i;
	for (i = 0; i < 8; i++) {
		uint16_t start_x = (i / 2) * 64 + 16;
		uint16_t end_x   = (i / 2) * 64 + 16 + 60;
		uint16_t start_y = (i % 2) * 64 + 10;
		uint16_t end_y   = (i % 2) * 64 + 10 + 60;

		menu_item_t * item = akt_menu[i].item;

		if ((x >= start_x && x <= end_x)
		    && (y >= start_y && y <= end_y)) {
			x -= akt_menu[i].x;
			y -= akt_menu[i].y;

			switch (item->type) {
				case TYPE_BUTTON:
					button_touched(akt_menu[i].item, click);
					break;
				case TYPE_SLIDER:
					slider_touched(akt_menu[i].item, x, y, click);
					break;
			}
		}
	}
}


void menu_test() {
	setup_menu(main_menu);
	draw_menu(1);
}

*/

