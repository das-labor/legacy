
#include <stdint.h>
#include "graphics.h"
#include "display.h"
#include "fonts/font_uni53.h"

//color in which new pixels are drawn
static uint8_t draw_color;

static font * draw_font = & font_uni53;

#ifdef AVR
	#define PW(a) pgm_read_word(&(a))
	#define PB(a) pgm_read_byte(&(a))
#else
	#define PW(a) ((uint16_t)a)
	#define PB(a) ((uint8_t)a)
#endif


void g_set_draw_color(uint8_t c){
	draw_color = c;
}

void g_set_draw_font(font * f){
	draw_font = f;
}

void g_draw_pixel(uint16_t x, uint16_t y){
	display_set_pixel(x, y, draw_color);
}

void g_clear_screen(){
	display_clear_screen();
}

/**
 * Draws a horizontal line, left to right, at the specified coordinates and of
 * the specified length.
 * @param x The x coordinante of the line's origin.
 * @param y The y coordinante of the line's origin.
 * @param length The length of the line, in pixels.
 */

void g_draw_horizontal_line(unsigned short x, unsigned short y, unsigned short length) {
	unsigned short i;
	if(length == 0) return;
	unsigned short x_max = x+length-1;
	for (i = x; i <= x_max; i++)
		g_draw_pixel(i, y);
}


/**
 * Draws a vertical line, top to bottom, at the specified coordinates and of
 * the specified length.
 * @param x The x coordinante of the line's origin.
 * @param y The y coordinante of the line's origin.
 * @param length The length of the line, in pixels.
 */

void g_draw_vertical_line(unsigned short x, unsigned short y, unsigned short length) {
	unsigned short i;
	for (i = y; i < y+length; i++)
		g_draw_pixel(x, i);
}


void g_draw_line(unsigned short x, unsigned short y, unsigned short x1, unsigned short y1){
	int dx, dy, stepx, stepy, fraction;

        dy = y1 - y;
    	dx = x1 - x;

    if ( dy < 0 )
    {
        dy    = -dy;
        stepy = -1;
    }
    else
    {
        stepy = 1;
    }

    if ( dx < 0 )
    {
        dx    = -dx;
        stepx = -1;
    }
    else
    {
        stepx = 1;
    }

    dx <<= 1;
    dy <<= 1;

    g_draw_pixel(x, y);
    if ( dx > dy )
    {
        fraction = dy - (dx >> 1);
        while ( x != x1 )
        {
            if ( fraction >= 0 )
            {
                y += stepy;
                fraction -= dx;
            }
            x += stepx;
            fraction += dy;
            g_draw_pixel(x, y);
        }
    }
    else
    {
        fraction = dx - (dy >> 1);
        while ( y != y1 )
        {
            if ( fraction >= 0 )
            {
                x += stepx;
                fraction -= dy;
            }
            y += stepy;
            fraction += dx;
            g_draw_pixel(x, y);

        }
    }
}


/**
 * Draws a rectangle. The coordinantes specify the top left corner of the
 * rectangle.
 * @param x The x coordinante of the rectangle's origin.
 * @param y The y coordinante of the rectangle's origin.
 * @param width The width of the rectangle, in pixels.
 * @param height The height of the rectangle, in pixels.
 */
void g_draw_rectangle(rectangle_t * r) {
	uint16_t width  = r->w;
	uint16_t height = r->h;
	uint16_t x = r->x;
	uint16_t y = r->y;

	g_draw_horizontal_line(x, y, width);
	g_draw_vertical_line(x, y, height);
	g_draw_vertical_line(x+width-1, y, height);
	g_draw_horizontal_line(x, y+height-1, width);
}


void g_fill_rectangle(rectangle_t * r) {
	uint16_t x = r->x;
	uint16_t y = r->y;
	uint16_t w = r->w;
	uint16_t h = r->h;

	while(h--){
		g_draw_horizontal_line(x, y, w);
		y++;
	}
}


uint16_t text_x, text_y;

void g_draw_char( char ch ) {
	uint16_t s, e;
	uint8_t h, i, d=0, bytes, b;
	uint8_t c = ch;

	if(c < draw_font->glyph_beg || c > draw_font->glyph_end ) return;
	c -= draw_font->glyph_beg;

	s = PW(draw_font->fontIndex[c]);
	e = PW(draw_font->fontIndex[c+1]);
	bytes =  draw_font->storebytes;

	h = draw_font->fontHeight;

	for(;s < e; s += bytes){
		i=0; b=0;
		do{
			if((i%8) == 0)
				d = PB(draw_font->fontData[s + b++]);
			if(d & 0x01){
				g_draw_pixel(text_x, text_y+i);
			}
			d >>= 1;
			i++;
		}while(i<h);

		text_x ++;
	}
	text_x ++;
}

