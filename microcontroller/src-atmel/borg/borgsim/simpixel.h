typedef struct{
	unsigned char x;
	unsigned char y;
}pixel;
void setpixel(pixel p, unsigned char value );
typedef enum{right,left,up,down} direction;
unsigned char get_pixel(pixel p);
pixel next_pixel(pixel pix, direction dir);
direction direction_r(direction dir);
