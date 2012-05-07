/**
 * \defgroup mherweg Martin Herweg's animations.
 * @{
 */

/**
 * @file mherweg.c
 * @brief Simple animations for getting started with developing for the Borg.
 * @details The following animations were developed by Martin Herweg (hence the
 *          name) as a personal aid for getting familiar with developing for the
 *          Borg.
 *
 *          Although these animations are rarely used among Borg owners, we left
 *          them in because of their simplicity in hopes that a novice Borg
 *          developer may find them useful.
 * @author Martin Herweg
 */

#include "../compat/pgmspace.h"
#include "../random/prng.h"
#include "../config.h"
#include "../pixel.h"
#include "../util.h"


/** macro for simplifying flash memory access */
#define PGM(x) pgm_read_byte(&(x))


#if NUM_ROWS < 64 && NUM_COLS < 64
	/** use 8 bit operands where feasible */
	typedef signed char operand_t;
#else
	/** use 16 bit operands if either width or height are >= 64 */
	typedef int operand_t;
#endif


/**
 * An implementation of Bresenham's line drawing algorithm.
 * @param p1 first coordinate of the line
 * @param p2 second coordinate of the line
 * @param color brightness level of the line
 */
static void line(pixel p1,
                 pixel const p2,
                 unsigned char const color)
{
	operand_t const dx = p1.x < p2.x ? p2.x - p1.x : p1.x - p2.x;
	operand_t const sx = p1.x < p2.x ? 1 : -1;
	operand_t const dy = p1.y < p2.y ? p2.y - p1.y : p1.y - p2.y;
	operand_t const sy = p1.y < p2.y ? 1 : -1;
	operand_t error = dx - dy;

	while(1)
	{
		setpixel(p1, color);
		if ((p1.x == p2.x) && (p1.y == p2.y))
			break;
		operand_t const error2 = 2 * error;
		if (error2 > -dy)
		{
			error -= dy;
			p1.x += sx;
		}
		if (error2 < dx)
		{
			error += dx;
			p1.y += sy;
		}
	}
}


/**
 * Draws a rectangle at the given coordinates.
 * @param p coordinate of the rectangle's upper right corner
 * @param w width of the rectangle
 * @param h height of the rectangle
 * @param color brightness level of the rectangle
 */
static void filled_rectangle(pixel const p,
                             unsigned char const w,
                             unsigned char const h,
                             unsigned char const color)
{
	// emulate the rectangle via consecutive drawn lines
	for (unsigned char y = p.y; y < (p.y + h); ++y)
	{
		line((pixel){p.x, y}, (pixel){p.x + w - 1, y}, color);
	}
}


/**
 * Draws a checkbox like figure.
 */
static void checkbox()
{
	// some parameters regarding appearance and timing
	unsigned char const color = NUMPLANE; // brightest color
	int const delay = 250, shiftdelay = 30;

	// draw a surrounding square
	static pixel const square[] = {{0, 0}, {7, 0}, {7, 7}, {0, 7}, {0, 0}};
	for (unsigned char i = 0; i < 4; ++i)
	{
		line(square[i], square[i + 1], color);
		wait(delay);
	}

	// draw two diagonal lines
	line((pixel){7, 7}, (pixel){0, 0}, color);
	wait(delay);
	line((pixel){0, 7}, (pixel){7, 0}, color);
	wait(delay * 3);

	// shift image to the right (shift_pximap_l() really shifts to the right)
	for (unsigned char x = NUM_COLS; x--;)
	{
		shift_pixmap_l();
		wait(shiftdelay);
	}
}


/**
 * Animated lines walking over the screen.
 */
