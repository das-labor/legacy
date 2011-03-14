#include <math.h>
#include <stdint.h>
#include "../config.h"
#include "../pixel.h"
#include "../util.h"
#include "plasma.h"


#define FRAME_TICK 80
#define FRAME_COUNT 750
#define PLASMA_X (1.0 / (NUM_COLS / (2.0 * M_PI)))


/**
 * calculates the distance between two points
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
 * draws a simple plasma effect
 */
void plasma(void)
{
	for (double t = 0; t < (FRAME_COUNT / 10.0); t = t + 0.1) {
		for (unsigned char x = 0; x < NUM_COLS; ++x)
		{
			for (unsigned char y = 0; y < NUM_ROWS; ++y)
			{
				double nColor1 = sin(x * PLASMA_X + t) + 1;
				double nColor2 = sin(dist(x, y,	NUM_COLS * sin(t) + NUM_COLS,
					NUM_ROWS * cos(t) + NUM_ROWS) * PLASMA_X) + 1;
				unsigned char nColor =
						((nColor1 + nColor2) * (NUMPLANE - 1)) / 2;
				setpixel((pixel){x, y}, nColor);
			}
		}
		wait(FRAME_TICK);
	}
}
