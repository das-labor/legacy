#ifndef _H_RECTANGLE
#define _H_RECTANGLE

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
} rectangle_t;

uint8_t rectangle_contains(rectangle_t r, uint16_t x, uint16_t y);

#endif
