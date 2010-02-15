/* 
 * Basic Raster Graphics interface to the lcd_graphics_* LCD graphics driver 
 * functions.
 * Vanya A. Sergeev - <vsergeev@gmail.com>
 *
 * font5x7.h header file comes from Procyon AVRlib.
 */

#ifndef _H_GRAPHICS
#define _H_GRAPHICS

#include "fonts/font.h"

void g_draw_horizontal_line(unsigned short x, unsigned short y, unsigned short length);
void g_draw_vertical_line(unsigned short x, unsigned short y, unsigned short length);
void g_draw_line(unsigned short x, unsigned short y, unsigned short x1, unsigned short y1);
 
void g_draw_char( char c) ;
void g_draw_string(unsigned short x, unsigned short y, const char *str);

//color in which new pixels are drawn
extern uint8_t draw_color;

extern font * draw_font; 

typedef struct{
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
}rectangle_t;


void g_draw_rectangle(rectangle_t * r) ;

void g_draw_string_in_rect(rectangle_t * r, const char *str);
void g_draw_string_in_rect_vert(rectangle_t * r, const char *str);

void g_fill_rectangle(rectangle_t * r);

void g_draw_cross(uint16_t x, uint16_t y);

void g_fill_circle(uint16_t x, uint16_t y, uint8_t r);

#define PIXEL_ON 1
#define PIXEL_OFF 0

#endif //_H_GRAPHICS

