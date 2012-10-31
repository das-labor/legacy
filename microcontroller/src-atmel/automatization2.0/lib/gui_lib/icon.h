#ifndef _ICON_H
#define _ICON_H

typedef struct {
	uint16_t size; //overall size of structure in bytes
	uint16_t width; //size of image
	uint16_t height; //size of image
	uint8_t data[]; //pixel data
} icon_t;

#endif
