#ifndef PIXEL_H
#define PIXEL_H

#define LINEBYTES (((NUM_COLS-1)/8)+1)

extern unsigned char shl_table[];
extern unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];

typedef struct {
	unsigned char x;
	unsigned char y;
} pixel;


typedef enum {right,left,up,down} direction;
typedef struct {
	pixel pos;
	direction dir;
	enum{clear=0, set=1} mode;
} cursor;

/****************************************************************************
 * Pixel routines
 */
void clear_screen(unsigned char value);
void setpixel(pixel p, unsigned char value);
#define clearpixel(p) setpixel(p, 0);

// straight or non straight line from one point to the other
// value=brightness
void line(pixel p1, pixel p2 ,unsigned char value);

// filled_rectangle p1=upper right corner, w=width, h=height , 
// value=brightness
void filled_rectangle(pixel p1, unsigned char w, unsigned char h ,unsigned char value);




unsigned char get_pixel(pixel p);
unsigned char get_next_pixel(pixel p, direction dir);

pixel next_pixel(pixel pix, direction dir);

direction direction_r(direction dir);

void shift_pixmap_l();


void set_cursor(cursor* cur, pixel p);

#endif // PIXEL_H
