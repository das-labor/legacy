
#include <stdio.h>
#include <stdint.h>
#include <math.h>

FILE* port;

uint8_t pixmap[8][16][3];


void transmit(){
	fputc(0x23, port);
	fputc(0x42, port);

	fwrite(pixmap, 8, 48, port);
}


typedef struct
{
    //entweder H S V oder R G B
    double f[3];
}farbe;

farbe HSVtoRGB(farbe hsv)
{
    farbe rgb;
    if(hsv.f[1] == 0)   //keine Farbe, nur Grau
    {
		rgb.f[0] = hsv.f[2];
		rgb.f[1] = hsv.f[2];
		rgb.f[2] = hsv.f[2];
    }
    else                //Farbsaettigung vorhanden
    {
		unsigned short sextant;
		double         p,q;

		hsv.f[0] = hsv.f[0]/60.0;
		sextant  = (unsigned short) hsv.f[0];
		hsv.f[0] = hsv.f[0]-sextant;
		p        = hsv.f[2]*(1-hsv.f[1]);
		q        = hsv.f[2]*(1-(hsv.f[1]*hsv.f[0]));
		hsv.f[0] = hsv.f[2]*(1-(hsv.f[1]*(1-hsv.f[0])));

		switch(sextant)
		{
		    case 0:
			rgb.f[0] = hsv.f[2];
			rgb.f[1] = hsv.f[0];
			rgb.f[2] = p;
			break;
		    case 1:
			rgb.f[0] = q;
			rgb.f[1] = hsv.f[2];
			rgb.f[2] = p;
			break;
		    case 2:
			rgb.f[0] = p;
			rgb.f[1] = hsv.f[2];
			rgb.f[2] = hsv.f[0];
			break;
		    case 3:
			rgb.f[0] = p;
			rgb.f[1] = q;
			rgb.f[2] = hsv.f[2];
			break;
		    case 4:
			rgb.f[0] = hsv.f[0];
			rgb.f[1] = p;
			rgb.f[2] = hsv.f[2];
			break;
		    default:
			rgb.f[0] = hsv.f[2];
			rgb.f[1] = p;
			rgb.f[2] = q;
			break;
		};
    };
    return(rgb);
};

void setPixelHSV(int x, int y, farbe hsv){
	farbe rgb;
	rgb = HSVtoRGB(hsv);
	pixmap[y][x][0] = rgb.f[1] * 255.0 ;
	pixmap[y][x][1] = rgb.f[2] * 255.0 ;
	pixmap[y][x][2] = rgb.f[0] * 255.0 ;
}

void setPixelH(int x, int y, double h){
	farbe f;
	
	//cap to 1.0
	h -= floor(h);

	f.f[0] = h*360;
	f.f[1] = 1;
	f.f[2] = 1;

	setPixelHSV(x,y,f);
}

#define PI 3.14159265
#define WIDTH 16.0
#define HEIGHT 8.0
#define SQUARE(x) (x)*(x)

int main(){
	port = fopen("/dev/ttyUSB0", "wo");

	int x, y, x2, y2;

	float p1=0.1, p2=0.2;

	double fx, fy, color, scale, offset;

	scale = 6.0; offset = 0.0; fy = 0.0;

	double bla [16*4][8*4];
	
	for(y = 0; y < 8*4; y++)
	{
		for(x=0; x < 16*4; x++)
		{
			fx = x;
			fy = y;

			bla[x][y] = (((0.5 * sin(fx * (PI / (WIDTH * scale)))) + (0.5 * sin(fy * (PI / (HEIGHT * scale)))) + (0.5 * sin((fx * (PI / (WIDTH * scale))) + (fy * (PI / (HEIGHT * scale)))) + (0.5 * (color + sin(sqrt(SQUARE(fx - (WIDTH / 2.0)) + SQUARE(fy - (HEIGHT / 2.0))) * (PI / ((scale * 1.0) * (sqrt(SQUARE(HEIGHT) + SQUARE(WIDTH)))))))))) / 4.0);
		}
	}
	
	x2=0; y2=0;
	while(1){

		offset += 0.05;

		//sine scaling
		scale = 4.0 + (2.0 * sin(offset*0.1));
		
		for(y=0;y<8;y++){
			for(x=0;x<16;x++){
				fx = x;
				fy = y;
				//diagonal scrolling sine
				color = 0.5 + (sin((fx * (PI / (WIDTH * scale))) + (fy * (PI / (HEIGHT * scale)) + offset)));

				//polar coordinate sine
				//-> sine of distance from centerpoint (note the pythagoras to calc the distance)
				color =  0.5 + (offset*0.1) + (0.25 * (color + sin(sqrt(SQUARE(fx - (WIDTH / 2.0)) + SQUARE(fy - (HEIGHT / 2.0))) * (PI / ((scale/2.0) * (sqrt(SQUARE(HEIGHT) + SQUARE(WIDTH))))) + offset*0.2)));

				//sum of x-sine and y-sine
				//color =  0.5 + (((0.5 * sin((fx * (PI / (WIDTH * scale))) + offset )) + (0.5 * sin((fy * (PI / (HEIGHT * scale)) + offset)))) / 2.0);

				//multiplasma
				//color =  (offset*0.1) + (((0.5 * sin(fx * (PI / (WIDTH * scale)))) + (0.5 * sin(fy * (PI / (HEIGHT * scale)))) + (0.5 * sin((fx * (PI / (WIDTH * scale))) + (fy * (PI / (HEIGHT * scale)))) + (0.5 * (color + sin(sqrt(SQUARE(fx - (WIDTH / 2.0)) + SQUARE(fy - (HEIGHT / 2.0))) * (PI / ((scale * 1.0) * (sqrt(SQUARE(HEIGHT) + SQUARE(WIDTH)))))))))) / 4.0);

				setPixelH(x,y, color);

				//setPixelH(x,y, bla[x2 % (16*4)][y2 % (8*4)] + (offset * 0.1));

				//debug
				//setPixelH(x,y, 1.0);

				x2++;
			}
			y2++;
		}
		transmit();

	}

	fclose(port);
}
