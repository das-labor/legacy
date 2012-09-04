/**
 * \defgroup squares Square patterns for the Borg.
 * @{
 */


/**
 * @file squares.c
 * @brief Moves layers of translucent checker boards over each other.
 * @author Christian Kroll
 */


#include <assert.h>
#include <stdint.h>
#include "../config.h"
#include "../util.h"
#include "../pixel.h"
#include "../random/prng.h"
#include "squares.h"

#define STEP_WIDTH (NUMPLANE * 2u)
#define TICK 100
#define CYCLES 200u

/**
 * Moves layers of translucent checker boards over each other.
 */
void squares(void) {
	// add a rotating color map for smooth transitions
	uint8_t nColorMap[NUMPLANE * 2];
	for (uint8_t i = 0; i <  (NUMPLANE * 2); ++i) {
		if (i < (NUMPLANE + 1)) {
			nColorMap[i] = i;
		} else {
			nColorMap[i] = (NUMPLANE * 2) - i;
		}
	}

	uint8_t nOffsets[NUMPLANE] = {0};
	uint8_t nColOffset = 0;
	for (uint8_t nCount = CYCLES; nCount--;) {
		for (uint8_t x = 0; x < NUM_COLS ; ++x) {
			for (uint8_t y = 0; y < NUM_ROWS; ++y) {
				uint8_t nColor = 0;
				for (uint8_t nLayer = 0; nLayer < NUMPLANE; ++nLayer) {
					nColor += (((x + nOffsets[nLayer]) / STEP_WIDTH) + ((y +
							nOffsets[nLayer] + STEP_WIDTH) / STEP_WIDTH)) % 2u;
				}
				setpixel((pixel){x, y},
						nColorMap[(nColOffset + nColor) % (2* NUMPLANE)]);
			}
		}

		// add randomly calculated offsets to each layer starting points
		for (uint8_t i = 0; i < NUMPLANE; ++i) {
			nOffsets[i] = (nOffsets[i] + random8()) % STEP_WIDTH;
		}
		// rotate color map
		nColOffset = (nColOffset + 1) % (NUMPLANE * 2);

		// pause for a moment to ensure that frame transitions are visible
		wait(TICK);
	}
}

/*@}*/
