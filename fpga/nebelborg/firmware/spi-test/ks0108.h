#ifndef	KS0108_H
#define KS0108_H

#include "spike_hw.h"



extern void dispInit();         //Initialize LCD
extern void dispClear();

extern void dispSetPix(uint8_t x, uint8_t y, uint8_t color);
extern void dispHLine(uint8_t x, uint8_t y, uint8_t x2, uint8_t color);
extern void dispVLine(uint8_t x, uint8_t y, uint8_t y2);
extern void dispFillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
extern void dispDrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);

extern void dispSetPos(uint8_t x, uint8_t y);
extern void dispPrint(uint8_t * str);


#endif //KS0108_H
