#ifndef	KS0108_H
#define KS0108_H

#include <avr/io.h>

void dispInit();

void dispClear();

void dispSetPix(uint8_t x, uint8_t y, uint8_t color);

void dispHLine(uint8_t x, uint8_t y, uint8_t x2, uint8_t color);
void dispVLine(uint8_t x, uint8_t y, uint8_t y2);
void dispFillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);

#endif //KS0108_H
