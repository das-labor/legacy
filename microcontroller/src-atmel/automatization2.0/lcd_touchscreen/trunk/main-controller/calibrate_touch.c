#include <stdint.h>
#include <stdio.h>
#include <avr/eeprom.h>

#include "calibrate_touch.h"
#include "touchscreen.h"
#include "dc_com.h"

#define EEPROM_TOUCHCAL_DATA_OFFSET 1

pixel read_mean() {
	pixel p;
	uint16_t mean_x = 0;
	uint16_t mean_y = 0;

	uint16_t bar = 0;
	rectangle_t progress_bar = {40, 100, 160, 40};

	g_draw_rectangle(&progress_bar);

	while (1) {
		p = read_touch_raw();
		if (p.x != -1) {
			mean_x += p.x;
			mean_y += p.y;
			bar += 10;
			progress_bar.w = bar;
			g_fill_rectangle(&progress_bar);

			if (bar == 160) break;
		} else {
			mean_x = 0;
			mean_y = 0;
			if (bar != 0) {
				bar = 0;
				progress_bar.w = 160;
				g_set_draw_color(0);
				g_fill_rectangle(&progress_bar);
				g_set_draw_color(1);
				g_draw_rectangle(&progress_bar);
			}
		}
	}

	pixel rp = (pixel){mean_x / 16, mean_y / 16};

	do {
		p = read_touch_raw();
		
	} while (p.x != -1);

	progress_bar.w = 160;
	g_set_draw_color(0);
	g_fill_rectangle(&progress_bar);

	return rp;
}



#define NUM_CALIB_POINTS 4

pixel calib_points[] = {
	{10       , 10},
	{320 - 10 , 10},
	{10       , 240 - 10},
	{320 - 10 , 240 - 10}
};

rectangle_t text_boxes[] = {
	{5  , 15      , 100, 20},
	{220, 15      , 100, 20},
	{5  , 240 - 30, 100, 20},
	{220, 240 - 30, 100, 20}
};


pixel calibrate_point(uint8_t i) {
	rectangle_t box;
	char textbuf[100];
	box.x = calib_points[i].x - 5;
	box.y = calib_points[i].y - 5;
	box.w = 10;
	box.h = 10;


	g_draw_rectangle(&box);
	pixel p = read_mean();
	g_set_draw_color(0);
	g_draw_rectangle(&box);
	g_set_draw_color(1);
	sprintf(textbuf, "X:%d Y:%d", p.x, p.y);
	g_draw_string_in_rect(&text_boxes[i], textbuf);

	return p;
}


void test_touchscreen() {
	pixel p1 = {0, 0};
	pixel p;

	while (1) {
		p = read_touch_screen_coordinates();

		//mouse cursor
		if (((p1.x != p.x) || (p1.y != p.y ))) {
			g_set_draw_color(0);
			//g_draw_cross(p1.x , p1.y);
		}
		
		if (p.x != -1) {
			g_set_draw_color(1);
			g_draw_cross(p.x , p.y);
		}


		if ((p1.x == -1 )&& (p.x != -1)) {
			g_clear_screen();

			g_draw_rectangle(&(rectangle_t) {10, 10, 50, 20});
			g_draw_string_in_rect(&(rectangle_t) {10, 10, 50, 20}, "Exit");

			if (p.x < 60 && p.x > 10 && p.y < 30 && p.y > 10) {
				break;
			}
		}

	#ifdef OSZI
	//Oszi
		static uint16_t xc = 0;

		if (p.x != -1 )
			g_draw_cross(xc, p.x);

		xc++;
		if (xc == 320) {
			xc = 0;
		}
	#endif

		p1 = p;
	}
}


void draw_crosses() {
	uint8_t i;
	for (i = 0; i < NUM_CALIB_POINTS; i++) {
		g_draw_cross(calib_points[i].x, calib_points[i].y);
	}
}


void calibrate_touch() {
	//g_clear_screen();

	draw_crosses();

	pixel p[NUM_CALIB_POINTS];

	uint8_t i;
	for (i = 0; i < NUM_CALIB_POINTS; i++) {
		p[i] = calibrate_point(i);
	}

	uint16_t xl,xh,yl,yh;
	xl = p[0].x + p[2].x;
	xh = p[1].x + p[3].x;
	yl = p[0].y + p[1].y;
	yh = p[2].y + p[3].y;

	uint16_t xd = xh-xl;
	uint16_t xg = 300ul * 1024ul * 2ul / xd;
	uint16_t xz = (xl - xd / 30) / 2 ;

	uint16_t yd = yh - yl;
	uint16_t yg = 220ul * 1024ul * 2ul / yd;
	uint16_t yz = (yl - yd / 22) / 2 ;

	calibration_values.xz = xz;
	calibration_values.xg = xg;
	calibration_values.yz = yz;
	calibration_values.yg = yg;
	
	eeprom_write_block(&calibration_values,
			  (void *) EEPROM_TOUCHCAL_DATA_OFFSET,
		           sizeof(calibration_values_t));
	
	char textbuf[100];
	sprintf(textbuf,"xg:%d  xz:%d", xg, xz);
	g_draw_string_in_rect(&(rectangle_t) {200, 200, 100, 100}, textbuf);

	sprintf(textbuf,"yg:%d  yz:%d", yg, yz);
	g_draw_string_in_rect(&(rectangle_t) {200, 180, 100, 100}, textbuf);


/*
	g_clear_screen();
	g_draw_rectangle(&((rectangle_t){5,5,10,10}));
	g_draw_cross(p.x,p.y);
*/

	//test_touchscreen();
}

uint8_t read_calibration_data_from_eeprom() {
	eeprom_read_block(&calibration_values,
			  (void *)EEPROM_TOUCHCAL_DATA_OFFSET, 
			  sizeof(calibration_values_t));
	if (calibration_values.xz == 0xffff && calibration_values.xg == 0xffff
	     && calibration_values.yz == 0xffff && calibration_values.yg == 0xffff)
		return 1;
	else
		return 0;
}

