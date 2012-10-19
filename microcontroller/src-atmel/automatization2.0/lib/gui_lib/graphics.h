#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "icon.h"
#include "rectangle.h"
#include "fonts/font.h"

void g_draw_pixel(uint16_t x, uint16_t y);
void g_draw_horizontal_line(unsigned short x, unsigned short y, unsigned short length);
void g_draw_vertical_line(unsigned short x, unsigned short y, unsigned short length);
void g_draw_line(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1);
 
void g_draw_char(char c) ;
void g_draw_string(uint16_t x, uint16_t y, const char *str);


void g_set_draw_color(uint8_t color);
void g_set_draw_font(font *new_font);
void g_clear_screen();

void g_draw_rectangle(rectangle_t *r);
void g_fill_rectangle(rectangle_t *r);
void g_draw_string_in_rect(rectangle_t *r, const char *str);
void g_draw_string_in_rect_vert(rectangle_t *r, const char *str);

int g_get_last_text_height();

void g_draw_cross(uint16_t x, uint16_t y);

void g_draw_icon(uint16_t x, uint16_t y, icon_t *i);


#define PIXEL_ON 1
#define PIXEL_OFF 0

#endif // GRAPHICS_H
