#include <math.h>
#include <stdio.h>

#include "api.h"


#define PI 3.14159265358979323846
#define iPI 65535

double sin_table2[16384];


int sin_table[16384];

void init_sin_table(){
	int x;
	float f;
	for(x=0;x<16384;x++){
		f = x;
		sin_table[x] = sinf(f*2.0*PI/65535.0 ) * 32768;
	}
}

int sini(unsigned int w){
	unsigned int v;
	
	w %= 65536;//360° wrap
	if (w<0){
		w+=65536;
	}
	
	v = w%16384;
	
	switch(w/16384){
		case 0: return sin_table[v];
		case 1: return sin_table[16383-v];
		case 2: return -sin_table[v];
		case 3: return -sin_table[16383-v];
		default: return 0;
	}
}

void init_sin_table2(){
	int x;
	float f;
	for(x=0;x<16384;x++){
		f = x;
		sin_table2[x] = sinf(f*2.0*PI/65535.0 );
	}
}

double sinhack(double d){
	int w, v;

	w = d*(65535/(2*PI));
	w %= 65536;//360° wrap
	v = w%16384;
	switch(w/16384)
	{
		case 0: return sin_table2[v];
		case 1: return sin_table2[16383-v];
		case 2: return -sin_table2[v];
		case 3: return -sin_table2[16383-v];
		default: break;
	}
	
	return 0.0;
}


void plasma();

void *display_loop(void * unused)  
{
    // Initialize stuff
//	uart_init();

	// Say Hello!
	uart_putstr("Sin Table init\r\n");

	init_sin_table();
	init_sin_table2();
	
	uart_putstr("done calculating sin table\r\n");

	/*int x;
	
	for(x=0;x<64;x++){
		
	}
	*/
	
	// Initialize TIC
//	isr_init();
//	tic_init();
//	irq_set_mask( 0x00000002 );
//	irq_enable();

	initPwm();
	

	plasma();
	
/*	
	uint8_t c, oldc;
	int i;
	while(1){
		//wait for sync
		while(1){
			oldc = c;
			c = uart_getchar();
			if((oldc == 0x23) && (c == 0x42))
				break;
		}
		for(i=0; i<(MAX_X*MAX_Y*COLOR_BYTES); i++){
			((uint8_t*) imag)[i] = uart_getchar();
		}
		swap();
	}
*/	
	return 0;
}



color HtoRGB(int h31bit)
{
/*
	while (h31bit < 0)
		h31bit += 49152;
	
	while (h31bit > 49152)
		h31bit -= 49152;
*/
	
	h31bit %= 49152;
	
	if(h31bit < 0){
		h31bit += 49152;
	}
	
	//printf("%d\n", h31bit);
	
	
	color rgb;

		unsigned char sextant;
	int           q;
	sextant  = h31bit / 8192;   // 60°

	h31bit     = h31bit % 8192;
	q          = 8192 - h31bit;	
	//h31bit     = 8192 - (8192 - h31bit);
	//printf("%d %d %d\n", h31bit, sextant, help);
	switch(sextant)
	{
	    case 0:
			rgb.r = 255;
			rgb.g = h31bit / 32; 
			rgb.b = 0;
			break;
	    case 1:
			rgb.r = q / 32;
			rgb.g = 255;
			rgb.b = 0;
			break;
	    case 2:
			rgb.r = 0;
			rgb.g = 255;
			rgb.b = h31bit / 32;
			break;
	    case 3:
			rgb.r = 0;
			rgb.g = q / 32;
			rgb.b = 255;
			break;
	    case 4:
			rgb.r = h31bit / 32;
			rgb.g = 0;
			rgb.b = 255;
			break;
	    default:
			rgb.r = 255;
			rgb.g = 0;
			rgb.b = q / 32;
			break;
	};
	return(rgb);
};

/*
void setPixelH(int x, int y, float h)
{
	setPixel((pixel) {x, y}, HtoRGB(h*49152));
}
*/

void setPixelH(int x, int y, int h)
{
	setPixel((pixel) {x, y}, HtoRGB(h));
}

#define WIDTH 16
#define HEIGHT 8
#define SQUARE(x) ((x)*(x))

//static term value caches
unsigned int term_PolarScale;
unsigned int term_XScale;
unsigned int term_YScale;
unsigned int term_PolarYDistSquare;
int term_YSine;

/*
void initTermCaches(unsigned int scale)
{
	term_PolarScale = (iPI / (scale * (sqrt(SQUARE(HEIGHT) + SQUARE(WIDTH)))));
	term_XScale = (iPI / (WIDTH * scale));
	term_YScale = (iPI / (HEIGHT * scale));
}
*/

/*
void updateYSine(unsigned int y, unsigned int offset)
{
	term_YSine = sini((unsigned int)(y * term_YScale) + offset);
}
*/

/*
void updatePolarYDistSquare(unsigned int y, int cy)
{
	term_PolarYDistSquare = SQUARE(y - (HEIGHT / 2) - cy);
}
*/

//polar plasma
int plasma_Polar(int x, int y, unsigned int offset, unsigned int scale, int cx, int cy)
{
	return sini( ((sqrt(SQUARE(x - (WIDTH*256/2) - cx) + SQUARE(y - (HEIGHT*256/2) - cy)) *scale) /4096  ) + offset);
}

//sum of x-sine and y-sine
int plasma_XYSum(unsigned int x, unsigned int y, unsigned int offset, unsigned int scale)
{
	return ( sini((unsigned int)(x )*scale/65536 + offset) + sini((unsigned int)(y )*scale/32768 + offset ) );
}

void plasma(){
	int x, y;
	int mx, my;
	
	//float cx, cy, offset;
	int cx, cy;
	int color, scale, XYSumOffset, PolarOffset, colOff;
	int cxOffset, cyOffset;
	
	
	//offset = 0.0;
	scale = 1;
//	initTermCaches(scale);

	PolarOffset = 0;
	XYSumOffset = 0;
	cxOffset = 0;
	cyOffset = 0;
	colOff = 0;

	while(1){

		XYSumOffset += 80;
		XYSumOffset %= 65536;
		
		cxOffset += 50;
		//cxOffset %= 65536;
		
		cyOffset += 59;
		cyOffset %= 65536;

		PolarOffset +=100;
		PolarOffset %=65536;
		
		//colOff += 250;
		colOff %= 49152;
	
		
		cx = (sini(cxOffset) * (WIDTH/2)) /128;
		cy = (sini(cyOffset) * (HEIGHT/2)) /128;
			
		
		/*
		for(x=0;x<256;x++){
			printf("%d\n", sini(x*(65536/64)));
		
		}
		
		while(1);
		*/
				
		
		for(y=0;y<8;y++){
			for(x=0;x<16;x++){
				mx = x*256;
				my = y*256;
				
				//sum of hansi-tuned XYSum plasma and peter-tuned polar plasma
				color = 0;
				
				color += plasma_XYSum(mx, my, XYSumOffset, 32768);			
				color += plasma_Polar(mx, my, PolarOffset, 10000, cx, cy);
				color /= 2;
				//color += 32768;
				
				color += color/2;
				
				setPixelH(x,y, color+colOff);
			}
		}
		//setPixel((pixel) {0,0}, white);
		swap();
		myWait(30);
	}
}