static void movinglines()
{
	// some parameters regarding appearance and timing
	unsigned char const color = NUMPLANE; // brightest color
	unsigned char const blank = 0;
	int const delay = 100;

	// a line walking to the right
	line((pixel){NUM_COLS - 1, NUM_ROWS - 1}, (pixel){NUM_COLS - 1, 0}, color);
	for (unsigned char x = 0; x < NUM_COLS; x++)
	{
		shift_pixmap_l();
		wait(delay);
	}

	// a line walking from the lower to the upper border
	for (unsigned char y = NUM_ROWS; y--;)
	{
		line((pixel){0, y}, (pixel){NUM_COLS - 1, y}, color);
		wait(delay);
		line((pixel){0, y}, (pixel){NUM_COLS - 1, y}, blank);
	}

	// quickly moving cross hairs
	for (unsigned char n = 0; n < 4; n++)
	{
		for (unsigned char x = 0; x < NUM_COLS - 1; x++)
		{
			unsigned char y = x % NUM_ROWS;
			line((pixel){0, y}, (pixel){NUM_COLS - 1, y}, color);
			line((pixel){x, 0}, (pixel){x, NUM_ROWS - 1}, color);
			wait(delay / 2);
			line((pixel){0, y}, (pixel){NUM_COLS - 1, y}, blank);
			line((pixel){x, 0}, (pixel){x, NUM_ROWS - 1}, blank);
		}
	}
}


/**
 * Draws a gradient colored square.
 */
static void rectangle1()
{
	// we want a centered square
	unsigned char const xcenter = NUM_COLS / 2, ycenter = NUM_ROWS / 2;
	// it should be as big as the borg's height
	unsigned char size = NUM_ROWS;
	// darkest color as a starting point for the gradient
	unsigned char color = 0;
	// wait about 500 ms between each frame
	int const delay = 500;

	// create a gradient by drawing shrinking rectangles on top of each other
	clear_screen(0);
	for (unsigned char x = 8; x > 0; x--)
	{
		// draw the rectangle and wait for a moment
		filled_rectangle((pixel){(xcenter - x), (ycenter - x)},
				size, size, color);
		wait(delay);

		// iterate through all colors periodically
		++color;
		color %= (NUMPLANE + 1);

		// shrink the dimensions of the succeeding rectangle
		size -= 2;
	}

	wait(delay * 3);
}


/**
 * Draws randomly placed rectangles.
 */
static void rectangles()
{
	unsigned char const blank = 0;
	clear_screen(blank);
	for (unsigned char n = 0; n < 60; n++)
	{
		// randomly choose position, dimensions and color (the rectangle may
		// exceed the actual screen size, only width and height have to be > 0)
		unsigned char const x = random8() % NUM_COLS;
		unsigned char const y = random8() % NUM_ROWS;
		unsigned char const w = random8() % (NUM_COLS / 2) + 1;
		unsigned char const h = random8() % (NUM_ROWS / 2) + 1;
		unsigned char const color = random8() % (NUMPLANE + 1);

		filled_rectangle((pixel){x, y}, w, h, color);

		// wait between 500 and 750 ms
		wait(500 + random8());
	}
}


/**
 * Draws flashing slanted lines.
 */
static void lines1()
{
	unsigned char const blank = 0;
	clear_screen(blank);
	for (unsigned char n = 0; n < 200; n++)
	{
		// randomly choose position, length and color
		unsigned char const x1 = random8() % NUM_COLS;
		unsigned char const y1 = random8() % NUM_ROWS;
		unsigned char const x2 = random8() % NUM_COLS;
		unsigned char const y2 = random8() % NUM_ROWS;
		unsigned char const color = random8() % (NUMPLANE + 1);

		line((pixel){x1, y1}, (pixel){x2, y2}, color);
		wait(random8()); // wait up to 250 ms
		line((pixel){x1, y1}, (pixel){x2, y2}, blank);
	}
}


/**
 * Draws randomly placed dots.
 */
static void dots1()
{
	// some parameters regarding appearance and timing
	int const glimmer_delay = 100;
	unsigned char const blank = 0;
	clear_screen(blank);

	// draw up to 50 dots
	for (unsigned char n = 50; n--;)
	{
		// random coordinates
		unsigned char x = random8() % NUM_COLS;
		unsigned char y = random8() % NUM_ROWS;

		// those dots are glimmering
		static unsigned char const color[5] PROGMEM = {1, 2, 3, 2, 1};
		for (unsigned char i = 0; i < 5; ++i)
		{
			setpixel((pixel){x, y}, PGM(color[i]));
			wait(glimmer_delay);
		}

		// wait up to 2.5 seconds before the next dot is drawn
		wait(random8() * 10);
	}
}


/**
 * Go through all of Martin's animations.
 */
void mherweg()
{
	lines1();
	dots1();
	movinglines();
	checkbox();
	rectangle1();
	rectangles();
}

/*@}*/
