#include <stdint.h>
#include "../config.h"
#include "../pixel.h"
#include "../util.h"
#include "../compat/pgmspace.h"
#include "blackhole.h"

// macro for simplifying flash memory access
#define PGM(x) pgm_read_byte(&(x))


/**
 * Integer variant of the sinus function which takes an integer based angle
 * (range from 0 to 63) where sin_i(64) corresponds to sin(2 * M_PI) * 64 and
 * sin_i(16) corresponds to sin(0.5 * M_PI) * 64 (each excluding the fractional
 * part). It uses a lookup table which models one quarter of a full sinus period
 * and calculates the rest of the function from that quarter.
 * @param angle angle based on an integer (range from 0 to 63)
 * @return result of the sinus function normalized to a range from -64 to 64
 */
static signed char sin_i(unsigned char angle) {
	// the aforementioned lookup table
	static signed char const sinus_table[] PROGMEM =
			{0, 6, 12, 19, 24, 30, 36, 41, 45, 49, 53, 56, 59, 61, 63, 64};

	// determine correct index for the lookup table depending on the given angle
	angle %= 64u;
	unsigned char index;
	if (angle < 16) {
		index = angle;
	} else if (angle < 32) {
		index = 31 - angle;
	} else if (angle < 48) {
		index = angle - 32;
	} else {
		index = 63 - angle;
	}

	return (signed char)(PGM(sinus_table[index])) * (angle < 32 ? 1 : -1);
}


/**
 * Integer variant of the cosinus function which takes an integer based angle
 * (range from 0 to 63) where cos_i(64) corresponds to cos(2 * M_PI) * 64 and
 * cos_i(16) corresponds to cos(0.5 * M_PI) * 64. It uses the sin_i function and
 * shifts the given angle by 16 (which corresponds to 90 degrees).
 * @param angle angle based on an integer (range from 0 to 63)
 * @return result of the cosinus function normalized to a range from -64 to 64
 */
static signed char cos_i(unsigned char const angle) {
	return sin_i(angle + 16);
}


#define NUM_CIRCLE 7


/**
 * Draws a black hole like pattern (viewed from different perspectives).
 */
void blackhole(void) {
	pixel circlePoints[NUM_CIRCLE][8];
	unsigned char add = 0;
	signed char firstRadius = 80, helpRadius, angle = 0, x, y;
	// initialize data
	for (int k = 0; k < 800; k++) {
		if (k > 300)
			add = k / 16;
		helpRadius = firstRadius;
		for (unsigned char i = 0; i < NUM_CIRCLE; i++) {
			for (unsigned char j = 0; j < 8; j++) {
				if (j & 1) {
					circlePoints[i][j].x = 64
							+ (cos_i(angle + j * 8) * helpRadius) / 64;
					circlePoints[i][j].y = 64
							+ (sin_i(angle + add + j * 8) * helpRadius) / 64;

				} else {
					circlePoints[i][j].x = 64 +
							(cos_i(angle + j * 8 + 4) * helpRadius) / 64;
					circlePoints[i][j].y = 64 +
							(sin_i(angle + add + j * 8 + 4) * helpRadius) / 64;
				}
				x = circlePoints[i][j].x / 8;
				y = circlePoints[i][j].y / 8;
				if (x < 16 && y < 16)
					setpixel((pixel){x, y}, 3);
			}
			helpRadius = (helpRadius * 2) / 3;
		}
		wait(30);
		clear_screen(0);
		angle++;
		firstRadius += 2;
		if (firstRadius > 119)
			firstRadius = 80;
	}
}
