#include "../compat/pgmspace.h"
#include "../random/prng.h"
#include "../config.h"
#include "../pixel.h"
#include "../util.h"


#define RANDOM8(x) random8(x)
#define PGM(x) pgm_read_byte(&(x))


// straight or non straight line from one point to the other
// value=brightness
static void line(pixel p1, pixel p2, unsigned char value)
{
	char dx, dy, stepx, stepy, fraction;

	dy = p2.y - p1.y;
	dx = p2.x - p1.x;

	if (dy < 0)
	{
		dy = -dy;
		stepy = -1;
	}
	else
	{
		stepy = 1;
	}

	if (dx < 0)
	{
		dx = -dx;
		stepx = -1;
	}
	else
	{
		stepx = 1;
	}

	dx <<= 1;
	dy <<= 1;

	setpixel(p1, value);
	if (dx > dy)
	{
		fraction = dy - (dx >> 1);
		while (p1.x != p2.x)
		{
			if (fraction >= 0)
			{
				p1.y += stepy;
				fraction -= dx;
			}
			p1.x += stepx;
			fraction += dy;
			setpixel(p1, value);
		}
	}
	else
	{
		fraction = dx - (dy >> 1);
		while (p1.y != p2.y)
		{
			if (fraction >= 0)
			{
				p1.x += stepx;
				fraction -= dy;
			}
			p1.y += stepy;
			fraction += dx;
			setpixel(p1, value);
		}
	}
}


// filled_rectangle p1=upper right corner, w=width, h=height ,
// value=brightness
static void filled_rectangle(pixel p1,
                             unsigned char w,
                             unsigned char h,
                             unsigned char value)
{
	unsigned char y;
	for (y = p1.y; y < (p1.y + h); y++)
	{
		line((pixel){p1.x,y}, (pixel){(p1.x+w-1),y}, value);
	}
}


void checkbox()
{
	unsigned char x, delay = 250;

	// quadrat aus linien
	line((pixel){0, 0}, (pixel){7, 0}, 3);
	wait (delay);
	line ((pixel){7, 0}, (pixel){7, 7}, 3);
	wait (delay);
	line ((pixel){7, 7}, (pixel){0, 7}, 3);
	wait (delay);
	line ((pixel){0, 7}, (pixel){0, 0}, 3);
	wait (delay);

	// 2 diagonale lininen
	line ((pixel){7, 7}, (pixel){0, 0}, 3);
	wait (delay);
	line ((pixel){0, 7}, (pixel){7, 0}, 3);
	wait (delay * 3);

	for (x = NUM_COLS; x--;)
	{
		// shift image right
		shift_pixmap_l ();
		wait (30);
	}
}


void movinglines()
{
	unsigned char x, y, n;

	line((pixel){NUM_COLS - 1, NUM_ROWS - 1}, (pixel){NUM_COLS - 1, 0}, 3);

	// linie wandert nach rechts
	for (x = 0; x < NUM_COLS; x++)
	{
		shift_pixmap_l ();
		wait (100);
	}

	// von unten nach oben
	for (y = NUM_ROWS; y--;)
	{
		line ((pixel){0, y}, (pixel){NUM_COLS - 1, y}, 3);
		wait (100);
		line ((pixel){0, y}, (pixel){NUM_COLS - 1, y}, 0);
		//wait(10);
	}

	// eine linie von rechts nach links und 1-8 mal von oben nach unten
	// je nach display format das ganze 4 mal
	for (n = 0; n < 4; n++)
	{
		for (x = 0; x < NUM_COLS - 1; x++)
		{
			y = x % NUM_ROWS;
			line ((pixel){0, y}, (pixel){NUM_COLS - 1, y}, 3);
			line ((pixel){x, 0}, (pixel){x, NUM_ROWS - 1}, 3);
			wait (50);

			line ((pixel){0, y}, (pixel){NUM_COLS - 1, y}, 0);
			line ((pixel){x, 0}, (pixel){x, NUM_ROWS - 1}, 0);
			//wait(10);
		}
	}
}


//rechteckmuster mit helligkeitsstufen
void rectangle1()
{
	unsigned char x, value, xcenter, ycenter, size;
	unsigned int delay = 500;

	xcenter = NUM_COLS / 2;
	ycenter = NUM_ROWS / 2;

	clear_screen(0);

	size = NUM_ROWS;
	value = 3;

	for (x = 8; x > 0; x--)
	{

		if (value < NUMPLANE)
			value++;
		else
			value = 0;

		filled_rectangle((pixel){(xcenter - x), (ycenter - x)},
				size, size, value);
		wait (delay);
		size -= 2;
	}

	wait (delay * 3);
}


// zufallsrechtecke
void rectangles()
{
	unsigned char value, n, x, y, h, w;
	clear_screen(0);

	for (n = 0; n < 60; n++)
	{
		x = RANDOM8() % NUM_COLS;
		y = RANDOM8() % NUM_ROWS;
		h = RANDOM8() % NUM_COLS / 2;
		w = RANDOM8() % NUM_ROWS / 2;
		value = RANDOM8() % (NUMPLANE + 1);

		filled_rectangle((pixel){x, y}, w, h, value);
		wait (500 + RANDOM8 () % 3000);
	}
}


// schräge linien die aufblitzen
void lines1()
{
	line((pixel){0, 15}, (pixel){15, 15}, 3);
	wait(10000);

	unsigned char value, n, x, y, h, w;
	unsigned int delay = 500;
	clear_screen(0);

	for (n = 0; n < 200; n++)
	{
		x = RANDOM8() % NUM_COLS;
		y = RANDOM8() % NUM_ROWS;
		h = RANDOM8() % NUM_COLS;
		w = RANDOM8() % NUM_ROWS;
		value = RANDOM8() % (NUMPLANE + 1);

		line((pixel){x, y}, (pixel){w, h}, value);
		wait (RANDOM8 () % delay);
		line ((pixel){x, y}, (pixel){w, h}, 0);
	}
}


// random dots
void dots1()
{
	unsigned char n, x, y;
	clear_screen(0);

	for (n = 50; n--;)
	{
		x = RANDOM8() % NUM_COLS;
		y = RANDOM8() % NUM_ROWS;

		static unsigned char const color[5] PROGMEM = {1, 2, 3, 2, 1};
		for (unsigned char i = 0; i < 5; ++i)
		{
			setpixel ((pixel){x, y}, PGM(color[i]));
			wait(100);
		}
		wait ((RANDOM8 () % 255) * 10);
	}
}
