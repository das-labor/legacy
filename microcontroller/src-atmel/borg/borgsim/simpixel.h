typedef struct{
	unsigned char x;
	unsigned char y;
}pixel;
void setpixel(pixel p, unsigned char value );
typedef enum{right,left,up,down} direction;
unsigned char get_pixel(pixel p);
