#include "api.h"

#define TIME_BETWEEN_STEP 15
#define TIME_HOLD_COLOR   100
#define FADE_STEPS        255

void fnordLicht() {
	int i;
	clearImage(red);
	swapAndWait(TIME_HOLD_COLOR);
	for (i = 0; i < 3; i++) {
		clearImage(green);
		fade(TIME_BETWEEN_STEP, FADE_STEPS);
		swapAndWait(TIME_HOLD_COLOR);
		clearImage(blue);
		fade(TIME_BETWEEN_STEP, FADE_STEPS);
		swapAndWait(TIME_HOLD_COLOR);
		clearImage(red);
		fade(TIME_BETWEEN_STEP, FADE_STEPS);
		swapAndWait(TIME_HOLD_COLOR);
	}
}
