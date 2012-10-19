#include <stdint.h>
#include "rectangle.h"


uint8_t rectangle_contains (rectangle_t r, uint16_t x, uint16_t y) {
	if (    (r.x < x) && ((r.x + r.w) >= x)
	     &&	(r.y < y) && ((r.y + r.h) >= y)  ) {
		return 1;
	} else {
		return 0;
	}
}
