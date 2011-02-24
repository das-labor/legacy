#ifndef PIXEL_H
#define PIXEL_H

#define LINEBYTES (((NUM_COLS-1)/8)+1)

extern unsigned char shl_table[];
extern unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

typedef struct {
	unsigned char x;
	unsigned char y;
} pixel;


typedef enum {up, right, down, left} direction;
typedef struct {
	pixel pos;
	direction dir;
	enum{clear=0, set=1} mode;
} cursor;

/****************************************************************************
 * Pixel routines
 */

unsigned char get_pixel(pixel p);

static inline pixel next_pixel(pixel pix, direction dir){
	static char const nDelta[] = {0, -1, 0, 1, 0};
	return (pixel){pix.x + nDelta[dir], pix.y + nDelta[dir + 1]};
}


static inline unsigned char get_next_pixel(pixel p, direction dir){
	return get_pixel(next_pixel(p, dir));
}


static inline direction direction_r(direction dir){
	return (dir + 1) % 4;
}


void clear_screen(unsigned char value);


void setpixel(pixel p, unsigned char value);
#define clearpixel(p) setpixel(p, 0);


void shift_pixmap_l();


static inline void set_cursor(cursor* cur, pixel p){
	cur->pos = p;
	setpixel(p, cur->mode ? 3 : 0);
}

#endif // PIXEL_H
