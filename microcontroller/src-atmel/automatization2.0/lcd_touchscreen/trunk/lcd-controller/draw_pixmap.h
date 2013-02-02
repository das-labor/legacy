#ifndef _H_DRAW_PIXMAP
#define _H_DRAW_PIXMAP

#include "gui_lib/icon.h"

void g_clear_screen(void);
void lcd_graphics_plot_pixel(unsigned short x, unsigned short y, unsigned char state);
void g_clear(void);
void g_copy_logo(void * logo, uint8_t x_bytes, uint8_t y_size, uint16_t yc, uint16_t xc );
void g_draw_icon(uint16_t x, uint16_t y, icon_t * i);

#endif // _H_DRAW_PIXMAP

