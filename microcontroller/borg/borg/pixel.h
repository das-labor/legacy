#ifndef PIXEL_C
extern unsigned char shl_table[];
#endif

#define NUMPLANE 3
unsigned char pixmap[NUMPLANE][8];

typedef struct{
	unsigned char x;
	unsigned char y;
}pixel;


typedef enum{right,left,up,down} direction;

typedef struct{
	pixel pos;
	direction dir;
	enum{clear=0, set=1} mode;
}cursor;




void clear_screen(unsigned char value);

void setpixel(pixel p, unsigned char value);
void clearpixel(pixel p);
unsigned char get_pixel(pixel p);
unsigned char get_next_pixel(pixel p, direction dir);

pixel next_pixel(pixel pix, direction dir);

direction direction_r(direction dir);



void set_cursor(cursor* cur, pixel p);
void walk(cursor* cur, unsigned char steps, unsigned int delay);
