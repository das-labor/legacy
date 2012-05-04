#include <stdint.h>
#include "../config.h"
#include "../pixel.h"
#include "../util.h"
#include "../compat/pgmspace.h"
#include "blackhole.h"

// macro for simplifying flash memory access
#define PGM(x) pgm_read_byte(&(x))


/**
 * Integer variant of the sine function which takes an integer based angle
 * (range from 0 to 63) where sin_i(64) corresponds to sin(2 * M_PI) * 64 and
 * sin_i(16) corresponds to sin(M_PI_2) * 64 (each excluding the fractional
 * part). It uses a lookup table which models one quarter of a full sine period
 * and calculates the rest of the function from that quarter.
 * @param angle angle based on an integer (range from 0 to 63)
 * @return result of the sine function normalized to a range from -64 to 64
 */
static signed char sin_i(signed char angle) {
	// the aforementioned lookup table
	static signed char const sine_table[] PROGMEM =
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

	return (signed char)(PGM(sine_table[index])) * (angle < 32 ? 1 : -1);
}


/**
 * Integer variant of the cosine function which takes an integer based angle
 * (range from 0 to 63) where cos_i(64) corresponds to cos(2 * M_PI) * 64 and
 * cos_i(16) corresponds to cos(M_PI_2) * 64. It uses the sin_i function and
 * shifts the given angle by 16 (which corresponds to a delta of 90 degrees).
 * @param angle angle based on an integer (range from 0 to 63)
 * @return result of the cosine function normalized to a range from -64 to 64
 */
static signed char cos_i(unsigned char const angle) {
	return sin_i(angle + 16);
}


#define NUM_CIRCLE 7


/**
 * Draws a black hole like pattern (viewed from different perspectives).
 */
void blackhole(void) {
	signed char firstRadius = 80, helpRadius, angle = 0, add = 0;
	// initialize data
	for (unsigned int k = 0; k < 800; k++) {
		if (k > 300) {
			add = k / 16;
		}
		helpRadius = firstRadius;
		for (signed char i = 0; i < NUM_CIRCLE; i++) {
			for (signed char j = 0; j < 64; j += 8) {
				signed char a = (j & 0x08) ? 0 : 4;
				pixel p;
				p.x = (64 + cos_i(angle + j + a) * helpRadius / 64) >> 3;
				p.y = (64 + sin_i(angle + add + j + a) * helpRadius / 64) >> 3;
				if ((p.x < NUM_COLS) && (p.y < NUM_ROWS)) {
					setpixel(p, 3);
				}
			}
			helpRadius = (helpRadius * 2) / 3;
		}
		wait(30);
		clear_screen(0);
		angle++;
		firstRadius += 2;
		if (firstRadius > 119) {
			firstRadius = 80;
		}
	}
}
