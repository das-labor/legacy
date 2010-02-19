#ifndef _H_DC_COM
#define _H_DC_COM

#include "../include/dc_commands.h"
#include "../include/icon.h"
#include "rectangle.h"


void init_dc_com();

void g_draw_horizontal_line(unsigned short x, unsigned short y, unsigned short length);
void g_draw_vertical_line(unsigned short x, unsigned short y, unsigned short length);
void g_draw_line(unsigned short x, unsigned short y, unsigned short x1, unsigned short y1);
 
void g_draw_char(char c) ;
void g_draw_string(unsigned short x, unsigned short y, const char *str);


void g_set_draw_color(uint8_t color);
void g_clear_screen();

void g_draw_rectangle(rectangle_t *r) ;
void g_fill_rectangle(rectangle_t *r);
void g_draw_string_in_rect(rectangle_t *r, const char *str);
void g_draw_string_in_rect_vert(rectangle_t *r, const char *str);


void g_draw_cross(uint16_t x, uint16_t y);

void g_draw_icon(uint16_t x, uint16_t y, icon_t * i);

#define PIXEL_ON 1
#define PIXEL_OFF 0

#endif // _H_DC_COM