void g_draw_char_vert( char ch) {
	uint16_t s, e;
	uint8_t h, i, d, bytes;
	uint8_t c = ch;

	if(c < draw_font->glyph_beg || c > draw_font->glyph_end ) return;
	c -= draw_font->glyph_beg;

	s = PW(draw_font->fontIndex[c]);
	e = PW(draw_font->fontIndex[c+1]);
	bytes =  draw_font->storebytes;

	h = draw_font->fontHeight;

	for(;s < e; s += bytes){
		d = PB(draw_font->fontData[s]);
		for(i=0;i<8;i++){
			if(d & 0x01){
				lcd_graphics_plot_pixel(text_x+i, text_y, draw_color);
			}
			d >>= 1;
		}
		if(bytes > 1){
			d = PB(draw_font->fontData[s+1]);
			for(i=8;i<h;i++){
				if(d & 0x01){
					lcd_graphics_plot_pixel(text_x+i, text_y, draw_color);
				}
				d >>= 1;
			}
		}
		text_y --;
	}
	text_y --;
}


uint8_t char_width( char ch) {
	uint16_t s, e;
	uint8_t bytes;
	uint8_t c = ch;

	if(c < draw_font->glyph_beg || c > draw_font->glyph_end ) return 0;
	c -= draw_font->glyph_beg;

	s = PW(draw_font->fontIndex[c]);
	e = PW(draw_font->fontIndex[c+1]);
	e -= s;
	bytes =  draw_font->storebytes;

	if(bytes > 1){
		e /= 2 ;
	}

	return e;
}

int16_t get_string_width(const char * str){
	char c;
	int16_t width = 0;
	while((c=*str++) != 0){
		width += char_width(c) + 1;
	}
	return width;
}


int16_t text_height;

void g_draw_string_in_rect(rectangle_t * r, const char *str){
	int16_t start_x = r->x + 2;
	int16_t right_x = r->x + r->w - 2;
	int16_t bottom_y = r->y + r->h - 2;
	char c;

	text_y = r->y + 2;
	int16_t text_begin_y = text_y;

	text_x = start_x;

	do{
		if(text_y + draw_font->fontHeight > bottom_y){
			break;
		}

		if(*str == '\b'){//center
			str++;
			int16_t swidth = get_string_width(str);
			int16_t rwidth = r->w - 4;
			text_x += (rwidth - swidth)/2;
		}

		while((c = *str)){
			if( (text_x + char_width(c) ) > right_x){
				break;
			}
			if(c == '\n'){
				str++;
				break;
			}
			g_draw_char(c);
			str++;
		}
		text_y += draw_font->fontHeight;
		text_x = start_x;
	}while(c);

	text_height = text_y - text_begin_y;
}

int g_get_last_text_height(){
	return text_height;
}


void g_draw_string_in_rect_vert(rectangle_t * r, const char *str){
	int16_t start_y = r->y + r->h - 2;
	int16_t right_y = r->y + 2;
	int16_t bottom_x = r->x + r->w - 2;
	char c;

	text_x = r->x + 2;

	do{
		if(text_x + draw_font->fontHeight > bottom_x){
			break;
		}
		text_y = start_y;
		while((c = *str)){
			if( (text_y - char_width(c) ) < right_y){
				break;
			}
			g_draw_char_vert(c);
			str++;
		}
		text_x += draw_font->fontHeight;
	}while(c);
}


void g_draw_string(unsigned short x, unsigned short y, const char *str) {
	char c;

	text_x = x;
	text_y = y;

	while((c = *str++)){
		g_draw_char(c);
	}

}

void g_draw_cross(uint16_t x, uint16_t y) {
	x -= 4;
	y -= 4;
	g_draw_line(x, y, x + 8, y + 8);
	g_draw_line(x, y + 8, x + 8, y);

//	lcd_graphics_plot_pixel(x, y, draw_color);
}

extern uint8_t draw_color;
void g_draw_icon(uint16_t x, uint16_t y, icon_t * i){
	uint16_t w;
	uint16_t h;

	uint16_t byte_cnt = 0;
	uint8_t msk;
	uint8_t d = 0;

	h = i->height;
	while(h--){
		uint16_t xi = x;
		msk = 0x00;
		w = i->width;
		while(w--){
			if(msk == 0x00){
				d = i->data[byte_cnt++];
				msk = 0x01;
			}
			if(d & msk){
				g_draw_pixel(xi,y);
			}
			xi++;
			msk <<= 1;
		}
		y++;
	}
}

