#ifndef PIXEL_C
extern unsigned char shl_table[];
#endif


#define NUMPLANE 3

#ifndef MAIN_C
extern unsigned char pixmap[NUMPLANE][6];
#endif

typedef struct {
	unsigned char x;
	unsigned char y;
	unsigned char z;
}pixel3d;


//typedef enum{right,left,up,down} direction;
typedef enum {right, left, forward, back, up, down} direction;

void clear_screen(unsigned char value);

void setpixel3d(pixel3d p, unsigned char value);
void clearpixel3d(pixel3d p);

unsigned char get_pixel3d(pixel3d p);

unsigned char get_next_pixel3d(pixel3d p, direction dir);

pixel3d next_pixel3d(pixel3d pix, direction dir);

direction direction_r(direction dir);
