#include <math.h> // Floating point math is dog slow on AVR, but I don't care.
#include <string.h>
#include "../config.h"
#include "../pixel.h"
#include "../util.h"
#include "fpmath_patterns.h"
#include <inttypes.h>

#define DOUBLE_BUFFERING
#ifdef DOUBLE_BUFFERING
#define BUFFER pixmap_buffer
#else
#define BUFFER pixmap
#endif


/**
 * Pointer to a function which return a value depending on two-dimensional
 * coordinates and a "step" value.
 * @param x x-coordinate
 * @param y y-coordinate
 * @param t a step value which changes for each frame, allowing for animations
 * @return
 */
typedef unsigned char (*fpmath_pattern_func_t)(unsigned char const x,
                                               unsigned char const y,
                                               double t);


/**
 * Calculates the distance between two points.
 * @param x1 x coordinate of the first point
 * @param y1 y coordinate of the first point
 * @param x2 x coordinate of the second point
 * @param y2 y coordinate of the second point
 * @return distance between the points
 */
static double dist(double x1, double y1, double x2, double y2)
{
	return sqrt(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)));
}


/**
 * Draws an animated two dimensional graph for a given function f(x,y,t).
 * @param t_start start value for the function's time variable
 * @param t_stop stop value for the function's time variable
 * @param t_delta value by which the function's timing variable gets incremented
 * @param frame_delay frame delay in ms
 * @param fpPattern function which generates a pattern depending on x, y, t
 */
static void fpmath_pattern(double const t_start,
                           double const t_stop,
                           double const t_delta,
                           unsigned int const frame_delay,
                           fpmath_pattern_func_t fpPattern)
{
#ifdef DOUBLE_BUFFERING
	// double buffering to reduce half painted pictures
	unsigned char pixmap_buffer[NUMPLANE][NUM_ROWS][LINEBYTES];
#endif
	for (double t = t_start; t < t_stop; t += t_delta)
	{
		for (unsigned char y = 0; y < NUM_ROWS; ++y)
		{
			for (unsigned char x = 0; x < NUM_COLS; ++x)
			{
				unsigned char const mask = shl_table[x % 8U];
				unsigned char const x_8 = x / 8u;
				unsigned char const color = fpPattern(x, y, t);
				for (unsigned char p = 0; p < NUMPLANE; ++p)
				{
					if (p <= (color - 1))
					{
						BUFFER[p][y][x_8] |= mask;
					}
					else
					{
						BUFFER[p][y][x_8] &= ~mask;
					}
				}
			}
		}
#ifdef DOUBLE_BUFFERING
		memcpy(pixmap, pixmap_buffer, sizeof(pixmap));
#endif
		wait(frame_delay);
	}
}


#ifdef ANIMATION_PLASMA
#define PLASMA_X (1.0 / (NUM_COLS / (2.0 * M_PI)))

#define INTEGERFUN 1
#ifdef INTEGERFUN

//XXX this is inefficient, as we could as this is a 2*PI sine and we'd really need only half of it - hansi
const uint8_t sine_table[32] =
{
	128,152,176,198,218,234,245,253,255,253,245,234,218,198,176,152,
	128,103,79,57,37,21,10,2,0,2,10,21,37,57,79,103
};

uint8_t sin_i(uint8_t phase)
{
	return sine_table[phase >> 3];
}

#define SQRT0x8000 181
#define SQRT25x3   0x45483
void plasma(void)
{
	uint8_t sqx, x, y, scale = 3, off = 0;

#ifdef DOUBLE_BUFFERING
	// double buffering to reduce half painted pictures
	unsigned char pixmap_buffer[NUMPLANE][NUM_ROWS][LINEBYTES];
#endif

	while (off < 200)
	{
		for(x = 0; x < NUM_COLS; x++)
		{
			sqx = x*x;
			for(y = 0; y < NUM_ROWS; y++)
			{
				unsigned char const mask = shl_table[y % 8U];
				unsigned char const color = (uint16_t)sin_i(sqrt((uint16_t)sqx + ((uint16_t)y*(uint16_t)y)) * (255*255 / ((scale * NUM_COLS) / 2)) + off) * (NUMPLANE - 1) / 255 ;
				unsigned char const x_8 = x / 8u;
				for (unsigned char p = 0; p < NUMPLANE; ++p)
				{
					if (p <= (color - 1))
					{
						BUFFER[p][y][x_8] |= mask;
					}
					else
					{
						BUFFER[p][y][x_8] &= ~mask;
					}
				}
			}
		}

#ifdef DOUBLE_BUFFERING
		memcpy(pixmap, pixmap_buffer, sizeof(pixmap));
#endif
	}
}

#else
/**
 * Draws a simple plasma like pattern.
 */
static unsigned char fpmath_plasma(unsigned char x, unsigned char y, double t)
{
	static double fFunc1[NUM_COLS];
	static double fFunc2CosArg;
	static double fFunc2SinArg;
	if (x == 0 && y == 0)
	{
		fFunc2CosArg = NUM_ROWS * cos(t) + NUM_ROWS;
		fFunc2SinArg = NUM_COLS * sin(t) + NUM_COLS;
	}
	if (y == 0)
	{
		fFunc1[x] = sin(x * PLASMA_X + t);
	}

	return (fFunc1[x] + sin(dist(x, y, fFunc2SinArg, fFunc2CosArg) * PLASMA_X)
			+ 2) * (NUMPLANE - 1) / 2;
}

void plasma(void)
{
#ifndef __AVR__
	fpmath_pattern(0.0, 75, 0.1, 80, fpmath_plasma);
#else
	fpmath_pattern(0.0, 60.0, 0.1, 1, fpmath_plasma);
#endif
}
#endif

#endif /* INTEGERFUN */


#ifdef ANIMATION_PSYCHEDELIC
/**
 * Draws flowing circular waves with a rotating center.
 */
static unsigned char fpmath_psycho(unsigned char x, unsigned char y, double t)
{
	static double fCosinus;
	static double fSinus;
	static double t10;
	if (x == 0 && y == 0)
	{
		fCosinus = NUM_COLS * cos(t);
		fSinus = NUM_ROWS * sin(t);
		t10 = t * 10;
	}
	return (sin(dist(x, y, fCosinus, fSinus) - t10) + 1) * (NUMPLANE - 1);
}

void psychedelic(void)
{
#ifndef __AVR__
	fpmath_pattern(0.0, 75, 0.1, 80, fpmath_psycho);
#else
	fpmath_pattern(0.0, 60.0, 0.1, 1, fpmath_psycho);
#endif
}
#endif
