#ifndef PIXEL_H
#define PIXEL_H

#define LINEBYTES (((NUM_COLS-1)/8)+1)

extern unsigned char shl_table[];
extern unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

typedef struct {
	unsigned char x;
	unsigned char y;
} pixel;


enum direction{
	up,
	right,
	down,
	left
};
#ifdef NDEBUG
	typedef unsigned char direction_t;
#else
	typedef enum direction direction_t;
#endif


enum pixelmode{
	clear,
	set
};
#ifdef NDEBUG
	typedef unsigned char pixelmode_t;
#else
	typedef enum pixelmode pixelmode_t;
#endif


typedef struct cursor{
	pixel pos;
	direction_t dir;
	pixelmode_t mode;
} cursor_t;

/****************************************************************************
 * Pixel routines
 */

unsigned char get_pixel(pixel p);

inline static pixel next_pixel(pixel pix, direction_t dir){
	static char const nDelta[] = {0, -1, 0, 1, 0};
	return (pixel){pix.x + nDelta[dir], pix.y + nDelta[dir + 1]};
}


inline static unsigned char get_next_pixel(pixel p, direction_t dir){
	return get_pixel(next_pixel(p, dir));
}


inline static direction_t direction_r(direction_t dir){
	return (dir + 1u) % 4u;
}


void clear_screen(unsigned char value);


void setpixel(pixel p, unsigned char value);
#define clearpixel(p) setpixel(p, 0);


void shift_pixmap_l();


static inline void set_cursor(cursor_t* cur, pixel p){
	cur->pos = p;
	setpixel(p, cur->mode ? 3 : 0);
}

#endif // PIXEL_H
